/**
 * @file sim_main.c
 * @brief 3 diffeernt Battery simulator.
 *
 * Implements and control of for the battery simulator.
 *
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
const char* validKeys[] = {"initvoltage","seriesres","loadres","cvoltage","cutoff","sourcecurr","remaincap","capacity","start","stop","switch",(char*)0}; 

/**
 * @brief Shows the help text.
 *
 * @param void
 * @return void
 */
void showHelp(void)
{
	std::cout<<"\nMYBATSIM \n";
	std::cout<<"\nNAME\n\tMybatsim - Assignment for Battery Simulation\n";
	std::cout<<"\nSYNOPSIS\n\tMybatsim\n";
	std::cout<<"\nDESCRIPTION\n\tMybatsim simulates a baterry pack with three parallel connected cells connected through switches.\
			\n\tThe simulator will start a command line interface and accepts command to view and set various parameters.\
			\n\tGeneric command format is: MybatSim>> <command> <key> <value1> <value2> <value3>\n";
	std::cout<<"\nCOMMANDS AND KEYWORDS\n\
			\n\tset   \tSets a value. Format: MybatSim>> <set> <key> <value1> <value2> <value3>\
			\n\t      \tUnnecessary options/arguments are ignored. If required value is not provided, by default it takes 0.\
			\n\t      \tValid keys are: initvoltage, seriesres, and loadres (only loadres have one argument)\
			\n\tget   \tReturns a parameter. Format: MybatSim>> <get> <key>\
			\n\t      \tValid keys are: initvoltage, seriesres, loadres, cvoltage, cutoff, sourcecurr, remaincap, and switch\
			\n\tsim   \tStarts or stops the simulator. Format: MybatSim>> <sim> <start> / <stop>\
			\n\thelp  \tPrints this help text.\
			\n\texit  \tExits the simulator. If the simulator is still running, tries to stop it first.\n";
	std::cout<<"\nDEFAULT VALUES\n\
			\n\tInitial voltages  : 12.5 V, 14.1 V, 12.9 V\
			\n\tSeries resistances: 20 Ohm,  30 Ohm,  40 Ohm\
			\n\tLoad              : 150 Ohm\
			\n\tCapacity          : 800 mAH\
			\n\tshift             : 10\
			\n\tdrop              : 5\
			\n\tCutoff voltage    : 8 V\n";
	return;
}

/**
 * @brief handle the main operation
 *
 * @param void
 * @return int
 */
int main (void)
{
	cprocessIP inputdata;
	cBattery battstatus;
	cSingleBatt battpack[3];
	cSimulation Simulator;

	char exit_loop = false;
	int Function = 0;
	int i = 0;

	//battpack[0].setCapacity(2000);		//for 2000 mAh
	//battpack[1].setCapacity(2600);		//for 2600 mAh
	//battpack[2].setCapacity(3000);		//for 3000 mAh

	battpack[0].setInitialVoltage(12.5);	
	battpack[1].setInitialVoltage(14.1);	
	battpack[2].setInitialVoltage(12.9);
	
	battpack[0].setSeriesResistance(20);
	battpack[1].setSeriesResistance(30);
	battpack[2].setSeriesResistance(40);
	
	battstatus.addCell(&battpack[0]);
	battstatus.addCell(&battpack[1]);
	battstatus.addCell(&battpack[2]);

	Simulator.connect(&battstatus);
	Simulator.connect(10); // set load at 150 ohm
	Simulator.setSpeed(100000);
	Simulator.setResolution(10);

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
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Initiate Battery Voltage in Volts:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Batery " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getInitialVoltage() <<" V.\n";
					break;

					case GETSERISR:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Series Resistance:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Res " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getSeriesResistance() <<" Ohm.\n";
					break;
					case GETLOADR:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Load Resistance in Ohms:\n";
						std::cout <<Simulator.getLoad() <<" Ohm."<<std::endl;
					break;

					case GETVOLT:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Battery Voltage:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Batery " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getCurrentVoltage() <<" V.\n";
					break;

					case GETCUTOFF:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Battery Voltage cut off in Volts:\n";
						std::cout <<battstatus.getCutOffVoltage() <<" V."<<std::endl;
					break;

					case GETCAP:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Battery Capacity in mAh:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Capacity " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getCapacity() <<" mAh.\n";
					break;

					case GETSCURR:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Presently source current through the Battery:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Battery " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getSourceCurrent() <<" A.\n";
					break;

					case GETRCAP:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
						std::cout <<"Capacity remaining of the battery:\n";
						for(i =0; i<3 ; i++)
							std::cout <<"Capacity " <<i <<": " <<std::fixed <<std::setprecision(3) 
							<<battpack[i].getRemainingCapacityPercentage() <<" %\n";
					break;

					case GETSWTCH:
						if(inputdata.getParamCount() > 0)
							std::cout<<"Extra values omitted."<<std::endl;
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
						if(inputdata.getParamCount() < 3)
						{
							std::cout<<"Insufficient arguments. Please Specify series resistance."<<std::endl;
							break;
						}
						std::cout <<"Initiate Series Resistance at:\n";
						for( i=0;i<inputdata.getParamCount() && i<3;i++)
						{
							if(battpack[i].setSeriesResistance(inputdata.getIPParam(i)))
								std::cout <<i+1<<": Done." <<std::endl;
							else
								std::cout <<i+1<<": Failed." <<std::endl;
						}
						if(inputdata.getParamCount() > 3)
							std::cout<<"Extra values omitted."<<std::endl;
					break;

					case SETLOAD:
						if(inputdata.getParamCount() < 1)
						{
							std::cout<<"Insufficient arguments. Please Specify load resistance."<<std::endl;
							break;
						}
						std::cout <<"Initiate Load resistance at:\n";
						if(Simulator.setLoad(inputdata.getIPParam(0)))
							std::cout <<1 <<": Done." <<std::endl;
						else
							std::cout <<1 <<": Failed." <<std::endl;
						if(inputdata.getParamCount() > 1)
							std::cout<<"Extra values omitted."<<std::endl;
					break;

					case SETINTV:
						if(inputdata.getParamCount() < 3)
						{
							std::cout<<"Insufficient arguments. Please Specify battery voltage."<<std::endl;
							break;
						}
						std::cout <<"Initiate Battery Voltage at:\n";
						for( i=0;i<inputdata.getParamCount() && i<3;i++)
						{
							if(battpack[i].setInitialVoltage(inputdata.getIPParam(i)))
								std::cout <<i+1 <<": Done." <<std::endl;
							else
								std::cout <<i+1 <<": Failed." <<std::endl;
						}
						if(inputdata.getParamCount() > 3)
							std::cout<<"Extra values omitted."<<std::endl;
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
						showHelp();
					break;

					case EXIT:
						Simulator.stop();
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



