/* linalg.c
 *
 * Maximilian Scherr
 * 2009
 */

#ifndef LINALG_H
#define LINALG_H

#include <math.h>

typedef struct Vector {
	float x;
	float y;
	float z;
} Vector;

Vector			vectorLiteral(float x, float y, float z);
Vector 			vectorAdd(Vector vectorA, Vector vectorB);
Vector 			vectorSub(Vector vectorA, Vector vectorB);
Vector			vectorScale(Vector vector, float scale);
Vector			vectorRound(Vector vector);
Vector			vectorFloor(Vector vector);
Vector			vectorCeil(Vector vector);
float			vectorLength(Vector vector);
Vector			vectorNormalize(Vector vector);
Vector			vectorCrossProduct(Vector vectorA, Vector vectorB);
float			vectorDotProduct(Vector vectorA, Vector vectorB);
float			vectorDistance(Vector pointA, Vector pointB);
Vector			vectorRotate(Vector axisPoint, Vector axisDir, Vector pointToRotate, float angle);

typedef struct Plane {
	float a;
	float b;
	float c;
	float d;
} Plane;

Plane			planeConstruct(Vector pointA, Vector pointB, Vector pointC);
float			planeDistance(Plane plane, Vector point);

#endif
