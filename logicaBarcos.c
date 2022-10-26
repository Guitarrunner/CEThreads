#include "./logicaBarcos.h"

//Detects collision
bool rectBoatCol(float x1, float x2){
    if((x1 <= x2+19)&&(x2<= x1 +19)){
        return 1;
    }
    return 0;
}

//validates collision
bool validateBoatCollision(float x1,Boat *boat, struct List *listaBoats){
    int p = 0;
    Boat *boat_to_validate;
    while(p < listaBoats->length){
        getAt(listaBoats,p,(void *) &boat_to_validate);
         if(boat != boat_to_validate){
            if(rectBoatCol(x1,boat_to_validate->pos_x)){
                return 0;
            }
         }
        p++;
    }
    return 1;
}

//Calculates the new position for an boat
bool getNewPos(Boat *boat){
    boat->stage += 1;
    if(boat->stage == MAXSTAGESIZE){
        return 0;
    }
    boat->find_x = boat->route_x[boat->stage];
    return 1;
}

//Used to free the memory previously occupied by an boat
void liberarMemoria(Boat *boat,struct List *listaBoats){
    printf("Entra a la func\n");
    int cantidad = 0;
    Boat *boat_to_remove;
    while(cantidad < listaBoats->length){
        getAt(listaBoats,cantidad,(void *) &boat_to_remove);
        printf("eliminamos boat de la lista...\n");
        if(boat_to_remove == boat){
            removeAt(listaBoats,cantidad);
            printf("eliminamos boat de la lista...\n");
        }
        cantidad ++;
    }
}

void BoatWhile(Boat *boat,CEthread_mutex_t *lock,struct List *listaBoats){
    int x = 0;
    int new_pos = 0;
    bool moveBoat = 1;
    while(moveBoat){
        CEthread_mutex_trylock(lock);
        if(boat->stage == MAXSTAGESIZE){
            moveBoat = 0;
        }
        if(boat->cond==1){
            if(fabs(boat->pos_x -boat->find_x)<boat->speed){
                if(validateAntCollision(boat->find_x,boat,listaBoats)){
                    boat->pos_x = boat->find_x;
                    new_pos += 1;
                }
            }else if(boat->pos_x < boat->find_x){
                if(validateAntCollision(boat->pos_x + boat->speed,boat,listaBoats)){
                    boat->pos_x = boat->pos_x + boat->speed;
                }
            }else{
                if(validateAntCollision(boat->pos_x - boat->speed,boat,listaBoats)){
                    boat->pos_x = boat->pos_x - boat->speed;
                }
            }
            if(new_pos ==2){
                moveBoat = getNewPos(boat);
            }
            x++;
            new_pos = 0;
            boat->cond = 0;
        }
        CEthread_mutex_unlock(lock);
        sleep(0.5);
    }
    liberarMemoria(boat,listaBoats);
    free(boat);
    printf("Hilo eliminado.....");
    
}
//Boat init
void initBoat(Boat *boat,CEthread_mutex_t *lock,struct List *listaBoats){
    readBoatConfig(boat);
    boat->dir           = 1;
    boat->priority      = 3;
    boat->weight        = 3;
    boat->cond          = 1;
    boat->stage         = 0;
    boat->queue         = 0;
    boat->isSelected    = 0;
    loadRoute(boat);
    boat->pos_x         = boat->route_x[0];
    boat->find_x        = boat->route_x[1];
    BoatWhile(boat, lock,listaBoats);
    
}
//Parameters for each boat are provided in a config file. This function translates those parameters into attributes
void readBoatConfig(Boat *boat){
    double a_speed;
    double a_time;
    FILE *fp;
    char buffer[1024];
    struct json_object *parsed_json;
    struct json_object *speed;
    struct json_object *max_exec_time;
    fp =fopen(BOAT_CONFIG_PAHT,"r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "speed", &speed);
    json_object_object_get_ex(parsed_json, "max_exec_time", &max_exec_time);
    a_speed = json_object_get_double(speed);
    a_time = json_object_get_double(max_exec_time);
    if(boat->type == 0)
        boat->speed = a_speed;
    if(boat->type == 1)
        boat->speed = a_speed*1.2;
    if(boat->type == 2)
        boat->speed = a_speed * 1.6;
    boat->max_exec_time = a_time;
}

//Movement function. Updates the current x position of the boat.
void moveBoat(Boat *boat){
    if(boat->dir==1){
        boat->pos_x++;
    }
    printf("%f",boat->pos_x);
}

//Much new, such wow.
void* newBoat(void *arg){
};