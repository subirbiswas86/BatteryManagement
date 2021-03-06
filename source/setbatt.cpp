/**
 * @file setbatt.cpp
 * @brief Defines a battery class.
 *
 * A battery is consist of three cells and switches.
 * Battery provides a output voltage and output current when
 * connected to a load. It discharges the cells untill
 * a cutoff voltage is reached.
 *
 * @author Subir Biswas
 * @date 24/04/2016
 * @see setbatt.hpp
 */

#include "../header/setbatt.hpp"
#include <unistd.h>
#include <iostream> 
#include <thread>	// std::thread

/**
 * @brief Constructor of a Battery pack object
 *
 * Creates and initializes a battery pack.
 * @param void
 * @return void
 */
cBattery::cBattery()
{
	count = 0;
	Vout = 0;
	Iout = 0;
	ElapsedTime = 0;
	CutOffVoltage = 8;	//cut-off at 8 volts
	tollarance = 0.005; //50mV
	SimState.unlock();
	for(int i=0; i<3; i++)
		Switch[i] = false;		
}

/**
 * @brief Returns the switch state of a particular cell
 *
 * @param cell The cell number
 * @return true If the switch is on
 * @return false If the switch is off
 * @see Switch
 */
char cBattery::getSwitchStatus(int cell)
{
	bool result;
	mtx.lock();
	result = Switch[cell];
	mtx.unlock();
	return result;
}

/**
 * @brief Returns the time in milisec for which the battery has ran
 *
 * The Elapsed time accounts for how long it has been running.
 * The Elapsed time resets to 0 when the battery is resetted.
 *
 * @param void
 * @return elapsed time in milisecond
 * @see ElapsedTime
 */
double cBattery::getElapsedTime(void)
{
	double result;
	mtx.lock();
	result = ElapsedTime;
	mtx.unlock();
	return result;
}

/**
 * @brief Resets the battery and its cells
 *
 * Resets the battery to initial state.
 * Resets the connected cells. Resets Elapsed time to 0.
 *
 * @param void
 * @return true  successsfully reseted the battery.
 * @return false failed to reset one or more connected cell.
 */
bool cBattery::reset(void)
{
	bool status = false;
	bool lockStatus = false;
	for(int i=0; i<count; i++)
	{
		lockStatus = Cell[i]->lock(this);
		status = Cell[i]->loadDefaults(this);
		if(!status)
			break;
		if(lockStatus)
			Cell[i]->unlock(this);
	}
	ElapsedTime = 0;
	Vout = 0;
	Iout = 0;
	return status;
}

/**
 * @brief Runs the battery with a load
 *
 * Repeteadly calculate the battery parameters with
 * a load in a fixed interval.
 *
 * @param double load 		Load to be connected with
 * @param double resolution	The interval between two successive calculatein, in miliseconds.
 * @param double speed		Speed of the calculation. reduces the wait time between two calculations.
 * @return true successfully started to run the battery
 * @return false battery is already runing
 */
bool cBattery::run(double load,double resolution,double speed)
{
	if(IsRunning())
		return false;
	SimState.lock();
	Runner = new std::thread(&cBattery::runBattery, this, load, resolution, speed);
	return true;
}

/**
 * @brief Stops a battery if it is running
 *
 * Signals the runner thread to stop and wait for it to stop.
 * @param void
 * @return true successsfully stopped the battery
 * @return false battery wasnot running
 */
bool cBattery::stop(void)
{
	if(IsRunning())
	{
		SimState.unlock();
		Runner->join();
		return true;
	}
	return false;
}


/**
 * @brief returns the Battery output voltage
 *
 * @param void
 * @return the output voltage in volt
 */
double cBattery::getVout(void)
{
	double result;
	mtx.lock();
	result = Vout;
	mtx.unlock();
	return result;
}

/**
 * @brief returns the Battery output current
 *
 * @param void
 * @return the output current in mini Ampere (mA)
 */
double cBattery::getIout(void)
{
	double result;
	mtx.lock();
	result = Iout;
	mtx.unlock();
	return (result*1000);
}

/**
 * @brief returns the Battery simulator cut off voltage
 *
 * @param void
 * @return the output voltage in volts (V)
 */
double cBattery::getCutOffVoltage(void)
{
	double result;
	mtx.lock();
	result = CutOffVoltage;
	mtx.unlock();
	return result;
}

/**
 * @brief Adds a cell to the battery
 *
 * Addes a cell to the battery if it is not ruuning and not full.
 *
 * @param cCell* Adcell Pointer to a cell object
 * @return true successfully added the cell
 * @return false battery is running or the battery is full
 * @see cCell
 */
bool cBattery::addCell(cSingleBatt* AdCell)
{
	if(IsRunning())
		return false;
	if(count>=3)
		return false;
	Cell[count++] = AdCell;
	return true;
}

/**
 * @brief Determines the runner thread is still runnig or not
 *
 * wrapper function to ContinueRunning()
 * @param void
 * @return true Battery is running
 * @return false Battery is not running
 * @see ContinueRunnig
 */
bool cBattery::IsRunning(void)
{
	return ContinueRunning();
}

/**
 * @brief Determines wheather the simulator state is locked or not.
 *
 * Determines if the simulator state is locked. The runner thread will queary
 * and continue to run if simulator state is locked.
 * Simulator state also states whether the thread is runnig or not.
 * @param void
 * @return true Simulator state is locked
 * @return false simulator state is unlocked
 * @see IsRunning
 * @see SimulatorState
 */
bool cBattery::ContinueRunning(void)
{
	if(SimState.try_lock())
	{
		SimState.unlock();
		return false;
	}
	//std::cout << "Test start2";
	return true;
}

/**
 * @brief The runner thread function that updates the battery parameters
 *
 * Runs untill a stop signal is received or battery voltage goes down cutoff voltage
 * in a specific speed and update the battery parameters and cells in a specific interval.
 * writes to a  log file for each run.
 *
 * @param double load 		Load to be connected with
 * @param double resolution	The interval between two successive calculatein, in miliseconds.
 * @param double speed		Speed of the calculation. reduces the wait time between two calculations.
 * @return void
 */
void cBattery::runBattery(double load, double resolution, double speed)
{
	if(resolution == 0 || speed == 0 || load == 0)
		return;
	bool status = false;
	int i;
	double seriesResistance[count];
	for(i=0; i<3; i++)
	{
		status = Cell[i]->lock(this);
		if(!status)
			return;
		seriesResistance[i] = Cell[i]->getSeriesResistance();
	}

	int j, iTemp;
	double dTemp;
	double outVolt=0;
	bool localSwitch[count];
	int sortedCells[count];
	double cellVoltages[count];
	double tempVoltages[count];
	double sourceCurrent[count];
	double ratio;

	while(ContinueRunning())
	{
		//std::cout << "Test case1";
		for(i=0;i<count;i++)
		{
			localSwitch[i] = false;
			sortedCells[i] = i;
			cellVoltages[i] = Cell[i]->getCurrentVoltage();
			tempVoltages[i] = cellVoltages[i];
		}

		for(i=0;i<count;i++)		//rearrange as big to small
		{
			for(j=i+1;j<count;j++)
			{
				if(tempVoltages[i]<tempVoltages[j])
				{
					iTemp=sortedCells[i];
					dTemp=tempVoltages[i];
					sortedCells[i]=sortedCells[j];
					tempVoltages[i]=tempVoltages[j];
					sortedCells[j]=iTemp;
					tempVoltages[j]=dTemp;
				}
			}
		}

		localSwitch[sortedCells[0]] = true;
		outVolt = cellVoltages[sortedCells[0]];
		for(i=1;i<count;i++)
		{
			if((cellVoltages[sortedCells[0]] - cellVoltages[sortedCells[i]]) <= tollarance)
			{
				localSwitch[sortedCells[i]] = true;
				outVolt = cellVoltages[sortedCells[i]];
			}
		}
		ratio = 0;
		for(i=0;i<count;i++)
		{
			if(localSwitch[i])
				ratio += cellVoltages[i]/seriesResistance[i];
		}

		mtx.lock();
		Vout = outVolt;
		Iout = Vout / load;

		for(i=0;i<count;i++)
		{
			if(localSwitch[i])
				sourceCurrent[i] = (Iout*cellVoltages[i])/(ratio * seriesResistance[i]);
			else
				sourceCurrent[i] = 0;
			Switch[i] = localSwitch[i];
			Cell[i]->update(this,localSwitch[i],sourceCurrent[i],resolution);
		}
		mtx.unlock();
		//sleep for Inteval
		usleep(resolution*1000/speed);
		mtx.lock();
		ElapsedTime += resolution;		
		mtx.unlock();

		//if total voltage < MIN, break;
		if(outVolt < CutOffVoltage)
		{
			for(i = 0; i<count; i++)
				localSwitch[i] = false;
			SimState.unlock();
			std::cout<<"\nBattery exhausted\nSimulation completed\n";
			std::cout<<"MybatSim >> ";
		}
	}
	
	for(i =0;i<count;i++)
		Cell[0]->unlock(this);
	return;
}



