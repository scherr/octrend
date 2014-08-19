/* render.h
 *
 * Maximilian Scherr
 * 2009
 */

#ifndef RENDER_H
#define RENDER_H

#include <math.h>
// #include <GL/gl.h>
// #include <GL/glut.h>
 
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "voxel.h"
#include "scene.h"

typedef struct RenderEnv {
	Scene scene;
	float viewWidth;
	float viewHeight;
	unsigned short maxDrawDepth;
	float highMediumDetailBorder;
	float mediumLowDetailBorder;
} RenderEnv;

RenderEnv *renderInit(Scene scene, unsigned short maxDrawDepth, float highMediumDetailBorder, float mediumLowDetailBorder);
void renderUpdate(void);
void renderReshape(int width, int height);
void renderDisplay(void);
void renderDrawVoxel(Voxel *voxel, int frustCull, Vector center, float size, unsigned short maxDrawDepth);
#endif
