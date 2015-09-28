#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "board.h"

Position* Position_create(uint8_t x, uint8_t y, Position* next) {
	Position* position = malloc(sizeof(Position));
	position->x = x;
	position->y = y;
	position->size = next == NULL ? 1 : next->size+1;
	position->next = next;
	return position;
}

void Position_destroy(Position* position) {
	while(position != NULL) {
		Position* next = position->next;
		free(position);
		position = next;
	}
}

void Position_print(Position* position) {
	while(position != NULL) {
		printf("(%d, %d)", position->x, position->y);
		position = position->next;
		
		if(position == NULL) break;
		printf(", ");
	}
	
	printf("\n");
}