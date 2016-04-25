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
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>


const char* validCommands[] = {"get","set","sim","help","exit",(char*)0};
const char* validKeys[] = {"initvoltage","seriesres","loadres","voltage","cutoff","shift","drop","capacity","start","stop","getswitch",(char*)0}; 

int main (void)
{
	processIP inputdata;
	Battery battstatus;
	SingleBatt singlebatt[3];

	char exit_loop = false;
	int Function = 0;
	int i = 0;

	
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
					case GETVOLT:						
						std::cout <<"Battery Voltage in Volts:\n";
					break;

					case GETSERISR:
						std::cout <<"Series Resistance in Ohms:\n";
					break;

					case GETLOADR:
						std::cout <<"Load Resistance in Ohms:\n";
					break;

					case GETINITV:
						std::cout <<"Initiate Battery Voltage in Volts:\n";
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
							if(singlebatt[i].setSeriesResistance(inputdata.getIPParam(i)))
								std::cout <<i+1<<": Done." <<std::endl;
							else
								std::cout <<i+1<<": Failed." <<std::endl;
						}
					break;

					case SETLOAD:
						std::cout <<"Initiate Load resistance at:\n";
					break;

					case SETINTV:
						std::cout <<"Initiate Battery Voltage at:\n";
						for( i=0;i<inputdata.getParamCount() && i<3;i++)
						{
							if(singlebatt[i].setInitialVoltage(inputdata.getIPParam(i)))
								std::cout <<i+1<<": Done." <<std::endl;
							else
								std::cout <<i+1<<": Failed." <<std::endl;
						}
					break;

					case SIMSTART:
						std::cout <<"Simulation started." <<std::endl;
					break;

					case SIMSTOP:
						std::cout<<"Simulation stopped."<<std::endl;
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



