#include <stdio.h>
#include <unistd.h>
#include "./logicaBarcos.h"
#include "./references/CEthreads.h"
#include <stdbool.h>
#include "lista.h"
#include "./canal.h"

CEthread_mutex_t lock; 

struct List *listaBarcos;

//Function used to reduce the internal timer of a boat
bool reducirSegundo(){
	Boat *boat_to_timer;
	int cant = 0;
	while(cant < listaBarcos->length){
		getAt(listaBarcos,cant,(void *) &boat_to_timer);
		if(boat_to_timer->type == 2){
			if(boat_to_timer->max_exec_time != 0){
				boat_to_timer->max_exec_time -= 1;
			}else{
				boat_to_timer->max_exec_time = 10000;
				return 1;
			}
		}
		cant++;
	}
	return 0;
}

//Boat thread initialization
void* thread_boat(void *arg){
	printf("Created22\n");
	Boat *boat = (Boat *) arg;
    initBoat(boat,&lock,listaBarcos);
}
//Creates an boat from the specified anthill
Boat* create_boat(int type, int priority, int duration, int route){
	Boat *boat = (Boat *) malloc(sizeof(Boat));
	boat->type          = 0;
	boat -> route = route;
	boat -> priority= priority;
	boat-> duration = duration;

	CEthread_t hilo;
	CEthread_create(&hilo,NULL,&thread_boat,(void *) boat);
	addLast(listaBarcos,&boat);
	return boat;
}

//Colition system
bool detectColition(float x1, int ancho1, int alto1, float x2, int ancho2,int alto2){
	return ((x1 <= x2+ancho2)&&(x2<= x1 +ancho1));
}

//Function to eliminate a boat.
void deleteBoat(int mouse_x){
	int cantidad = 0;
	Boat *boat_to_delete;
	while(cantidad < listaBarcos->length){
		getAt(listaBarcos,cantidad,(void *) &boat_to_delete);
		if(detectColition(boat_to_delete->pos_x, 20, 20, mouse_x,0,0)){
			boat_to_delete->stage = MAXSTAGESIZE;
			removeAt(listaBarcos,cantidad);
		}
		cantidad ++;
	}
}

int main(int argc, char *argv[]){
	struct Canal canal; //Channels
	int movimiento      = 0; 
	int contador        = 0;
	int mouse_timer     = 0;
	int cant_boat        = 0;
	bool rtos_fin       = 0;

	//Channel initialization
	initCanal(&canal,CANAL_FILENAME);
	listaBarcos = createList(sizeof(Boat *));
	Boat * boat_to_show;

	bool keepgoing = 1;	
	bool running = true;
	bool redraw = true;
	float x = 0;

	create_boat(0,3,5,0);
	create_boat(1,1,3,0);
	create_boat(2,1,3,1);
	create_boat(2,2,3,1);
	create_boat(1,3,5,0);
	create_boat(0,1,3,0);
	create_boat(0,3,5,0);

	while (running) { 
		if (redraw) { //Refreshing the screen
			if(contador == 60){
				movimiento = (movimiento+1)%4;
				contador = 0;
				CEthread_mutex_trylock(&lock);
				rtos_fin = reducirSegundo();
				CEthread_mutex_unlock(&lock);
				
			}

			contador += 1;
					
			cant_boat = 0;
			//printf("%d",listaBarcos->length);
			CEthread_mutex_trylock(&lock);
			while(cant_boat < listaBarcos->length){	//We manage as many boats as we can handle (lenght)
				getAt(listaBarcos,cant_boat,(void *) &boat_to_show);
				if(boat_to_show->stage != 10){ //We let the boat move around as long as it is not at the bridge
					boat_to_show->cond = 1;
				}else{	//Once we hit the bridge
					if(!boat_to_show->queue){	//Checking if its queued already
						boat_to_show->queue = 1; //setting it to "queued"
						if(boat_to_show->route == 0){	//The boat gets inserted in the corresponding bridge queue
							insertBoatInLeft(&canal,boat_to_show);
						}
						if(boat_to_show->route == 3){
							insertBoatInRight(&canal,boat_to_show);
						}
					}
					if(boat_to_show->isSelected){
						boat_to_show->cond = 1;
					}
					
				}
				cant_boat++;
			}
			CEthread_mutex_unlock(&lock);
		}
		if(listaBarcos->length==0){
			running=0;
		}
		//Managing the traffic of each individual channel
		manageTraffic(&canal);
		//printf("manageTraffic\n");
	}
	return 0;
}
