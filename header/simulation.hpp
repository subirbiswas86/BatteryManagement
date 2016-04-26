/**
 * @file simulation.hpp
 * @brief Implements the simulation process
 *
 * implements the public interface of the simulator
 *
 * @author Subir Biswas
 * @date 25/04/2016
 * @see simulation.cpp
 */
#ifndef  SIMULATION_CLASS
#define  SIMULATION_CLASS

#include "setbatt.hpp"

/**
 * @brief The simulator class
 *
 * Provides an interface to run, stop, pause and resume a battery.
 * Connects the battery with load and maintain speed and resolution of the simulation
 */
class cSimulation
{
	public:
		cSimulation(void);
		cSimulation(int,double);
		bool start(void);
		bool stop(void);
		bool setSpeed(int);
		bool setResolution(double);
		bool connect(cBattery*);
		bool connect(double);
		bool setLoad(double load);
		double getLoad(void);
	private:
		double Load;		///<Load to connect with the battery
		cBattery* BatPack;  	///<Pointer to the Battery to be simulated
		double Speed;		///<simulation speed. used to reduce the delay by this factor
		double Resolution;  	///resolution of the simulation. It determines how often battery will be sampled
		bool BatteryConnected;	///<denotes weather a battery is connected or not
};

#endif //SIMULATION_CLASS

