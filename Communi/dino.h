#ifndef __DINO_H
#define __DINO_H

#include <stdint.h>

struct Object_Position{
	uint8_t minX,minY,maxX,maxY;
};

void Dino_Tick(void);
int DinoGame_Animation(void);
void DinoGame_Pos_Init(void);
int isColliding(struct Object_Position *a, struct Object_Position *b);

extern struct Object_Position barrier;
extern struct Object_Position dino;
extern uint8_t dino_jump_flag;
#endif
