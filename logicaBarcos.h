#ifndef ANTLOGIC_H
#define ANTLOGIC_H

#include<stdio.h>
#include<unistd.h>
#include<cethread.h>
#include<math.h>
#include<stdbool.h>
#include"lista.h"

#define ANT_CONFIG_PAHT  "./jsons/Boattxt.json"
#define ANT_AUTO_CREATE_PAHT  "./jsons/BoatCreate.json"

#define MAXSTAGESIZE  10
typedef struct Boat{

    float speed; 
    int type;
    float pos_x;
    int dir;
    int arrival;
    int priority;
    int weight;
    double max_exec_time;
    int route;
    int cond;
    int stage;
    int route_x[MAXSTAGESIZE];  
	double find_x;
    int queue;
    int isSelected;

} Boat;

void liberarMemoria(Boat *boat,struct List *listaBoats);
bool rectBoatCol(float x1,float y1, float x2, float y2);
bool validateBoatCollision(float x1,float y1, Boat *boat, struct List *listaBoats);
void loadRoute(Boat *boat);
bool getNewPos(Boat *boat);
void BoatWhile(Boat *boat, CEthread_mutex_t *lock,struct List *listaBoats);
void initBoat(Boat *boat,CEthread_mutex_t  *lock,struct List *listaBoats);
void moveBoat(Boat *boat);
void * newBoat(void *arg);
void readBoatConfig(Boat *boat);

#endif