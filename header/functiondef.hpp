/**
 * @file functiondef.hpp
 * @brief Macros for function command
 * 
 * Defines the command that the CLI will accept
 * @author Subir Biswas
 * @date 24/04/2016
 * @see sim_main.cpp
 */

#ifndef  FUNCTIONDEF_H
#define  FUNCTIONDEF_H


#define GETINITV	00 //<get battery voltage
#define GETSERISR	01 //<get series resistance
#define GETLOADR	02 //<get load resistance
#define GETVOLT		03 //<get battery initial voltage
#define GETCUTOFF	04 //<get cut off voltage
#define GETCAP		07 //<get battery capacity
#define GETSHIFT	05 //<get battery discharge curve top value
#define GETDROP		06 //<get battery discharge curve bottom value
#define GETSWTCH	10 //<get switch status

#define SETSRES		101 //<set series resistance <v1> <v2> <V3>
#define SETLOAD		102 //<set load resistance <v1>
#define SETINTV		100 //<set initial voltage <v1> <v2> <v3>

#define SIMSTART	208 //<simulation start
#define SIMSTOP		209 //<simulation stop

#define HELP		300 //<help
#define EXIT		400 //<exit


#endif //FUNCTIONDEF_H

