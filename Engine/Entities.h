/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/
#pragma once

#define GRAV 0.00430200000
#define NUM_OBJECTS 8192
#define MIN_RADIUS 1.0

struct Entities
{
	float *locX;
	float *locY;
	float *mass;
	float *velX;
	float *velY;
};

struct Entity
{
	float locX;
	float locY;
	float mass;
	float velX;
	float velY;
};