#include "./canal.h"

//Init method
void initCanal(struct Canal *canal, const char *filename){
	FILE *fp;
	char buffer[1024];
	struct json_object *parsed_json;
	struct json_object *length;
	struct json_object *speed;
	struct json_object *transit_control_algorithm;
	struct json_object *scheduling_algorithm;
	struct json_object *ordered_boats;

	struct json_object *w;
	struct json_object *letrero_time;

	fp = fopen(filename, "r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);
	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "length", &length);
	json_object_object_get_ex(parsed_json, "speed", &speed);
	json_object_object_get_ex(parsed_json, "transit_control_algorithm", &transit_control_algorithm);
	json_object_object_get_ex(parsed_json, "scheduling_algorithm", &scheduling_algorithm);

	json_object_object_get_ex(parsed_json, "ordered_boats_left", &ordered_boats);

	json_object_object_get_ex(parsed_json, "W", &w);
	json_object_object_get_ex(parsed_json, "letrero_time", &letrero_time);

	canal->length 						= json_object_get_int(length);
	canal->speed 						= json_object_get_int(speed);

	canal->transit_control_algorithm	= json_object_get_int(transit_control_algorithm);
	canal->scheduling_algorithm 		= json_object_get_int(scheduling_algorithm);

	canal->ordered_boats 	= json_object_get_int(ordered_boats);

	canal->W_boats 						= json_object_get_int(w);
	canal->letrero_time					= json_object_get_int(letrero_time);

	canal->left_boats_queue		= createList(sizeof(Boat *));
	canal->right_boats_queue		= createList(sizeof(Boat *));
	canal->boats_crossing_list	= createList(sizeof(Boat *));

	canal->open_side	= LEFT;
	canal->W_count 	= 0;

	if (canal->transit_control_algorithm == LETRERO)
		clock_gettime(CLOCK_REALTIME, &canal->start_time);
}

//Scheduling algorithm. It sorts based on priority
int priority(const void *a, const void *b){
	int boat1_priority = ((Boat *)a)->priority;
	int boat2_priority = ((Boat *)b)->priority;
	return boat1_priority > boat2_priority;
}

//Scheduling algorithm. It sorts based on the speed(time) of each boat
int shortestJobFirst(const void *a, const void *b, const void *c){
	int canal_length = ((struct Canal *)c)->length;
	int boat1_speed = ((Boat *)a)->speed;
	int boat2_speed = ((Boat *)b)->speed;
	return !(canal_length / boat1_speed > canal_length / boat2_speed);
}

//Aux function to sort each channel based on the scheduling algorithm 
void sortCanalSide(struct Canal *canal, struct List **queue){
	switch (canal->scheduling_algorithm){
		case PRIORITY:
			priority_sort(*queue, canal->ordered_boats, priority);
			break;
    /*    case FCFS:
			fcfs_sort(*queue, canal->ordered_boats, first_come_first_served);
			break;*/
		case SJF:
			sjf_sort(*queue, canal->ordered_boats, shortestJobFirst, (const void *)canal);
			break;
		default:
			break;
	}
}

//Inserts an boat in the left side of the channel's queue
void insertBoatInLeft(struct Canal *canal, Boat *boat){
	boat->pos_x += 30;
	addLast(canal->left_boats_queue, &boat);
	if ((canal->scheduling_algorithm == PRIORITY || canal->scheduling_algorithm == SJF) && 
		 canal->left_boats_queue->length <= canal->ordered_boats)
		sortCanalSide(canal, &canal->left_boats_queue);
}

//Inserts an boat in the right side of the channel's queue
void insertBoatInRight(struct Canal *canal, Boat *boat){
	boat->pos_x -= 30;
	addLast(canal->right_boats_queue, &boat);
	if ((canal->scheduling_algorithm == PRIORITY || canal->scheduling_algorithm == SJF) && 
		canal->right_boats_queue->length <= canal->ordered_boats)
		sortCanalSide(canal, &canal->right_boats_queue);
}

//Takes an boat out of the channel's queue based on the scheduling algorithm.
void getBoatFromQueue(struct List **queue, struct List **boats_crossing_list, int scheduling_algorithm, int ordered_boats){
	if (!isListEmpty(*queue)){
		Boat *boat_tmp;
		if (scheduling_algorithm == ROUND_ROBIN || scheduling_algorithm == PRIORITY || //In the case of these 4 algorithms, the queue was sorted previously
			scheduling_algorithm == SJF         || scheduling_algorithm == FCFS){		//accordingly
			getFirst(*queue, (void *)&boat_tmp);
			removeFirst(*queue);
		}
		else if (scheduling_algorithm == TREAL){
			int index;
			if ((*queue)->length > ordered_boats)
				index = rand() % ordered_boats;
			else
				index = rand() % (*queue)->length;
			getAt(*queue, index, (void *)&boat_tmp);
			removeAt(*queue, index);
		}
	}
}
//Allos an boat to be removed from the queue.
void letBoatPass(struct Canal *canal, int side){
	switch (side){
		case LEFT:
			getBoatFromQueue(&canal->left_boats_queue, &canal->boats_crossing_list, canal->scheduling_algorithm, canal->ordered_boats);
			break;
		case RIGHT:
			getBoatFromQueue(&canal->right_boats_queue, &canal->boats_crossing_list, canal->scheduling_algorithm, canal->ordered_boats);
			break;
		default:
			break;
	}
}

//Flow-control algorithm
void equidadAlgorithm(struct Canal *canal){
	//If there are boats waiting to cross the canal
	if (!isListEmpty(canal->left_boats_queue) || !isListEmpty(canal->right_boats_queue)){
		//If there are no boats crossing
		if (isListEmpty(canal->boats_crossing_list)){
			//If there are no boats waiting in the left
			if (isListEmpty(canal->left_boats_queue))
				canal->open_side = RIGHT;
			//If there are no boats waiting in the right
			else if (isListEmpty(canal->right_boats_queue))
				canal->open_side = LEFT;
			//There are boats waiting on both sides
			else{
				//If the number of boats that have passed is zero and on both sides the number of boats waiting is less than W
				if (canal->W_count == 0 && 
					canal->left_boats_queue->length < canal->W_boats && canal->right_boats_queue->length < canal->W_boats)
						//Select the side where there are more boats waiting
						canal->open_side = (canal->left_boats_queue->length >= canal->right_boats_queue->length) ? LEFT : RIGHT;
				else if (canal->W_count == canal->W_boats){
					canal->open_side = !canal->open_side;
					canal->W_count = 0;
				}
			}
		}
	}
}

//Flow-control algorithm. Alternates between sides every "n" seconds. The time is specified in the channel's config file.
void letreroAlgorithm(struct Canal *canal){
	clock_gettime(CLOCK_REALTIME, &(canal->current_time));
	//If there are no boats crossing
	if (isListEmpty(canal->boats_crossing_list)){
		//If time's up on the left side
		if (canal->open_side == LEFT && (canal->current_time.tv_sec - canal->start_time.tv_sec) >= canal->letrero_time){
			canal->open_side = RIGHT;
			clock_gettime(CLOCK_REALTIME, &canal->start_time);
		}

		//If time's up on the right side
		else if (canal->open_side == RIGHT && (canal->current_time.tv_sec - canal->start_time.tv_sec) >= canal->letrero_time){
			canal->open_side = LEFT;
			clock_gettime(CLOCK_REALTIME, &canal->start_time);
		}
	}
}

//This is controlled chaos. Also it controls the flow of boats
void ticoAlgorithm(struct Canal *canal){
	
	//If there are boats waiting to cross the canal
	if (!isListEmpty(canal->left_boats_queue) || !isListEmpty(canal->right_boats_queue)){
		//If there are no boats crossing
		if (isListEmpty(canal->boats_crossing_list)){	
			if (canal->open_side == LEFT && isListEmpty(canal->left_boats_queue)){
				canal->open_side = RIGHT;
			}else if (canal->open_side == RIGHT && isListEmpty(canal->right_boats_queue))
				canal->open_side = LEFT;
		}
	}
}

//Checks if the boat is done crossing the channel (completed the execution)
void checkCanalCrossed(struct Canal *canal){
	Boat *boat_tmp;
	int i = 0;
	printf("Total of boats crossing: %d\n", canal->boats_crossing_list->length);
	struct List *boats_crossing_list_local = canal->boats_crossing_list;
	for (struct lNode *node = canal->boats_crossing_list->head; node != NULL; node = node->next){
		getAt(canal->boats_crossing_list, i, (void *)&boat_tmp);
		if (boat_tmp->stage >= 6){
			printf("This is the type of boat: %d\n",boat_tmp->type);
			printf("Largo botes de borrar la hormiga: %d\n", canal->boats_crossing_list->length);
			removeAt(canal->boats_crossing_list, i);
			i--;
		}
		i++;
	}
}

//Chooses the appropiate flow-control algorithm
void manageTraffic(struct Canal *canal){
	switch (canal->transit_control_algorithm){
		case EQUIDAD:
			equidadAlgorithm(canal);
			break;
		case LETRERO:
			letreroAlgorithm(canal);
			break;
		case TICO:
			ticoAlgorithm(canal);
			break;
		default:
			break;
	}		
	if (!isListEmpty(canal->boats_crossing_list))
		checkCanalCrossed(canal);
}

//Aux function to sort each channel based on the scheduling algorithm 
void sortCanalSide(struct Canal *canal, struct List **queue){
	switch (canal->scheduling_algorithm){
		case PRIORITY:
			priority_sort(*queue, canal->ordered_boats, priority);
			break;
    /*    case FCFS:
			fcfs_sort(*queue, canal->ordered_boats, first_come_first_served);
			break;*/
		case SJF:
			sjf_sort(*queue, canal->ordered_boats, shortestJobFirst, (const void *)canal);
			break;
		default:
			break;
	}
}

//Inserts an boat in the left side of the channel's queue
void insertBoatInLeft(struct Canal *canal, Boat *boat){
	boat->pos_x += 30;
	addLast(canal->left_boats_queue, &boat);
	if ((canal->scheduling_algorithm == PRIORITY || canal->scheduling_algorithm == SJF) && 
		 canal->left_boats_queue->length <= canal->ordered_boats)
		sortCanalSide(canal, &canal->left_boats_queue);
}

//Inserts an boat in the right side of the channel's queue
void insertBoatInRight(struct Canal *canal, Boat *boat){
	boat->pos_x -= 30;
	addLast(canal->right_boats_queue, &boat);
	if ((canal->scheduling_algorithm == PRIORITY || canal->scheduling_algorithm == SJF) && 
		canal->right_boats_queue->length <= canal->ordered_boats)
		sortCanalSide(canal, &canal->right_boats_queue);
}

//Takes an boat out of the channel's queue based on the scheduling algorithm.
void getBoatFromQueue(struct List **queue, struct List **boats_crossing_list, int scheduling_algorithm, int ordered_boats){
	if (!isListEmpty(*queue)){
		Boat *boat_tmp;
		if (scheduling_algorithm == ROUND_ROBIN || scheduling_algorithm == PRIORITY || //In the case of these 4 algorithms, the queue was sorted previously
			scheduling_algorithm == SJF         || scheduling_algorithm == FCFS){		//accordingly
			getFirst(*queue, (void *)&boat_tmp);
			removeFirst(*queue);
		}
		else if (scheduling_algorithm == TREAL){
			int index;
			if ((*queue)->length > ordered_boats)
				index = rand() % ordered_boats;
			else
				index = rand() % (*queue)->length;
			getAt(*queue, index, (void *)&boat_tmp);
			removeAt(*queue, index);
		}
	}
}
//Allos an boat to be removed from the queue.
void letBoatPass(struct Canal *canal, int side){
	switch (side){
		case LEFT:
			getBoatFromQueue(&canal->left_boats_queue, &canal->boats_crossing_list, canal->scheduling_algorithm, canal->ordered_boats);
			break;
		case RIGHT:
			getBoatFromQueue(&canal->right_boats_queue, &canal->boats_crossing_list, canal->scheduling_algorithm, canal->ordered_boats);
			break;
		default:
			break;
	}
}
