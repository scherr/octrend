/* voxel.h
 *
 * Maximilian Scherr
 * 2009
 */

#ifndef VOXEL_H
#define VOXEL_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "linalg.h"
#include "octree.h"
#include "scene.h"

/* Not documented yet.
 *
 */
typedef struct voxel {
    float colRed;
    float colGreen;
    float colBlue;
    float colAlpha;
    char lighting;

    // Annotations:
    unsigned char facesFlag;
    unsigned char levelFacesFlag;
    floatVector vertices[8];
    floatVector center;
    floatVector faces[6];
    float radius;
} voxel;

voxel *createVoxel(float colRed, float colGreen, float colBlue, float colAlpha, char lighting);
voxel *cloneVoxel(voxel *originalVoxel);
int deleteVoxel(voxel *voxelToDelete);
int equalVoxels(voxel *voxelA, voxel *voxelB);
int printVoxel(voxel *voxelToPrint);
int updateVoxelAnnotations(octree *sceneToLight, intVector currentCoords);
#endif
