/**
 * @file iprocessip.cpp
 * @brief Defines the command inputdata class
 * 
 * @author Subir Biswas
 * @date 24/04/2016
 * @see processip.hpp
 */

#include "../header/processip.hpp"
#include <iostream>
#include <iomanip>
#include <string.h>

/**
 * @brief Constructor of the cparser object
 * 
 * @param void
 * @return void
 */
cprocessIP::cprocessIP()
{

}

/**
 * @brief Gets input from user
 * 
 * gets user input and fills the command, key and values
 * 
 * @param void
 * @return char true if user successfully given the data
 * and false if failed to get the input 
 */
char cprocessIP::getInput(void)
{
	char* buffer = new char[100];
	size_t len = 100;
	std::cout<<"Mybatsim>> ";
	NumberofParam = 0;
	command[0] = '\0';
	key[0] = '\0';
	
	len = getline(&buffer,&len, stdin);
	//std::cout <<len <<"\n";
	if(len <= 0)
		return false;
	
	NumberofParam = sscanf(buffer,"%s%s%lf%lf%lf",command,key,&value[0],&value[1],&value[2]);
	if(NumberofParam < 1)
		return false;
	//std::cout <<NumberofParam << "\n";
	//std::cout <<command <<" " <<key <<" " <<value[0] <<" " <<value[1] <<" " <<value[2] <<"\n";

	delete []buffer;
	
	return true;
}

/**
 * @brief Validate the inputs like command and key
 * 
 * Parse the input and compare the key and command agains a valid set
 * @param const char** valid commands
 * @param const char** valid keys
 * @return char true if successfully validate
 * false if number of user input parameter is less than 1 or valid command or key is NULL
 */
char cprocessIP::ValidateInput(const char** validCommands, const char** validKeys)
{
	int i;

	if(NumberofParam <1)
		return false;
	
	CommandNumber = 0;
	KeyNumber = 0;

	if(validCommands == (const char**)0)
		return false;
	for (i=0;validCommands[i] != (char*)0;i++)
	{
		if(!strcmp(validCommands[i],command))
			break;
	}
	CommandNumber = i;
	//std::cout <<"CommandNumber: " <<CommandNumber <<"\n";

	if(validKeys == (const char**)0)
		return false;
	if(validKeys != (const char**)0)
	{
		if(NumberofParam > 1)
		{
			for (i=0;validKeys[i] != (char*)0;i++)
			{
				if(!strcmp(validKeys[i],key))
					break;
			}
			KeyNumber = i;
		}
	}
	//std::cout <<"KeyNumber: " <<KeyNumber <<"\n";

	for(i=0;i<NumberofParam-2;i++)
		Param[i] = value[i];
	FunctionNumber = CommandNumber * 100 + KeyNumber;
	//std::cout << "FunctionNumber: " << FunctionNumber << "\n";

	return true;
}

/**
 * @brief Returns the function number
 * 
 * Returns the function number used for specfific work 
 * @param void
 * @return double the parameter at index
 */
int cprocessIP::getFunctionNumber(void)
{
	return FunctionNumber;
}

/**
 * @brief Returns the last command
 * 
 * @param void
 * @return char* the last command
 */
char* cprocessIP::getLastCommand(void)
{
	return command;
}

/**
 * @brief Returns the last key entered
 * 
 * @param void
 * @return char* The key
 */
char* cprocessIP::getLastKey(void)
{
	return key;
}

/**
 * @brief Returns the parameter count excluding command and key
 * 
 * @param void
 * @return int number of parameter
 */
int cprocessIP::getParamCount(void)
{
	if((NumberofParam - 2)<0)
		return 0;
	return NumberofParam - 2;
}

/**
 * @brief Returns the specific parameter
 * 
 * Returns the parameter at index 
 * @param int index of the parameter to be returned
 * @return double the parameter at index
 */
double cprocessIP::getIPParam(int index)
{
	return Param[index];
}


