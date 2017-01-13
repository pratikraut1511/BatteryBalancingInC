#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../inc/battery.h"
#include "../inc/cell.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>


#define SET_VOLT                     "set volt"
#define GET_VOLT                     "get volt"
#define SET_RESISTOR                 "set resis" 
#define GET_RESISTOR                 "get resis" 
#define SET_LOAD                     "set load"
#define GET_LOAD                     "get load"
#define START                        "start"

extern double resLoad;
sem_t bin_sem;

void *thread_simulator(void *arg)
{
	battery *foo = (battery*)arg;	
    cell *ptrLocalCell = foo->ptrCell;
	int localSwitch[3];
	int sortedCells[3];
	int i, count;
    int j, iTemp;
	double dTemp;
	double outVolt;
	double cellVoltages[3];
	double tempVoltages[3];
	int stop = 0;
	
    sem_wait(&bin_sem);
   
	while (!stop)
	{
		for(i=0;i<3;i++)
		{
			localSwitch[i] = 0;
			sortedCells[i] = i;
			cellVoltages[i] = foo->ptrCell->currentVoltage;
			tempVoltages[i] = cellVoltages[i];
            foo->ptrCell++;
		}
        for(i = 0; i < 3; i++)
		{
			foo->ptrCell--;
		}
		printf("\nsorting");
		for(i=0;i<3;i++)
		{
			for(j=i+1;j<3;j++)
			{
				if(tempVoltages[i]<tempVoltages[j])
				{
					iTemp=sortedCells[i];
					dTemp=tempVoltages[i];
					sortedCells[i]=sortedCells[j];
					tempVoltages[i]=tempVoltages[j];
					sortedCells[j]=iTemp;
					tempVoltages[j]=dTemp;
				}
			}
		}
			
		localSwitch[sortedCells[0]] = 1;
		outVolt = cellVoltages[sortedCells[0]];

		foo->outputVoltage = outVolt;
		foo->outputCurrent = foo->outputVoltage / resLoad;
		foo->ptrCell->flags.switchStatus = 1;
           
		for(i = 0; i<3; i++, foo->ptrCell++)
		{
			if( (foo->outputVoltage == foo->ptrCell->currentVoltage) )
			{
				count = i;
                break;
			}
		}
        foo->ptrCell = ptrLocalCell;
        for (i =0; i < count; i++ )
        {
            foo->ptrCell++;
        }
        simulator(foo->ptrCell, foo->outputCurrent);
        foo->outputVoltage = foo->ptrCell->currentVoltage;
		foo->ptrCell = ptrLocalCell;
		if( foo->outputVoltage <= foo->cutOffVoltage)
		{ 
			printf(" ");
			printf(" ");
			printf(" ");
         printf("\nSimulation ended...");
         sleep(10);
			stop = 1;
			for(i = 0; i<3; i++, foo->ptrCell++)
				foo->ptrCell->flags.switchStatus = 0;				
		}
	}
}


int main()
{
	char str[50];
	cell Cell[3] = { { 0,0,800,0,10,85,5,10,0,0,0,0}, { 0,0,800,0,10,85,5,10,0,0,0,0}, { 0,0,800,0,10,85,5,10,0,0,0,0} };
	battery Battery = {0, 0, 7.0, 0, 0, 0};
	Battery.ptrCell = Cell;
	char buffer[50], str1[50];
    double values[20];
	int ret;
    char *pch;
	int token;

	battery *ptrBattery;
	ptrBattery = &Battery;
	int res;
	pthread_t a_thread;
	void *thread_result;
	int voltSet= 0, sResistance = 0, lResistance = 0;
	res = sem_init(&bin_sem, 0, 0);
	
	if (res != 0) 
	{
		perror("Semaphore initialization failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_create(&a_thread, NULL, thread_simulator, ptrBattery);
	if (res != 0) 
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printf("\nEnter your choice:");
		strcpy(str,"");
		strcpy(buffer,"");
		token=0;
		scanf ("%[^\n]%*c",str);
		
		strcpy(str1,str);
        if(!strncmp(str,START,5))
        {
            if(voltSet ==1 && sResistance == 1 && lResistance == 1)
            {
                printf("\n simulation started");
                ftime(&Battery.Stime);
                sem_post(&bin_sem);
            }
            else
            {
                printf("\n Please first set cell voltage , load resistance and series resistance");
            }   
        }
        else if(!strncmp(str1,SET_VOLT,8))
        {
            pch = strtok (str," ");
            while (pch != NULL)
            {
                buffer[token] = *pch;
                token++;
                pch = strtok (NULL," ");
            }

            if( token == 5)
            {
                sscanf(&str1[9],"%lf %lf %lf", &values[0], &values[1], &values[2]);
                Battery.ptrCell = Cell;
                ret = set_voltage(values, Battery.ptrCell);
     
                if (ret == ALREADY_SET)
                {
                   printf("\nvoltages are already set\n");
                }else if( ret == SET )
                {
                   printf("\nvoltage successfully set\n");
                }
                else 
                {
                    printf("\rfailed to set voltage\n");
                }
                voltSet = 1;
            }
            else
            {
                printf("\rPlese enter API as below\nset volt 10.2 11 12");
            }
        }
        else if(!strncmp(str,GET_VOLT,8))
        {
            get_voltage(Battery.ptrCell);
        }
		else if(!strncmp(str,SET_LOAD,8))
        {
			pch = strtok (str," ");
			while (pch != NULL)
			{
				buffer[token] = *pch;
				token++;
				pch = strtok (NULL," ");
			}
			
			if( token == 3)
			{
                sscanf(&str1[9],"%lf", &values[0]);
				ret = set_load(values);
				ret == 1 ? printf("\nload resistor successfully set\n") : printf("\nfailed to set load resistor\n");
                lResistance = 1;
			}
			else
			{
				printf("\rPlese enter API as below\nset load 10");
			}
        }
		else if(!strncmp(str,GET_LOAD,8))
        {
            get_loadResistor();
        }
        else if(!strncmp(str,SET_RESISTOR,9))
        {
			pch = strtok (str," ");
			while (pch != NULL)
			{
				buffer[token] = *pch;
				token++;
				pch = strtok (NULL," ");
			}
			if( token == 5)
			{
				sscanf(&str1[10],"%lf %lf %lf", &values[0], &values[1], &values[2]);
            Battery.ptrCell = Cell;
				ret = set_resistor(values, Battery.ptrCell);

				if (ret == ALREADY_SET)
				{
					printf("\nresistors are already set\n");
				}else if( ret == SET )
				{
					printf("\nresistors successfully set\n");
				}
				else 
				{
					 printf("\rfailed to set resistors\n");
				}
                sResistance = 1;
			}
			else
			{
				printf("\nPlese enter API as below\nset resistor 10.2 11 12");
			}
        }
		else if(!strncmp(str,GET_RESISTOR,8))
        {
			get_seriesResistance(Battery.ptrCell);
		}
        else if(!strncmp(str,"get switch",10))
        {
            get_switch_status(Battery.ptrCell);
        }
        else if(!strncmp(str,"get current",10))
        {
            printf("\ncuurent from battery : %lf", Battery.outputCurrent);
        }  
        else 
        {
            printf("\nPlease enter proper API");
        }
	}
    return 0;
}



