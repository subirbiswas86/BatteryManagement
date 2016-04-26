/**
 * @file setbatt.hpp
 * @brief Defines a battery class.
 *
 * A battery is consist of three cells and switches.
 * Battery provides a output voltage and output current when
 * connected to a load. It discharges the cells untill
 * a cutoff voltage is reached.
 *
 * @author Subir Biswas
 * @date 24/04/2016
 * @see setbatt.cpp
 */

#ifndef  BATTERYSET_CLASS
#define  BATTERYSET_CLASS

#include "singlebatt.hpp"
#include <thread>	// std::thread
#include <mutex>	// std::mutex


/**
 * @brief defines a battery
 *
 * get & set different values
 *
 * @see battery
 **/



class cBattery
{
	public:
		cBattery();
		bool reset(void);
		bool run(double load,double resolution,double speed);
		bool stop(void);
		double getVout(void);
		double getIout(void);
		double getElapsedTime(void);
		bool addCell(cSingleBatt* AdCell);
		char getSwitchStatus(int Cell);
		bool IsRunning(void);
		double getLoadResistance(void);
		double getCutOffVoltage(void);

	private:
		cSingleBatt *Cell[3];		///<Holds the cells that are added. @see addCell
		bool Switch[3];			///<A switch for each cell
		double Vout;			///<Output voltage of the battery in Volts.
		double Iout;			///<Output current of the battery in Ampere.
		double ElapsedTime;		///<Time for which the battery is running in mS.
		double CutOffVoltage;		///<Battery will be disconnected when Output voltage drops below this. expressed in Volts.
		double tollarance;
		std::thread* Runner;		///<Pointer to the runner thread
		std::mutex SimState;		///<Used to signal thread terminaton event
		void runBattery(double load,double resolution,double speed);
		bool ContinueRunning(void);
		int count;				///<number of cells added in the battery
		std::mutex mtx; 		///<Lock to synchronize access to members from different thread and unlock

		
};

#endif //BATTERYSET_CLASS

