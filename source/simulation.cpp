/**
 * @file simulation.cpp
 * @brief Implements the simulation process
 *
 * implements the public interface of the simulator
 *
 * @author Subir Biswas
 * @date 25/04/2016
 * @see simulation.hpp
 */
#include "../header/simulation.hpp"
#include <iostream>

/**
 * @brief Constructor for simulator object
 *
 * Creates a simulator object anf initializes
 * speed and resolution by default value
 * @param void
 * @return void
 */
cSimulation::cSimulation(void)
{
	Resolution = 100;
	Speed = 1;
	BatteryConnected = 0;
}

/**
 * @brief Overloaded constructor
 *
 * Another constructor for simulator object
 * This sets the speed and resolution to specific value
 * @param int speed
 * @param double resolution
 * @return void
 */
cSimulation::cSimulation(int multiplier, double res)
{
	if(0 == res)
		Resolution = 100;
	else
		Resolution = res;
	if(0 == multiplier)
		Speed = 1;
	else
		Speed = multiplier;
	BatteryConnected = 0;
}

/**
 * @brief Starts a simulation
 *
 * Makes the battery to run if it is not already
 * @param void
 * @return bool true if successfully starts simulation
 * false if no battery is present or simulation is already running
 */
bool cSimulation::start(void)
{
	if(!BatteryConnected)
		return false;
	if(BatPack->IsRunning())
		return false;
	std::cout<<"calling battery run"<<std::endl;
	return (BatPack->run(Load,Resolution,Speed));
}

/**
 * @brief stops a simulation
 *
 * Stop the battery if it is running and reset it
 * to its initial state
 * @param void
 * @return bool true if successfully stopes a simulation
 * false if no battery is connected or simulation is not runing
 */
bool cSimulation::stop(void)
{
	if(!BatteryConnected)
		return false;
	if(!BatPack->IsRunning())
		return false;
	std::cout<<"calling battery stop"<<std::endl;
	if(BatPack->stop())
	{
		std::cout<<"battery stopped"<<std::endl;
		//return (BatPack->reset());
	}
	return false;
}

/**
 * @brief Pause a simulation
 *
 * Stops a simulation but do not reset the battery state
 * @param void
 * @return bool true if successfully paused a simulation
 * false if simulation is not running or battery is not connected
 */
bool cSimulation::pause(void)
{
	if(!BatteryConnected)
		return false;
	if(!BatPack->IsRunning())
		return false;
	return (BatPack->stop());
}

/**
 * @brief resuems a simulation
 *
 * Starts a simulation from where it was stopped
 * @param void
 * @return bool true if successfully starts
 * false if already running or no battery present
 */
bool cSimulation::resume(void)
{
	return start();
}

/**
 * @brief Sets the simulation speed
 *
 * Sets the simulation speed if battery is present
 * and simulation is not running
 * @param int number of times the simulation speed to be increased
 * @return bool true if successfully sets the speed
 * false id battery not present or simulation is running or
 * the input is less than 1
 */
bool cSimulation::setSpeed(int multiplier)
{
	if(BatteryConnected)
	{
		if(BatPack->IsRunning())
			return false;
	}
	if(0 >= multiplier)
		return false;
	Speed = multiplier;
	return true;
}

/**
 * @brief Sets the resolution of the simulation
 *
 * Sets the sampling interval for battery statistics
 * @param double time in milisecond
 * @return bool true if successfully set
 * false if simulation is running or input is less than or equals 0
 */
bool cSimulation::setResolution(double milisec)
{
	if(BatteryConnected)
	{
		if(BatPack->IsRunning())
			return false;
	}
	if(0 >= milisec)
		return false;
	Resolution = milisec;
	return true;
}

/**
 * @brief Connects a battery to the simulator
 *
 * Connects a battery or replaces othe one if simulation is not running
 * @param cBattery* pointer to the batttery to be connected
 * @return bool true is successfully cnnected.
 * false if simulation is running
 */
bool cSimulation::connect(cBattery* battery)
{
	if(BatteryConnected)
	{
		if(BatPack->IsRunning())
			return false;
	}
	BatPack = battery;
	BatteryConnected = true;
	return true;
}

/**
 * @brief Connects a load tothe simulator
 *
 * Connects the load to the simulator. this load
 * is to be attached to the battery
 * @param double load to be connected in Ohms
 * @return bool true if successful
 * false if simulation is running
 */
bool cSimulation::connect(double load)
{
	if(BatteryConnected)
	{
		if(BatPack->IsRunning())
			return false;
	}
	Load = load;
	return true;
}

/**
 * @brief REturns the connected load
 *
 * @param double load to be connected in Ohms
 * @return bool true
 */
bool cSimulation::setLoad(double load)
{
	Load = load;
	return true;
}

/**
 * @brief REturns the connected load
 *
 * @param void
 * @return double connected load in Ohm
 */
double cSimulation::getLoad(void)
{
	return Load;
}
