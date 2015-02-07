/* rendering.h
 *
 * Maximilian Scherr
 * 2009
 */

#ifndef RENDERING_H
#define RENDERING_H

#include <math.h>
// #include <GL/gl.h>
// #include <GL/glut.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "octree.h"
#include "voxel.h"
#include "scene.h"

/* Not documented yet.
 *
 */
extern unsigned char facesLookupTable[64][25];

/* Not documented yet.
 *
 */
typedef struct renderSettings {
    scene scene;
    float viewWidth;
    float viewHeight;
    unsigned char maxDetailLevel;
    float detailBorders[8];
} renderSettings;

/* Not documented yet.
 *
 */
extern renderSettings currentRenderSettings;

renderSettings *initializeRendering(scene scene, unsigned char maxDetailLevel, float detailBorders[]);
void updateView(void);
void changeSize(int width, int height);
void renderScene(void);
unsigned char calculateViewDependentFacesFlag(voxel *voxelToDraw, camera *cam);
void drawVoxel(octree *node, float distance, unsigned char detailLevel, camera *cam);
unsigned char getDetailLevel(float distance, camera *cam);
void drawCheck(octree *node, camera *cam);
void drawNoCheck(octree *node, camera *cam);
octree *castRay(octree *topNode, unsigned char terminalLevel, floatVector origin, floatVector direction);
#endif
