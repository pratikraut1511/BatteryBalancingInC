/**
 * @file battery.h
 * @brief Defines header for the battery.
 *
 * A battery is consist of one or more cells and switches.
 * Battery provides a output voltage and output current when
 * connected to a load. It discharges the cells untill
 * a cutoff voltage is reached.
 *
 *
 * @author Pratik Raut
 **/
#ifndef BATTERY_H
#define BATTERY_H

#include <sys/timeb.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include "../inc/cell.h"
#include <time.h>

/**
 * @brief struct batteryDetails defines a battery
 **/
typedef struct batteryDetails
{
	double outputVoltage;
	double outputCurrent;
	double cutOffVoltage;
	struct timeb Stime;
	double Etime;
	cell *ptrCell;
	int currentCell;
}battery;

double resLoad;

int set_voltage(double inputVal[], cell *ptrCell);
void get_voltage(cell *ptrCell);

int set_load(double inputVal[]);
void get_loadResistor();

int set_resistor(double inputVal[], cell *ptrCell);
void get_seriesResistance(cell *ptrCell);

void get_switch_status( cell *ptrCell);

void simulator( cell *ptrCell, double current );

#endif




