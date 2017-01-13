/**
 * @file cell.h
 * @brief Defines a function for the cell.
 *
 * Cell is the primary element of a battery.
 * A battery contains multiple cells connected in parallel.
 * Characteristics of a cell like discharge curve, capacity
 * etc decides the characteristics of the battery.
 *
 * @author Pratik Raut
 **/

#ifndef CELL_H
#define CELL_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>

/**
 * @brief defines a enum switchNo 
 * 
 **/
enum switchNo
{
	SWITCH1 = 1,
	SWITCH2,
	SWITCH3
};

/**
 * @brief defines a enum switch_status 
 * 
 **/
enum switch_status
{
    OFF = 0,
    ON = 1
};

/**
 * @brief defines a enum return_Type 
 * 
 **/
typedef enum return_Type
{
	FAILED = 0,
	SET,
	ALREADY_SET,
}returnType;

/**
 * @brief defines a struct status_Flags 
 * 
 **/
typedef struct status_Flags
{
   unsigned int initial_voltage_set : 1;
   unsigned int series_resistor_set : 1;
   unsigned int switchStatus : 1;
	unsigned int cutOffReached : 1;
}statusFlags;

/**
 * @brief defines a struct cell 
 *
 **/
typedef struct cellDetails
{
	double initialVoltage;  //initial voltage of the cell
	double seriesResistance; // series resistance of the cell
	double Capacity;         //capacity of the cell.
	double currentVoltage;  //current voltage of the cell
	double Shift1;				//First gradient change in the discharge curve. expressed as percentage of descharged capacity.
	double Shift2;				//Second gradient change in the discharge curve. expressed as percentage of deschargedcapacity.
	double Drop1;				//Voltage drop at shift1. expressed as percentage
	double Drop2;				//Voltage drop at shift2. expressed as percentage
	double Gradient;			//Current slop of the discharge curve. Measured as Voltage per Capacity
	double ConstantK;			//Constant factor of discharge curve equation. Measeured as Volt.
	double m1,m2;		     	//slope.
	double DischargedCapacity;  //Capacity of the cell already discharged. Unit AmS.
	double RemainigCapacity;	//Percentage of capacity remaining. (%)
	statusFlags flags;
}cell;

int getSwitchStatus( cell *ptrCell );
int setCellVoltage( double voltageToSet, cell *ptrCell);
int setSeriesResistance( double seriesResistor, cell *ptrCell);
int initialiseCell();
void getCellVoltage( cell *ptrCell );
void getSeriesResistance( cell *ptrCell );
void calculation( cell *ptrCell, double runTime, double sleepTime, double current );

#endif