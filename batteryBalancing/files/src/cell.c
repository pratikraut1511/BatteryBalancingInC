/**
 * @file cell.c
 * @brief Defines the function for the cell
 *
 * Cell is the primary element of a battery.
 * A battery contains multiple cells connected in parallel.
 * Characteristics of a cell like discharge curve, capacity
 * etc decides the characteristics of the battery.
 * A cell should be added to a battarey for operation.
 * @author Pratik Raut
 * @see cell.h
 */

#include <stdio.h>
#include "../inc/cell.h"
#include <unistd.h>
extern double resLoad;
/**
 * @brief Sets the initialvoltage of the cell
 *
 * Initial voltage is the voltage that the cell voltage
 * starts with when it is updated.
 *
 * @param double initial voltage to set in Volts
 * @param ptrCell contains the pointer to struct cell(representing the current cell of the battery)
 * @return 0 If voltage is not set
 * @return 1 if voltage is set successfully
 * @return 2 if voltage is already set
 */
int setCellVoltage( double voltageToSet, cell *ptrCell)
{
	if(ptrCell->flags.initial_voltage_set == 1)
	{
		return ALREADY_SET;
	}
	else 
	{
		ptrCell->initialVoltage = voltageToSet;
		ptrCell->currentVoltage = voltageToSet;
		ptrCell->flags.initial_voltage_set = 1;
		ptrCell->m1 = (ptrCell->initialVoltage * ptrCell->Drop1) / (ptrCell->Capacity * ptrCell->Shift1);
		ptrCell->m2 = (ptrCell->initialVoltage * (ptrCell->Drop2 - ptrCell->Drop1)) / (ptrCell->Capacity * (ptrCell->Shift2 - ptrCell->Shift1));
        ptrCell->RemainigCapacity = 100;
        ptrCell->Gradient = ptrCell->m1;
        ptrCell->ConstantK = 0;
    }
	return SET;
}


/**
 * @brief get the status of switch of the cell
 *
 * @param ptrCell contains the pointer to struct cell(representing the current cell of the battery)
 * @return 0 If switch is not set
 * @return 1 if switch is set 
 */
int getSwitchStatus( cell *ptrCell )
{
	return ptrCell->flags.switchStatus;
}

/**
 * @brief set Series Resistance of the cell
 *
 * @param value series Resistance for the cell
 * @param ptrCell contains the pointer to struct cell(representing the current cell of the battery)
 * @return 0 If series Resistance is not set
 * @return 1 if series Resistance is set successfully
 * @return 2 if series Resistance is already set
 */
int setSeriesResistance( double seriesResistor, cell *ptrCell)
{
    if(ptrCell->flags.series_resistor_set == 1)
	{
		return ALREADY_SET;
	}
	else
	{
		ptrCell->seriesResistance = seriesResistor;
		ptrCell->flags.series_resistor_set = 1;
	}	
	return SET;
}

/**
 * @brief get the cuurent voltage of the cell 
 *
 * @param ptrCell contains the pointer to struct cell(representing the current cell of the battery)
 *
 */

void getCellVoltage( cell *ptrCell )
{
	printf("\ncurrent voltage == %lf", ptrCell->currentVoltage );
}

/**
 * @brief get the series resistance of the cell 
 *
 * @param ptrCell contains the pointer to struct cell(representing the current cell of the battery)
 *
 */

void getSeriesResistance( cell *ptrCell )
{
	printf("\nseriesResistance== %lf", ptrCell->seriesResistance );
}


void calculation( cell *ptrCell, double runTime, double sleepTime, double current )
{  
    sleep(8);
    printf ("\ncalculation");
	ptrCell->DischargedCapacity += (current * runTime);
	ptrCell->RemainigCapacity = ((ptrCell->Capacity - ptrCell->DischargedCapacity) / ptrCell->Capacity) * 100;
	ptrCell->currentVoltage = ptrCell->currentVoltage - ptrCell->Gradient*current*runTime + ptrCell->ConstantK;
    printf("\n curebt voltage == %lf ",ptrCell->currentVoltage);
	if((ptrCell->Gradient == ptrCell->m1) && (ptrCell->DischargedCapacity >= ((ptrCell->Shift1*ptrCell->Capacity)/100.0)))
	{
		ptrCell->Gradient = ptrCell->m2;
		ptrCell->ConstantK += (ptrCell->m2 - ptrCell->m1)*ptrCell->DischargedCapacity;
	}
    usleep(sleepTime);
}







