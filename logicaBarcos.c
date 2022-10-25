#include "../include/logicaBarcos.h"

//Detects collision
bool rectBoatCol(float x1,float x2){
    if((x1 <= x2+19)&&(x2<= x1 +19)){
        return 1;
    }
    return 0;
}

//Validates the collision
bool validateBoatCollision(float x1, Boat *boat, struct List *listaBoats){
    int p = 0;
    Boat *boat_to_validate;
    while(p < listaBoats->length){
        getAt(listaBoats,p,(void *) &boat_to_validate);
         if(boat != boat_to_validate){
            if(rectBoatCol(x1,boat_to_validate->pos_x,boat_to_validate->pos_x)){
                return 0;
            }
         }
        p++;
    }
    return 1;
}

//Calculates a new position for the boat
bool getNewPos(Boat *boat){
    boat->stage += 1;
    if(boat->stage == MAXSTAGESIZE){
        return 0;
    }
    return 1;
}

//Used to free the memory previously occupied by an boat.
void liberarMemoria(Boat *boat,struct List *listaBoats){
    printf("Entra a la func\n");
    int cantidad = 0;
    Boat *boat_to_remove;
    while(cantidad < listaBoats->length){
        getAt(listaBoats,cantidad,(void *) &boat_to_remove);
        
        if(boat_to_remove == boat){
            removeAt(listaBoats,cantidad);
        }
        cantidad ++;
    }
}

void BoatWhile(Boat *boat,pthread_mutex_t *lock,struct List *listaBoats){
    int x = 0;
    bool moveBoat = 1;
    
    while(moveBoat){
        pthread_mutex_lock(lock);
        if(boat->stage == MAXSTAGESIZE){
            moveBoat = 0;
        }

        if(boat->cond==1){
            if(boat->pos_x < boat->find_x){
                if(validateBoatCollision(boat->pos_x + boat->speed,boat->pos_y,boat,listaBoats)){
                    boat->pos_x = boat->pos_x + boat->speed;
                }
        }
        pthread_mutex_unlock(lock);
        sleep(0.5);
    }
    liberarMemoria(boat,listaBoats);
    free(boat);
    printf("Hilo eliminado.....");
}
