#ifndef CANAL_H
#define CANAL_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <json-c/json.h>
#include "./references/queue.h"
#include "logicaBarcos.h"

#define CANAL_FILENAME "./jsons/canal_config.json"

//Calendarizadores --> scheduling algorithm
#define ROUND_ROBIN 					0
#define PRIORITY						1
#define SJF 							2
#define TREAL							3
#define FCFS 							4

//Control de flujo de barcos --> Transit control algorithm
#define EQUIDAD 						0
#define LETRERO						    1
#define TICO							2

//Direccion del canal
#define LEFT 							0
#define RIGHT 							1

struct Canal{

	int length; //lenght of the canal

	int scheduling_algorithm;
	int speed;
	int ordered_boats;
	int transit_control_algorithm;

	int W_boats; //number of boats waiting
	int W_count; //number of boats that have passed

	struct timespec start_time;
	struct timespec current_time;

	int open_side; //available side
	int letrero_time;

    //lists of boats
	struct List *left_boats_queue;
	struct List *right_boats_queue; 
	struct List *boats_crossing_list;
};

void initCanal(struct Canal *canal, const char *filename);

int priority(const void *a, const void *b);
int shortestJobFirst(const void *a, const void *b, const void *c);
int first_come_first_served(const void *a, const void *b, const void *c);

void sortCanalSide(struct Canal *canal, struct List **queue);

void insertBoatInLeft(struct Canal *canal, Boat *ant);
void insertBoatInRight(struct Canal *canal, Boat *ant);

void getBoatFromQueue(struct List **queue, struct List **boats_crossing_list, int scheduling_algorithm, int ordered_boats_list);


void letBoatPass(struct Canal *canal, int side);

void equidadAlgorithm(struct Canal *canal);
void letreroAlgorithm(struct Canal *canal);
void ticoAlgorithm(struct Canal *canal);

void checkCanalCrossed(struct Canal *canal);
void manageTraffic(struct Canal *canal);

#endif
