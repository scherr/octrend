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
#include "octree.h"
#include "voxel.h"

/* Not documented yet.
 *
 */
typedef struct camera {
    floatVector position;
    floatVector direction;
    floatVector up;
    floatVector right;
    float zoom;
    float fieldOfView;
    float nearDist;
    float farDist;
    float nearHeight;
    float nearWidth;
    float farHeight;
    float farWidth;
    char ortho;

    // Annotations:
    plane frustrum[6];
} camera;

/* Not documented yet.
 *
 */
typedef struct light {
    floatVector position;
    float radius;
    float intDecr;
    float colRed;
    float colGreen;
    float colBlue;
    char trace;
    float traceStart;
} light;

/* Not documented yet.
 *
 */
typedef struct scene {
    octree *octreeRoot;
    camera *camera;
    float ambientLightColRed;
    float ambientLightColGreen;
    float ambientLightColBlue;
    char lightCount;
    light *lights[32];
} scene;

scene createScene(octree *octreeRoot, camera *camera, float ambientLightColRed, float ambientLightColGreen, float ambientLightColBlue);
camera *createCamera(float fieldOfView, float nearDist, float farDist, char ortho);
int updateCameraAnnotations(camera *cam);
void translateCamera(camera *cam, floatVector translation);
void translateCameraLocally(camera *cam, floatVector translation);
void rotateCamera(camera *cam, floatVector axis, float angle);
void rotateCameraLocally(camera *cam, floatVector axis, float angle);
light *createLight(floatVector position, float radius, float intDecr, float colRed, float colGreen, float colBlue, char trace, float traceStart);
int addLight(scene *scene, light *lightToAdd);
int removeLight(scene *scene, light *lightToRemove);
#endif
