/* scene.h
 *
 * Maximilian Scherr
 * 2009
 */

#ifndef SCENE_H
#define SCENE_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "linalg.h"
#include "voxel.h"

typedef struct Camera {
	Vector position;
	Vector forward;
	Vector right;
	Vector up;
	float zoom;
	float fieldOfView;
	float nearDist;
	float farDist;
	float nearHeight;
	float nearWidth;
	float farHeight;
	float farWidth;
	Plane frustrum[6];
} Camera;

typedef struct Scene {
	Voxel *root;
	float rootSize;
	Camera camera;
} Scene;

Camera			cameraLiteral(Vector position, Vector forward, Vector right, Vector up, float zoom, float fieldOfView, float nearDist, float farDist);
Scene			sceneLiteral(Voxel *root, float rootSize, Camera camera);
void			cameraUpdate(Camera *camera);
void			cameraTranslate(Camera *camera, Vector translation);
void			cameraTranslateLocally(Camera *camera, Vector translation);
void			cameraRotate(Camera *camera, Vector axis, float angle);
void			cameraRotateLocally(Camera *camera, Vector axis, float angle);
#endif
