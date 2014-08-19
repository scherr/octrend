/* voxel.h
 *
 * Maximilian Scherr
 * 2009
 */

#ifndef VOXEL_H
#define VOXEL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <GL/gl.h>
// #include <GL/glut.h>
 
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "linalg.h"

#define VOXEL_PROP_DEPTH_BIT             0
#define VOXEL_PROP_DEPTH_FLAG            0x0000000F
#define VOXEL_PROP_POSITION_BIT          4
#define VOXEL_PROP_POSITION_FLAG         0x00000070
#define VOXEL_PROP_DISP_UPDATE_REQ_BIT   7
#define VOXEL_PROP_DISP_UPDATE_REQ_FLAG  0x00000080
#define VOXEL_PROP_DISP_LIST_BIT         9
#define VOXEL_PROP_DISP_LIST_FLAG        0xFFFFFF00

#define VOXEL_DISPLAY_VOXEL_DEPTH 2

typedef struct Voxel {
	struct Voxel	*parent;
	unsigned int	properties;
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
	unsigned char	childCount;
	struct Voxel	**children;
} Voxel;

Voxel					*voxelCreate(float red, float green, float blue);
void					voxelFree(Voxel *voxel);
void					voxelAddChild(Voxel *voxel, Voxel *child, unsigned char atPosition);
void					voxelAdd(Voxel *root, float rootSize, Voxel *voxel, Vector atCoordinates, unsigned short atDepth);
Voxel					*voxelGetChild(Voxel *voxel, unsigned char atPosition);
void					voxelPrint(Voxel *voxel, char *indentation);
unsigned int			voxelGetDepth(Voxel *voxel);
void					voxelSetDepth(Voxel *voxel, unsigned short depth);
unsigned int			voxelGetPosition(Voxel *voxel);
void					voxelSetPosition(Voxel *voxel, unsigned short position);
unsigned int			voxelIsDisplayUpdateRequired(Voxel *voxel);
void					voxelSetDisplayUpdateRequired(Voxel *voxel, unsigned int displayUpdateRequired);
unsigned int			voxelGetDisplayList(Voxel *voxel);
void					voxelSetDisplayList(Voxel *voxel, unsigned int displayList);
float					voxelGetRed(Voxel *voxel);
void					voxelSetRed(Voxel *voxel, float red);
float					voxelGetGreen(Voxel *voxel);
void					voxelSetGreen(Voxel *voxel, float green);
float					voxelGetBlue(Voxel *voxel);
void					voxelSetBlue(Voxel *voxel, float blue);
Voxel					*voxelGetNeighbour(Voxel *voxel, Voxel *sharedAncestor, Vector direction);
void					voxelDraw(Voxel *voxel, Voxel *displayVoxel, Vector center, float size, unsigned short maxDrawDepth);
void					voxelJoinColors(Voxel *voxel, unsigned short maxDrawDepth);
void					voxelUpdateDisplay(Voxel *voxel, Vector center, float size, unsigned short maxDrawDepth);
#endif
