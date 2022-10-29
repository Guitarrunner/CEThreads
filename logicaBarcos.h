#ifndef ANTLOGIC_H
#define ANTLOGIC_H

#include<stdio.h>
#include<unistd.h>
#include "./references/CEthreads.h"
#include<math.h>
#include<stdbool.h>
#include"lista.h"

#define BOAT_CONFIG_PATH  "./jsons/boats_config.json"
#define BOAT_AUTO_CREATE_PAHT  "./jsons/BoatCreate.json"

#define CANAL_FILENAME "./jsons/canal_config.json"

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
bool rectBoatCol(float x1, float x2);
bool validateBoatCollision(float x1, Boat *boat, struct List *listaBoats);
bool getNewPos(Boat *boat);
void BoatWhile(Boat *boat, CEthread_mutex_t *lock,struct List *listaBoats);
void initBoat(Boat *boat,CEthread_mutex_t  *lock,struct List *listaBoats);
void moveBoat(Boat *boat);
void * newBoat(void *arg);
void readBoatConfig(Boat *boat, char *filename);

#endif
