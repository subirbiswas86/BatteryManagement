/**
 * @file processip.hpp
 * @brief Defines the command inputdata class
 * 
 * @author Subir Biswas
 * @date 24/04/2016
 * @see processip.cpp
 */

#ifndef  PROCESSIP_CLASS
#define  PROCESSIP_CLASS

/**
 * @brief defines a user Inputss
 *
 * get different user values
 *
 * @see processIP
 **/

class cprocessIP
{
	public:	
		cprocessIP();
		char getInput(void);
		char ValidateInput(const char** , const char** );
		int getFunctionNumber(void);
		char* getLastCommand(void);
		char* getLastKey(void);
		int getParamCount(void);
		double getIPParam(int index);
	private:	
		char command[20];	//<Holds the command
		char key[20];		//<Holds the key
		double value[3];    	//<Holds the initial input parameters
		int CommandNumber;  	//<Index of the command
		int KeyNumber;      	//<Index of the key
		double Param[3];    	//<Holds the parameters (3 at max)
		int NumberofParam;  	//<Holds the number of parameter
		int FunctionNumber; 	//<Unique function number from command - key combination
};
#endif //PROCESSIP_CLASS
