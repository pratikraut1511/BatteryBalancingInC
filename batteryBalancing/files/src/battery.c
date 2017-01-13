/**
 * @file battery.c
 * @brief Implementation of function for Battery 
 *
 * @author Pratik Raut
 * @see battery.h
 */

#include <stdio.h>
#include "../inc/battery.h"
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>

void simulator( cell *ptrCell , double current )
{
    double runTime = 10;
    double sleepTime = 200;
     printf("\n in battery.c curebt voltage == %lf ",ptrCell->currentVoltage);
    calculation( ptrCell, runTime, sleepTime, current );
    
}

/**
 * @brief set the initial voltage for each cell present in the battery
 *
 * @param inputVal contains the use input
 * @param ptrCell contains the pointer to struct cell(representing the current cell of the battery)
 * @return 0 If voltage is not set
 * @return 1 if voltage is set successfully
 * @return 2 if voltage is already set
 */
int set_voltage(double inputVal[], cell *ptrCell)
{
	double val;
	int counter;
	int ret = 0;
	for(counter = 0; counter < 3; counter++)
	{
		val = inputVal[counter];
		ret = setCellVoltage( val, ptrCell);
		if( !ret )
			return ret;
		ptrCell++;
	}
	return ret;
}

/**
 * @brief set the load resisitance
 *
 * @param inputVal contains the use input
 * @return 0 If load resistance is not set
 * @return 1 if load resistance is set successfully
 * @return 2 If load resistance is already set
 */
int set_load(double inputVal[])
{
	resLoad = inputVal[0];
	get_loadResistor();
	return 1;
}

/**
 * @brief set the series resisitance for each cell present in the battery
 *
 * @param inputVal contains the use input
 * @param ptrCell contains the pointer to struct cell(presenting the current cell of the battery)
 * @return 0 If series resistance is not set
 * @return 1 if series resistance is set successfully
 * @return 2 If series resistance is already set
 */
int set_resistor(double inputVal[], cell *ptrCell)
{
   double val;
	int counter;
	int ret = 0;
	for(counter = 0; counter < 3; counter++)
	{
		val = inputVal[counter];
		ret = setSeriesResistance( val, ptrCell);
		if( !ret )
			return ret;
		ptrCell++;
	}
	return ret;
}

/**
 * @brief get the load resisitance 
 *
 */
void get_loadResistor()
{
	printf("\nload resistance %lf", resLoad);
}

/**
 * @brief get the voltage for each cell present in the battery
 *
 * @param ptrCell contains the pointer to struct cell(presenting the current cell of the battery)
 *
 */
void get_voltage(cell *ptrCell)
{
	int counter;
    for(counter = 0; counter < 3; counter++)
	{
		getCellVoltage( ptrCell );
		ptrCell++;
	}
}

/**
 * @brief get the series resisitance for each cell present in the battery
 *
 * @param ptrCell contains the pointer to struct cell(presenting the current cell of the battery)
 *
 */
void get_seriesResistance(cell *ptrCell)
{
	int counter;
    for(counter = 0; counter < 3; counter++)
	{
		getSeriesResistance( ptrCell );
		ptrCell++;
	}
}

/**
 * @brief get the switch status for each cell present in the battery
 *
 * @param ptrCell contains the pointer to struct cell(presenting the current cell of the battery)
 *
 */
void get_switch_status(cell *ptrCell)
{
   int counter, ret;
   for(counter = 0; counter < 3; counter++)
	{
		ret = getSwitchStatus( ptrCell );
		if (ret == ON)
		{
			printf("\nSwitch[%d] = ON", counter);
		}
		else
		{
			printf("\nSwitch[%d] = OFF",counter);
		}
		ptrCell++;
	}
}
