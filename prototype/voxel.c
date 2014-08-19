/* voxel.c
 *
 * Maximilian Scherr
 * 2009
 */

#include "voxel.h"

/* Not documented yet.
 *
 */
voxel *createVoxel(float colRed, float colGreen, float colBlue, float colAlpha, char lighting) {
	// Variable declarations.
	voxel *newVoxel;

	// Memory allocation for voxel.
	newVoxel = malloc(sizeof(voxel));
	// Error handling.
	if (newVoxel == NULL) {
		printf("createVoxel <NULL>: Not enough memory available!\n");
		return NULL;
	}

	// Initialization of structure elements.
	newVoxel->colRed = colRed;
	newVoxel->colGreen = colGreen;
	newVoxel->colBlue = colBlue;
	newVoxel->colAlpha = colAlpha;
	newVoxel->lighting = lighting;

	return newVoxel;
}

/* Not documented yet.
 *
 */
voxel *cloneVoxel(voxel *originalVoxel) {
	// Variable declarations.
	voxel *clonedVoxel;

	// Memory allocation for voxel.
	clonedVoxel = malloc(sizeof(voxel));
	// Error handling.
	if (clonedVoxel == NULL) {
		printf("cloneVoxel <NULL>: Not enough memory available!\n");
		return NULL;
	}

	// Initialization of structure elements.
	clonedVoxel->colRed = originalVoxel->colRed;
	clonedVoxel->colGreen = originalVoxel->colGreen;
	clonedVoxel->colBlue = originalVoxel->colBlue;
	clonedVoxel->colAlpha = originalVoxel->colAlpha;
	clonedVoxel->lighting = originalVoxel->lighting;
	
	clonedVoxel->facesFlag = originalVoxel->facesFlag;
	clonedVoxel->levelFacesFlag = originalVoxel->levelFacesFlag;
	clonedVoxel->vertices[0] = originalVoxel->vertices[0];
	clonedVoxel->vertices[1] = originalVoxel->vertices[1];
	clonedVoxel->vertices[2] = originalVoxel->vertices[2];
	clonedVoxel->vertices[3] = originalVoxel->vertices[3];
	clonedVoxel->vertices[4] = originalVoxel->vertices[4];
	clonedVoxel->vertices[5] = originalVoxel->vertices[5];
	clonedVoxel->vertices[6] = originalVoxel->vertices[6];
	clonedVoxel->vertices[7] = originalVoxel->vertices[7];
	clonedVoxel->faces[0] = originalVoxel->faces[0];
	clonedVoxel->faces[1] = originalVoxel->faces[1];
	clonedVoxel->faces[2] = originalVoxel->faces[2];
	clonedVoxel->faces[3] = originalVoxel->faces[3];
	clonedVoxel->faces[4] = originalVoxel->faces[4];
	clonedVoxel->faces[5] = originalVoxel->faces[5];
	clonedVoxel->center = originalVoxel->center;

	return clonedVoxel;
}

/* Not documented yet.
 *
 */
int deleteVoxel(voxel *voxelToDelete) {
	// Error handling.
	if (voxelToDelete == NULL) {
		printf("deleteVoxel <1>: Argument \"node\" is NULL!\n");
		return 1;
	}
	
	// Freeing of voxel memory!
	free(voxelToDelete);

	return 0;
}

/* Not documented yet.
 *
 */
int equalVoxels(voxel *voxelA, voxel *voxelB) {
	if (voxelA->colRed == voxelB->colRed && voxelA->colGreen == voxelB->colGreen &&
		voxelA->colBlue == voxelB->colBlue && voxelA->colAlpha == voxelB->colAlpha) {
		return 1;
	} else {
		return 0;
	}
}

/* Not documented yet.
 *
 */
int printVoxel(voxel *voxelToPrint) {
	// Not commented yet.
	unsigned char i;
	if (voxelToPrint == NULL) {
		printf("printVoxel <1>: Argument \"voxelToPrint\" is NULL!\n");
		return 1;
	}
	
	printf("[R = %f, G = %f, B = %f, A = %f, Flag = %d, ", voxelToPrint->colRed, voxelToPrint->colGreen, voxelToPrint->colBlue, voxelToPrint->colAlpha, voxelToPrint->facesFlag);
	for (i = 0; i < 8; i++) {
		printf("Vertex %d = (%f, %f, %f), ", i, voxelToPrint->vertices[i].x, voxelToPrint->vertices[i].y, voxelToPrint->vertices[i].z);
	}
	printf("Center = (%f, %f, %f)]\n", voxelToPrint->center.x, voxelToPrint->center.y, voxelToPrint->center.z);
	
	return 0;
}

/* Not documented yet.
 *
 */
int updateVoxelAnnotations(octree *node, intVector currentCoords) {
	// Not commented yet.
	intVector directions[6] =
	{
		{-1,  0,  0},
		{ 1,  0,  0},
		{ 0, -1,  0},
		{ 0,  1,  0},
		{ 0,  0, -1},
		{ 0,  0,  1}
	};
	voxel *voxelToAnnotate = (voxel *) node->value;
	octree neighbour;
	unsigned int size = 1 << node->level;
	unsigned char i;
	intVector tempCoords;
	
	// The difference between facesFlag and levelFacesFlag:
	// facesFlag determines which faces have to be drawn depending on non-terminal neighbours.
	// levelFacesFlag determines which faces have to be drawn depending on same-level neighbours and their child count.
	// facesFlag helps to determine whether a node is to be drawn at all (i.e. not enclosed) and levelFacesFlag is used
	// for level of detail.
	voxelToAnnotate->facesFlag = 0;
	voxelToAnnotate->levelFacesFlag = 0;
	for (i = 0; i < 6; i++) {
		neighbour = findNeighbour(*node, directions[i]);
		// That's a horribly long condition check right there. Let's simplify or fix it later.
		if ((
				(
					neighbour.parent == NULL ||
					neighbour.parent->children[neighbour.position] == NULL
				) &&
				neighbour.terminal == 0
			) ||
			(
				neighbour.parent != NULL &&
				neighbour.parent->children[neighbour.position] != NULL &&
				isRecursivelyTerminalOctree(neighbour.parent->children[neighbour.position]) == 0
			)) {
			voxelToAnnotate->facesFlag = voxelToAnnotate->facesFlag | (1 << i);
		
			if (neighbour.level > node->level || neighbour.childCount == 0) {
				voxelToAnnotate->levelFacesFlag = voxelToAnnotate->levelFacesFlag | (1 << i);
			}
		}
	}
	
	voxelToAnnotate->vertices[0].x = (float) currentCoords.x;
	voxelToAnnotate->vertices[0].y = (float) currentCoords.y;
	voxelToAnnotate->vertices[0].z = (float) currentCoords.z;
	
	voxelToAnnotate->vertices[1].x = (float) currentCoords.x;
	voxelToAnnotate->vertices[1].y = (float) currentCoords.y;
	voxelToAnnotate->vertices[1].z = (float) (currentCoords.z + size);
	
	voxelToAnnotate->vertices[2].x = (float) currentCoords.x;
	voxelToAnnotate->vertices[2].y = (float) (currentCoords.y + size);
	voxelToAnnotate->vertices[2].z = (float) currentCoords.z;
	
	voxelToAnnotate->vertices[3].x = (float) currentCoords.x;
	voxelToAnnotate->vertices[3].y = (float) (currentCoords.y + size);
	voxelToAnnotate->vertices[3].z = (float) (currentCoords.z + size);
	
	voxelToAnnotate->vertices[4].x = (float) (currentCoords.x + size);
	voxelToAnnotate->vertices[4].y = (float) currentCoords.y;
	voxelToAnnotate->vertices[4].z = (float) currentCoords.z;
	
	voxelToAnnotate->vertices[5].x = (float) (currentCoords.x + size);
	voxelToAnnotate->vertices[5].y = (float) currentCoords.y;
	voxelToAnnotate->vertices[5].z = (float) (currentCoords.z + size);
	
	voxelToAnnotate->vertices[6].x = (float) (currentCoords.x + size);
	voxelToAnnotate->vertices[6].y = (float) (currentCoords.y + size);
	voxelToAnnotate->vertices[6].z = (float) currentCoords.z;
	
	voxelToAnnotate->vertices[7].x = (float) (currentCoords.x + size);
	voxelToAnnotate->vertices[7].y = (float) (currentCoords.y + size);
	voxelToAnnotate->vertices[7].z = (float) (currentCoords.z + size);

	voxelToAnnotate->center.x = (float) currentCoords.x + (float) size / 2;
	voxelToAnnotate->center.y = (float) currentCoords.y + (float) size / 2;
	voxelToAnnotate->center.z = (float) currentCoords.z + (float) size / 2;
	
	voxelToAnnotate->faces[0].x = (float) currentCoords.x;
	voxelToAnnotate->faces[0].y = voxelToAnnotate->center.y;
	voxelToAnnotate->faces[0].z = voxelToAnnotate->center.z;
	
	voxelToAnnotate->faces[1].x = (float) (currentCoords.x + size);
	voxelToAnnotate->faces[1].y = voxelToAnnotate->center.y;
	voxelToAnnotate->faces[1].z = voxelToAnnotate->center.z;
	
	voxelToAnnotate->faces[2].x = voxelToAnnotate->center.x;
	voxelToAnnotate->faces[2].y = (float) currentCoords.y;
	voxelToAnnotate->faces[2].z = voxelToAnnotate->center.z;
	
	voxelToAnnotate->faces[3].x = voxelToAnnotate->center.x;
	voxelToAnnotate->faces[3].y = (float) (currentCoords.y + size);
	voxelToAnnotate->faces[3].z = voxelToAnnotate->center.z;
	
	voxelToAnnotate->faces[4].x = voxelToAnnotate->center.x;
	voxelToAnnotate->faces[4].y = voxelToAnnotate->center.y;
	voxelToAnnotate->faces[4].z = (float) currentCoords.z;
	
	voxelToAnnotate->faces[5].x = voxelToAnnotate->center.x;
	voxelToAnnotate->faces[5].y = voxelToAnnotate->center.y;
	voxelToAnnotate->faces[5].z = (float) (currentCoords.z + size);
	
	voxelToAnnotate->radius = sqrt(3.0f * size * size) / 2.0f;
	
	if (node->terminal == 0) {
		for (i = 0; i < 8; i++) {
			if (node->children[i] != NULL) {
				switch (i) {
					case 0:
						tempCoords.x = currentCoords.x;
						tempCoords.y = currentCoords.y;
						tempCoords.z = currentCoords.z;
						break;
						
					case 1:
						tempCoords.x = currentCoords.x;
						tempCoords.y = currentCoords.y;
						tempCoords.z = currentCoords.z + (size >> 1);
						break;
						
					case 2:
						tempCoords.x = currentCoords.x;
						tempCoords.y = currentCoords.y + (size >> 1);
						tempCoords.z = currentCoords.z;
						break;
						
					case 3:
						tempCoords.x = currentCoords.x;
						tempCoords.y = currentCoords.y + (size >> 1);
						tempCoords.z = currentCoords.z + (size >> 1);
						break;
						
					case 4:
						tempCoords.x = currentCoords.x + (size >> 1);
						tempCoords.y = currentCoords.y;
						tempCoords.z = currentCoords.z;
						break;
						
					case 5:
						tempCoords.x = currentCoords.x + (size >> 1);
						tempCoords.y = currentCoords.y;
						tempCoords.z = currentCoords.z + (size >> 1);
						break;
						
					case 6:
						tempCoords.x = currentCoords.x + (size >> 1);
						tempCoords.y = currentCoords.y + (size >> 1);
						tempCoords.z = currentCoords.z;
						break;
						
					case 7:
						tempCoords.x = currentCoords.x + (size >> 1);
						tempCoords.y = currentCoords.y + (size >> 1);
						tempCoords.z = currentCoords.z + (size >> 1);
						break;
				}
				
				updateVoxelAnnotations(node->children[i], tempCoords);
			}
		}
	}
	
	return 0;
}
