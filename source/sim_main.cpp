/**
 * @file sim_main.c
 * @brief 3 diffeernt Battery simulator.
 *
 * Implements and control of for the battery simulator.
 * @author Subir Biswas
 * @date 24/04/2016
 * @see functiondef.hpp
*/

#include "../header/processip.hpp"
#include "../header/functiondef.hpp"
#include "../header/singlebatt.hpp"
#include "../header/setbatt.hpp"
#include "../header/simulation.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <unistd.h>


const char* validCommands[] = {"get","set","sim","help","exit",(char*)0};
const char* validKeys[] = {"initvoltage","seriesres","loadres","voltage","cutoff","shift","drop","capacity","start","stop","switch",(char*)0}; 

int main (void)
{
	processIP inputdata;
	cBattery battstatus;
	cSingleBatt battpack[3];
	cSimulation Simulator;

	char exit_loop = false;
	int Function = 0;
	int i = 0;

	//battpack[0].setCapacity(2000);
	//battpack[1].setCapacity(2600);
	//battpack[2].setCapacity(3000);

	//battpack[0].setInitialVoltage(12);	
	//battpack[1].setInitialVoltage(14);	
	//battpack[2].setInitialVoltage(15);
	
	//battpack[0].setSeriesResistance(20);
	//battpack[1].setSeriesResistance(30);
	//battpack[2].setSeriesResistance(40);
	
	battstatus.addCell(&battpack[0]);
	battstatus.addCell(&battpack[1]);
	battstatus.addCell(&battpack[2]);

	Simulator.connect(&battstatus);
	Simulator.setLoad(10); // set load at 150 ohm
	Simulator.setSpeed(100000);
	Simulator.setResolution(10);

	battpack[0].initialise();
	battpack[1].initialise();
	battpack[2].initialise();

	std::cout <<"Assignment for Battery Simulation\n";
	

	while(!exit_loop)
	{
		if(inputdata.getInput())
		{
			if(inputdata.ValidateInput(validCommands,validKeys))
			{
				Function = inputdata.getFunctionNumber();

				switch(Function)
				{
					case GETINITV:						
						std::cout <<"Initiate Battery Voltage in Volts:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Batery " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getInitialVoltage() <<" V.\n";
					break;

					case GETSERISR:
						std::cout <<"Series Resistance:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Res " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getSeriesResistance() <<" Ohm.\n";
					break;
					case GETLOADR:
						std::cout <<"Load Resistance in Ohms:\n";
						std::cout <<Simulator.getLoad() <<" Ohm."<<std::endl;
					break;

					case GETVOLT:
						std::cout <<"Battery Voltage:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Batery " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getCurrentVoltage() <<" V.\n";
					break;

					case GETCUTOFF:
						std::cout <<"Battery Voltage cut off in Volts:\n";
					break;

					case GETCAP:
						std::cout <<"Battery Capacity in mAh:\n";
					break;

					case GETSHIFT:
						std::cout <<"Battery Discharge curve high level value in Volts:\n";
					break;

					case GETDROP:
						std::cout <<"Battery Discharge curve low level value in Volts:\n";
					break;

					case GETSWTCH:
						std::cout <<"Switch status:\n";
						for(i =0; i<3 ; i++)
						{
							std::cout <<"Switch " <<i <<": ";
							if(battstatus.getSwitchStatus(i))
								std::cout <<"ON\n";
							else
								std::cout <<"OFF\n";
						}
					break;

					case SETSRES:
						std::cout <<"Initiate Series Resistance at:\n";
						for( i=0;i<inputdata.getParamCount() && i<3;i++)
						{
							if(battpack[i].setSeriesResistance(inputdata.getIPParam(i)))
								std::cout <<i+1<<": Done." <<std::endl;
							else
								std::cout <<i+1<<": Failed." <<std::endl;
						}
					break;

					case SETLOAD:
						std::cout <<"Initiate Load resistance at:\n";
						if(Simulator.setLoad(inputdata.getIPParam(0)))
							std::cout <<1 <<": Done." <<std::endl;
						else
							std::cout <<1 <<": Failed." <<std::endl;
					break;

					case SETINTV:
						std::cout <<"Initiate Battery Voltage at:\n";
						for( i=0;i<inputdata.getParamCount() && i<3;i++)
						{
							if(battpack[i].setInitialVoltage(inputdata.getIPParam(i)))
								std::cout <<i+1 <<": Done." <<std::endl;
							else
								std::cout <<i+1 <<": Failed." <<std::endl;
						}
					break;

					case SIMSTART:
						if(inputdata.getParamCount() > 0)
							std::cout <<"Extra parameters omitted." <<std::endl;
						if(Simulator.start())
							std::cout <<"Simulation started." <<std::endl;
						else
							std::cout <<"Simulation already running." <<std::endl;
					break;

					case SIMSTOP:
						if(inputdata.getParamCount() > 0)
							std::cout <<"Extra parameters omitted." <<std::endl;
						if(Simulator.stop())
							std::cout <<"Simulation stopped." <<std::endl;
						else
							std::cout <<"Simulation is not running currently." <<std::endl;
					break;

					case HELP:

					break;

					case EXIT:
						std::cout <<"Simualtion Process is aborted" <<std::endl;
						exit_loop = true;
					break;

					default:
						if(Function >=500)
							std::cout <<inputdata.getLastCommand() <<" is not a valid Command" <<std::endl;
						else
							std::cout <<inputdata.getLastKey() <<" is not a valid key for " 							<<inputdata.getLastCommand() <<" command" <<std::endl;
					break;
				}
			}
			else
				std::cout <<std::endl <<"Input correctly not recorded." <<std::endl;
		}
	}

	return false;
}



