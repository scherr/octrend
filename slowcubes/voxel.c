/* voxel.c
 *
 * Maximilian Scherr
 * 2009
 */

#include "voxel.h"

Voxel					*voxelCreate(float red, float green, float blue) {
	Voxel *resVox;
	resVox = malloc(sizeof(Voxel));
	if (resVox != NULL) {
		resVox->parent = NULL;
		resVox->childCount = 0;
		resVox->children = NULL;
		resVox->properties = 0;
		voxelSetRed(resVox, red);
		voxelSetGreen(resVox, green);
		voxelSetBlue(resVox, blue);
	}
	
	return resVox;
}

void					voxelFree(Voxel *voxel) {
	int i;
	for (i = 0; i < voxel->childCount; i++) {
		voxelFree(voxel->children[i]);
	}
	free(voxel->children);
	if (voxelGetDepth(voxel) == VOXEL_DISPLAY_VOXEL_DEPTH && voxelGetDisplayList(voxel) > 0) {
		glDeleteLists(voxelGetDisplayList(voxel), 1);
	}
	free(voxel);
}

void					voxelAddChild(Voxel *voxel, Voxel *child, unsigned char atPosition) {
	int childCount = voxel->childCount;
	int i;
	child->parent = voxel;
	voxelSetDepth(child, voxelGetDepth(voxel) + 1);
	voxelSetPosition(child, atPosition);
	if (childCount == 0) {
		voxel->children = malloc(sizeof(Voxel *));
		voxel->children[0] = child;
		voxel->childCount++;
	} else {
		for (i = 0; i < childCount; i++) {
			if (voxelGetPosition(voxel->children[i]) == atPosition) {
				voxelFree(voxel->children[i]);
				break;
			}
		}
		if (i == childCount) {
			voxel->children = realloc(voxel->children, sizeof(Voxel *) * (childCount + 1));
			voxel->children[i] = child;
			voxel->childCount++;
		}
		
		voxel->children[i] = child;
	}
}

void					voxelAdd(Voxel *root, float rootSize, Voxel *voxel, Vector atCoordinates, unsigned short atDepth) {
	short int depth = voxelGetDepth(root);
	if (depth == VOXEL_DISPLAY_VOXEL_DEPTH) {
		voxelSetDisplayUpdateRequired(root, 1);
	}
	
	float half = rootSize / 2.0f;
	unsigned char position = 0;
	if (atCoordinates.x >= half) {
		position += 4;
		atCoordinates.x -= half;
	}
	if (atCoordinates.y >= half) {
		position += 2;
		atCoordinates.y -= half;
	}
	if (atCoordinates.z >= half) {
		position += 1;
		atCoordinates.z -= half;
	}
	
	if (depth + 1 == atDepth) {
		voxelAddChild(root, voxel, position);
	} else if (depth + 1 < atDepth) {
		Voxel *child = voxelGetChild(root, position);
		if (child == NULL) {
			child = voxelCreate(0.0f, 0.0f, 0.0f);
			voxelAddChild(root, child, position);
		}
		voxelAdd(child, half, voxel, atCoordinates, atDepth);
	}
}

Voxel					*voxelGetChild(Voxel *voxel, unsigned char atPosition) {
	int i;
	for (i = 0; i < voxel->childCount; i++) {
		if (voxelGetPosition(voxel->children[i]) == atPosition) {
			return voxel->children[i];
		}
	}
	
	return NULL;
}

void					voxelPrint(Voxel *voxel, char *indentation) {
	if (voxelGetDepth(voxel) == VOXEL_DISPLAY_VOXEL_DEPTH) {
		printf("%sDisplay voxel(%d) {\n", indentation, (unsigned int) voxel);
	} else {
		printf("%sVoxel(%d) {\n", indentation, (unsigned int) voxel);
	}
	printf("%s                    parent = %d\n", indentation, (unsigned int) voxel->parent);
	if (voxelGetDepth(voxel) == VOXEL_DISPLAY_VOXEL_DEPTH) {
		printf("%s    displayUpdateRequired = %d\n", indentation, voxelIsDisplayUpdateRequired(voxel));
		printf("%s              displayList = %d\n", indentation, voxelGetDisplayList(voxel));
	}
	printf("%s                      red = %f\n", indentation, voxelGetRed(voxel));
	printf("%s                    green = %f\n", indentation, voxelGetGreen(voxel));
	printf("%s                     blue = %f\n", indentation, voxelGetBlue(voxel));
	printf("%s               childCount = %d\n", indentation, voxel->childCount);
	printf("%s                 children = {\n", indentation);
	int i;
	char *newIndentation = malloc(sizeof(char) * (strlen(indentation) + 8));
	strcpy(newIndentation, indentation);
	strcat(newIndentation,  "        ");
	for (i = 0; i < voxel->childCount; i++) {
		voxelPrint(voxel->children[i], newIndentation);
	}
	strcpy(newIndentation, indentation);
	strcat(newIndentation,  "    ");
	printf("%s}\n", newIndentation);
	free(newIndentation);
	printf("%s}\n", indentation);
}

unsigned int			voxelGetDepth(Voxel *voxel) {
	return (voxel->properties & VOXEL_PROP_DEPTH_FLAG) >> VOXEL_PROP_DEPTH_BIT;
}

void					voxelSetDepth(Voxel *voxel, unsigned short depth) {
	voxel->properties &= ~VOXEL_PROP_DEPTH_FLAG;
	voxel->properties |= (depth << VOXEL_PROP_DEPTH_BIT) & VOXEL_PROP_DEPTH_FLAG;
}

unsigned int			voxelGetPosition(Voxel *voxel) {
	return (voxel->properties & VOXEL_PROP_POSITION_FLAG) >> VOXEL_PROP_POSITION_BIT;
}

void					voxelSetPosition(Voxel *voxel, unsigned short position) {
	voxel->properties &= ~VOXEL_PROP_POSITION_FLAG;
	voxel->properties |= (position << VOXEL_PROP_POSITION_BIT) & VOXEL_PROP_POSITION_FLAG;
}

unsigned int			voxelIsDisplayUpdateRequired(Voxel *voxel) {
	return (voxel->properties & VOXEL_PROP_DISP_UPDATE_REQ_FLAG) >> VOXEL_PROP_DISP_UPDATE_REQ_BIT;
}

void					voxelSetDisplayUpdateRequired(Voxel *voxel, unsigned int displayUpdateRequired) {
	if (displayUpdateRequired) {
		displayUpdateRequired = 1;
	} else {
		displayUpdateRequired = 0;
	}
	voxel->properties &= ~VOXEL_PROP_DISP_UPDATE_REQ_FLAG;
	voxel->properties |= (displayUpdateRequired << VOXEL_PROP_DISP_UPDATE_REQ_BIT) & VOXEL_PROP_DISP_UPDATE_REQ_FLAG;
}

unsigned int			voxelGetDisplayList(Voxel *voxel) {
	return (voxel->properties & VOXEL_PROP_DISP_LIST_FLAG) >> VOXEL_PROP_DISP_LIST_BIT;
}

void					voxelSetDisplayList(Voxel *voxel, unsigned int displayList) {
	voxel->properties &= ~VOXEL_PROP_DISP_LIST_FLAG;
	voxel->properties |= (displayList << VOXEL_PROP_DISP_LIST_BIT) & VOXEL_PROP_DISP_LIST_FLAG;
}

float					voxelGetRed(Voxel *voxel) {
	return (float) voxel->red / 255.0f;
}

void					voxelSetRed(Voxel *voxel, float red) {
	if (red < 0.0f) {
		red = 0.0f;
	}
	if (red > 1.0f) {
		red = 1.0f;
	}
	voxel->red = (unsigned char) (red * 255.0f);
}

float					voxelGetGreen(Voxel *voxel) {
	return (float) voxel->green / 255.0f;
}

void					voxelSetGreen(Voxel *voxel, float green) {
	if (green < 0.0f) {
		green = 0.0f;
	}
	if (green > 1.0f) {
		green = 1.0f;
	}
	voxel->green = (unsigned char) (green * 255.0f);
}

float					voxelGetBlue(Voxel *voxel) {
	return (float) voxel->blue / 255.0f;
}

void					voxelSetBlue(Voxel *voxel, float blue) {
	if (blue < 0.0f) {
		blue = 0.0f;
	}
	if (blue > 1.0f) {
		blue = 1.0f;
	}
	voxel->blue = (unsigned char) (blue * 255.0f);
}

Voxel					*voxelGetNeighbour(Voxel *voxel, Voxel *sharedAncestor, Vector direction) {
	static char neighbourLookupTable[8][3][3][3][5] =
	{
		{
			{
				{
					{1, -1, -1, -1, 7}, // Neighbour of position 0 at (-1, -1, -1).
					{1, -1, -1,  0, 6}, // Neighbour of position 0 at (-1, -1,  0).
					{1, -1, -1,  0, 7}  // Neighbour of position 0 at (-1, -1,  1).
				},
				{
					{1, -1,  0, -1, 5}, // Neighbour of position 0 at (-1,  0, -1).
					{1, -1,  0,  0, 4}, // Neighbour of position 0 at (-1,  0,  0). Important!
					{1, -1,  0,  0, 5}  // Neighbour of position 0 at (-1,  0,  1).
				},
				{
					{1, -1,  0, -1, 7}, // Neighbour of position 0 at (-1,  1, -1).
					{1, -1,  0,  0, 6}, // Neighbour of position 0 at (-1,  1,  0).
					{1, -1,  0,  0, 7}  // Neighbour of position 0 at (-1,  1,  1).
				}
			},
			{
				{
					{1,  0, -1, -1, 3}, // Neighbour of position 0 at ( 0, -1, -1).
					{1,  0, -1,  0, 2}, // Neighbour of position 0 at ( 0, -1,  0). Important!
					{1,  0, -1,  0, 3}  // Neighbour of position 0 at ( 0, -1,  1).
				},
				{
					{1,  0,  0, -1, 1}, // Neighbour of position 0 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 0}, // Neighbour of position 0 at ( 0,  0,  0).
					{0,  0,  0,  0, 1}  // Neighbour of position 0 at ( 0,  0,  1). Important!
				},
				{
					{1,  0,  0, -1, 3}, // Neighbour of position 0 at ( 0,  1, -1).
					{0,  0,  0,  0, 2}, // Neighbour of position 0 at ( 0,  1,  0). Important!
					{0,  0,  0,  0, 3}  // Neighbour of position 0 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  0, -1, -1, 7}, // Neighbour of position 0 at ( 1, -1, -1).
					{1,  0, -1,  0, 6}, // Neighbour of position 0 at ( 1, -1,  0).
					{1,  0, -1,  0, 7}  // Neighbour of position 0 at ( 1, -1,  1).
				},
				{
					{1,  0,  0, -1, 5}, // Neighbour of position 0 at ( 1,  0, -1).
					{0,  0,  0,  0, 4}, // Neighbour of position 0 at ( 1,  0,  0). Important!
					{0,  0,  0,  0, 5}  // Neighbour of position 0 at ( 1,  0,  1).
				},
				{
					{1,  0,  0, -1, 7}, // Neighbour of position 0 at ( 1,  1, -1).
					{0,  0,  0,  0, 6}, // Neighbour of position 0 at ( 1,  1,  0).
					{0,  0,  0,  0, 7}  // Neighbour of position 0 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{1, -1, -1,  0, 6}, // Neighbour of position 1 at (-1, -1, -1).
					{1, -1, -1,  0, 7}, // Neighbour of position 1 at (-1, -1,  0).
					{1, -1, -1,  1, 6}  // Neighbour of position 1 at (-1, -1,  1).
				},
				{
					{1, -1,  0,  0, 4}, // Neighbour of position 1 at (-1,  0, -1).
					{1, -1,  0,  0, 5}, // Neighbour of position 1 at (-1,  0,  0). Important!
					{1, -1,  0,  1, 4}  // Neighbour of position 1 at (-1,  0,  1).
				},
				{
					{1, -1,  0,  0, 6}, // Neighbour of position 1 at (-1,  1, -1).
					{1, -1,  0,  0, 7}, // Neighbour of position 1 at (-1,  1,  0).
					{1, -1,  0,  1, 6}  // Neighbour of position 1 at (-1,  1,  1).
				}
			},
			{
				{
					{1,  0, -1,  0, 2}, // Neighbour of position 1 at ( 0, -1, -1).
					{1,  0, -1,  0, 3}, // Neighbour of position 1 at ( 0, -1,  0). Important!
					{1,  0, -1,  1, 2}  // Neighbour of position 1 at ( 0, -1,  1).
				},
				{
					{0,  0,  0,  0, 0}, // Neighbour of position 1 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 1}, // Neighbour of position 1 at ( 0,  0,  0).
					{1,  0,  0,  1, 0}  // Neighbour of position 1 at ( 0,  0,  1). Important!
				},
				{
					{0,  0,  0,  0, 2}, // Neighbour of position 1 at ( 0,  1, -1).
					{0,  0,  0,  0, 3}, // Neighbour of position 1 at ( 0,  1,  0). Important!
					{1,  0,  0,  1, 2}  // Neighbour of position 1 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  0, -1,  0, 6}, // Neighbour of position 1 at ( 1, -1, -1).
					{1,  0, -1,  0, 7}, // Neighbour of position 1 at ( 1, -1,  0).
					{1,  0, -1,  1, 6}  // Neighbour of position 1 at ( 1, -1,  1).
				},
				{
					{0,  0,  0,  0, 4}, // Neighbour of position 1 at ( 1,  0, -1).
					{0,  0,  0,  0, 5}, // Neighbour of position 1 at ( 1,  0,  0). Important!
					{1,  0,  0,  1, 4}  // Neighbour of position 1 at ( 1,  0,  1).
				},
				{
					{0,  0,  0,  0, 6}, // Neighbour of position 1 at ( 1,  1, -1).
					{0,  0,  0,  0, 7}, // Neighbour of position 1 at ( 1,  1,  0).
					{1,  0,  0,  1, 6}  // Neighbour of position 1 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{1, -1,  0, -1, 5}, // Neighbour of position 2 at (-1, -1, -1).
					{1, -1,  0,  0, 4}, // Neighbour of position 2 at (-1, -1,  0).
					{1, -1,  0,  0, 5}  // Neighbour of position 2 at (-1, -1,  1).
				},
				{
					{1, -1,  0, -1, 7}, // Neighbour of position 2 at (-1,  0, -1).
					{1, -1,  0,  0, 6}, // Neighbour of position 2 at (-1,  0,  0). Important!
					{1, -1,  0,  0, 7}  // Neighbour of position 2 at (-1,  0,  1).
				},
				{
					{1, -1,  1, -1, 5}, // Neighbour of position 2 at (-1,  1, -1).
					{1, -1,  1,  0, 4}, // Neighbour of position 2 at (-1,  1,  0).
					{1, -1,  1,  0, 5}  // Neighbour of position 2 at (-1,  1,  1).
				}
			},
			{
				{
					{1,  0,  0, -1, 1}, // Neighbour of position 2 at ( 0, -1, -1).
					{0,  0,  0,  0, 0}, // Neighbour of position 2 at ( 0, -1,  0). Important!
					{0,  0,  0,  0, 1}  // Neighbour of position 2 at ( 0, -1,  1).
				},
				{
					{1,  0,  0, -1, 3}, // Neighbour of position 2 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 2}, // Neighbour of position 2 at ( 0,  0,  0).
					{0,  0,  0,  0, 3}  // Neighbour of position 2 at ( 0,  0,  1). Important!
				},
				{
					{1,  0,  1, -1, 1}, // Neighbour of position 2 at ( 0,  1, -1).
					{1,  0,  1,  0, 0}, // Neighbour of position 2 at ( 0,  1,  0). Important!
					{1,  0,  1,  0, 1}  // Neighbour of position 2 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  0,  0, -1, 5}, // Neighbour of position 2 at ( 1, -1, -1).
					{0,  0,  0,  0, 4}, // Neighbour of position 2 at ( 1, -1,  0).
					{0,  0,  0,  0, 5}  // Neighbour of position 2 at ( 1, -1,  1).
				},
				{
					{1,  0,  0, -1, 7}, // Neighbour of position 2 at ( 1,  0, -1).
					{0,  0,  0,  0, 6}, // Neighbour of position 2 at ( 1,  0,  0). Important!
					{0,  0,  0,  0, 7}  // Neighbour of position 2 at ( 1,  0,  1).
				},
				{
					{1,  0,  1, -1, 5}, // Neighbour of position 2 at ( 1,  1, -1).
					{1,  0,  1,  0, 4}, // Neighbour of position 2 at ( 1,  1,  0).
					{1,  0,  1,  0, 5}  // Neighbour of position 2 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{1, -1,  0,  0, 4}, // Neighbour of position 3 at (-1, -1, -1).
					{1, -1,  0,  0, 5}, // Neighbour of position 3 at (-1, -1,  0).
					{1, -1,  0,  1, 4}  // Neighbour of position 3 at (-1, -1,  1).
				},
				{
					{1, -1,  0,  0, 6}, // Neighbour of position 3 at (-1,  0, -1).
					{1, -1,  0,  0, 7}, // Neighbour of position 3 at (-1,  0,  0). Important!
					{1, -1,  0,  1, 6}  // Neighbour of position 3 at (-1,  0,  1).
				},
				{
					{1, -1,  1,  0, 4}, // Neighbour of position 3 at (-1,  1, -1).
					{1, -1,  1,  0, 5}, // Neighbour of position 3 at (-1,  1,  0).
					{1, -1,  1,  1, 4}  // Neighbour of position 3 at (-1,  1,  1).
				}
			},
			{
				{
					{0,  0,  0,  0, 0}, // Neighbour of position 3 at ( 0, -1, -1).
					{0,  0,  0,  0, 1}, // Neighbour of position 3 at ( 0, -1,  0). Important!
					{1,  0,  0,  1, 0}  // Neighbour of position 3 at ( 0, -1,  1).
				},
				{
					{0,  0,  0,  0, 2}, // Neighbour of position 3 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 3}, // Neighbour of position 3 at ( 0,  0,  0).
					{1,  0,  0,  1, 2}  // Neighbour of position 3 at ( 0,  0,  1). Important!
				},
				{
					{1,  0,  1,  0, 0}, // Neighbour of position 3 at ( 0,  1, -1).
					{1,  0,  1,  0, 1}, // Neighbour of position 3 at ( 0,  1,  0). Important!
					{1,  0,  1,  1, 0}  // Neighbour of position 3 at ( 0,  1,  1).
				}
			},
			{
				{
					{0,  0,  0,  0, 4}, // Neighbour of position 3 at ( 1, -1, -1).
					{0,  0,  0,  0, 5}, // Neighbour of position 3 at ( 1, -1,  0).
					{1,  0,  0,  1, 4}  // Neighbour of position 3 at ( 1, -1,  1).
				},
				{
					{0,  0,  0,  0, 6}, // Neighbour of position 3 at ( 1,  0, -1).
					{0,  0,  0,  0, 7}, // Neighbour of position 3 at ( 1,  0,  0). Important!
					{1,  0,  0,  1, 6}  // Neighbour of position 3 at ( 1,  0,  1).
				},
				{
					{1,  0,  1,  0, 4}, // Neighbour of position 3 at ( 1,  1, -1).
					{1,  0,  1,  0, 5}, // Neighbour of position 3 at ( 1,  1,  0).
					{1,  0,  1,  1, 4}  // Neighbour of position 3 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{1,  0, -1, -1, 3}, // Neighbour of position 4 at (-1, -1, -1).
					{1,  0, -1,  0, 2}, // Neighbour of position 4 at (-1, -1,  0).
					{1,  0, -1,  0, 3}  // Neighbour of position 4 at (-1, -1,  1).
				},
				{
					{1,  0,  0, -1, 1}, // Neighbour of position 4 at (-1,  0, -1).
					{0,  0,  0,  0, 0}, // Neighbour of position 4 at (-1,  0,  0). Important!
					{0,  0,  0,  0, 1}  // Neighbour of position 4 at (-1,  0,  1).
				},
				{
					{1,  0,  0, -1, 3}, // Neighbour of position 4 at (-1,  1, -1).
					{0,  0,  0,  0, 2}, // Neighbour of position 4 at (-1,  1,  0).
					{0,  0,  0,  0, 3}  // Neighbour of position 4 at (-1,  1,  1).
				}
			},
			{
				{
					{1,  0, -1, -1, 7}, // Neighbour of position 4 at ( 0, -1, -1).
					{1,  0, -1,  0, 6}, // Neighbour of position 4 at ( 0, -1,  0). Important!
					{1,  0, -1,  0, 7}  // Neighbour of position 4 at ( 0, -1,  1).
				},
				{
					{1,  0,  0, -1, 5}, // Neighbour of position 4 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 4}, // Neighbour of position 4 at ( 0,  0,  0).
					{0,  0,  0,  0, 5}  // Neighbour of position 4 at ( 0,  0,  1). Important!
				},
				{
					{1,  0,  0, -1, 7}, // Neighbour of position 4 at ( 0,  1, -1).
					{0,  0,  0,  0, 6}, // Neighbour of position 4 at ( 0,  1,  0). Important!
					{0,  0,  0,  0, 7}  // Neighbour of position 4 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  1, -1, -1, 3}, // Neighbour of position 4 at ( 1, -1, -1).
					{1,  1, -1,  0, 2}, // Neighbour of position 4 at ( 1, -1,  0).
					{1,  1, -1,  0, 3}  // Neighbour of position 4 at ( 1, -1,  1).
				},
				{
					{1,  1,  0, -1, 1}, // Neighbour of position 4 at ( 1,  0, -1).
					{1,  1,  0,  0, 0}, // Neighbour of position 4 at ( 1,  0,  0). Important!
					{1,  1,  0,  0, 1}  // Neighbour of position 4 at ( 1,  0,  1).
				},
				{
					{1,  1,  0, -1, 3}, // Neighbour of position 4 at ( 1,  1, -1).
					{1,  1,  0,  0, 2}, // Neighbour of position 4 at ( 1,  1,  0).
					{1,  1,  0,  0, 3}  // Neighbour of position 4 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{1,  0, -1,  0, 2}, // Neighbour of position 5 at (-1, -1, -1).
					{1,  0, -1,  0, 3}, // Neighbour of position 5 at (-1, -1,  0).
					{1,  0, -1,  1, 2}  // Neighbour of position 5 at (-1, -1,  1).
				},
				{
					{0,  0,  0,  0, 0}, // Neighbour of position 5 at (-1,  0, -1).
					{0,  0,  0,  0, 1}, // Neighbour of position 5 at (-1,  0,  0). Important!
					{1,  0,  0,  1, 0}  // Neighbour of position 5 at (-1,  0,  1).
				},
				{
					{0,  0,  0,  0, 2}, // Neighbour of position 5 at (-1,  1, -1).
					{0,  0,  0,  0, 3}, // Neighbour of position 5 at (-1,  1,  0).
					{1,  0,  0,  1, 2}  // Neighbour of position 5 at (-1,  1,  1).
				}
			},
			{
				{
					{1,  0, -1,  0, 6}, // Neighbour of position 5 at ( 0, -1, -1).
					{1,  0, -1,  0, 7}, // Neighbour of position 5 at ( 0, -1,  0). Important!
					{1,  0, -1,  1, 6}  // Neighbour of position 5 at ( 0, -1,  1).
				},
				{
					{0,  0,  0,  0, 4}, // Neighbour of position 5 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 5}, // Neighbour of position 5 at ( 0,  0,  0).
					{1,  0,  0,  1, 4}  // Neighbour of position 5 at ( 0,  0,  1). Important!
				},
				{
					{0,  0,  0,  0, 6}, // Neighbour of position 5 at ( 0,  1, -1).
					{0,  0,  0,  0, 7}, // Neighbour of position 5 at ( 0,  1,  0). Important!
					{1,  0,  0,  1, 6}  // Neighbour of position 5 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  1, -1,  0, 2}, // Neighbour of position 5 at ( 1, -1, -1).
					{1,  1, -1,  0, 3}, // Neighbour of position 5 at ( 1, -1,  0).
					{1,  1, -1,  1, 2}  // Neighbour of position 5 at ( 1, -1,  1).
				},
				{
					{1,  1,  0,  0, 0}, // Neighbour of position 5 at ( 1,  0, -1).
					{1,  1,  0,  0, 1}, // Neighbour of position 5 at ( 1,  0,  0). Important!
					{1,  1,  0,  1, 0}  // Neighbour of position 5 at ( 1,  0,  1).
				},
				{
					{1,  1,  0,  0, 2}, // Neighbour of position 5 at ( 1,  1, -1).
					{1,  1,  0,  0, 3}, // Neighbour of position 5 at ( 1,  1,  0).
					{1,  1,  0,  1, 2}  // Neighbour of position 5 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{1,  0,  0, -1, 1}, // Neighbour of position 6 at (-1, -1, -1).
					{0,  0,  0,  0, 0}, // Neighbour of position 6 at (-1, -1,  0).
					{0,  0,  0,  0, 1}  // Neighbour of position 6 at (-1, -1,  1).
				},
				{
					{1,  0,  0, -1, 3}, // Neighbour of position 6 at (-1,  0, -1).
					{0,  0,  0,  0, 2}, // Neighbour of position 6 at (-1,  0,  0). Important!
					{0,  0,  0,  0, 3}  // Neighbour of position 6 at (-1,  0,  1).
				},
				{
					{1,  0,  1, -1, 1}, // Neighbour of position 6 at (-1,  1, -1).
					{1,  0,  1,  0, 0}, // Neighbour of position 6 at (-1,  1,  0).
					{1,  0,  1,  0, 1}  // Neighbour of position 6 at (-1,  1,  1).
				}
			},
			{
				{
					{1,  0,  0, -1, 5}, // Neighbour of position 6 at ( 0, -1, -1).
					{0,  0,  0,  0, 4}, // Neighbour of position 6 at ( 0, -1,  0). Important!
					{0,  0,  0,  0, 5}  // Neighbour of position 6 at ( 0, -1,  1).
				},
				{
					{1,  0,  0, -1, 7}, // Neighbour of position 6 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 6}, // Neighbour of position 6 at ( 0,  0,  0).
					{0,  0,  0,  0, 7}  // Neighbour of position 6 at ( 0,  0,  1). Important!
				},
				{
					{1,  0,  1, -1, 5}, // Neighbour of position 6 at ( 0,  1, -1).
					{1,  0,  1,  0, 4}, // Neighbour of position 6 at ( 0,  1,  0). Important!
					{1,  0,  1,  0, 5}  // Neighbour of position 6 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  1,  0, -1, 1}, // Neighbour of position 6 at ( 1, -1, -1).
					{1,  1,  0,  0, 0}, // Neighbour of position 6 at ( 1, -1,  0).
					{1,  1,  0,  0, 1}  // Neighbour of position 6 at ( 1, -1,  1).
				},
				{
					{1,  1,  0, -1, 3}, // Neighbour of position 6 at ( 1,  0, -1).
					{1,  1,  0,  0, 2}, // Neighbour of position 6 at ( 1,  0,  0). Important!
					{1,  1,  0,  0, 3}  // Neighbour of position 6 at ( 1,  0,  1).
				},
				{
					{1,  1,  1, -1, 1}, // Neighbour of position 6 at ( 1,  1, -1).
					{1,  1,  1,  0, 0}, // Neighbour of position 6 at ( 1,  1,  0).
					{1,  1,  1,  0, 1}  // Neighbour of position 6 at ( 1,  1,  1).
				}
			}
		},
		{
			{
				{
					{0,  0,  0,  0, 0}, // Neighbour of position 7 at (-1, -1, -1).
					{0,  0,  0,  0, 1}, // Neighbour of position 7 at (-1, -1,  0).
					{1,  0,  0,  1, 0}  // Neighbour of position 7 at (-1, -1,  1).
				},
				{
					{0,  0,  0,  0, 2}, // Neighbour of position 7 at (-1,  0, -1).
					{0,  0,  0,  0, 3}, // Neighbour of position 7 at (-1,  0,  0). Important!
					{1,  0,  0,  1, 2}  // Neighbour of position 7 at (-1,  0,  1).
				},
				{
					{1,  0,  1,  0, 0}, // Neighbour of position 7 at (-1,  1, -1).
					{1,  0,  1,  0, 1}, // Neighbour of position 7 at (-1,  1,  0).
					{1,  0,  1,  1, 0}  // Neighbour of position 7 at (-1,  1,  1).
				}
			},
			{
				{
					{0,  0,  0,  0, 4}, // Neighbour of position 7 at ( 0, -1, -1).
					{0,  0,  0,  0, 5}, // Neighbour of position 7 at ( 0, -1,  0). Important!
					{1,  0,  0,  1, 4}  // Neighbour of position 7 at ( 0, -1,  1).
				},
				{
					{0,  0,  0,  0, 6}, // Neighbour of position 7 at ( 0,  0, -1). Important!
					{0,  0,  0,  0, 7}, // Neighbour of position 7 at ( 0,  0,  0).
					{1,  0,  0,  1, 6}  // Neighbour of position 7 at ( 0,  0,  1). Important!
				},
				{
					{1,  0,  1,  0, 4}, // Neighbour of position 7 at ( 0,  1, -1).
					{1,  0,  1,  0, 5}, // Neighbour of position 7 at ( 0,  1,  0). Important!
					{1,  0,  1,  1, 4}  // Neighbour of position 7 at ( 0,  1,  1).
				}
			},
			{
				{
					{1,  1,  0,  0, 0}, // Neighbour of position 7 at ( 1, -1, -1).
					{1,  1,  0,  0, 1}, // Neighbour of position 7 at ( 1, -1,  0).
					{1,  1,  0,  1, 0}  // Neighbour of position 7 at ( 1, -1,  1).
				},
				{
					{1,  1,  0,  0, 2}, // Neighbour of position 7 at ( 1,  0, -1).
					{1,  1,  0,  0, 3}, // Neighbour of position 7 at ( 1,  0,  0). Important!
					{1,  1,  0,  1, 2}  // Neighbour of position 7 at ( 1,  0,  1).
				},
				{
					{1,  1,  1,  0, 0}, // Neighbour of position 7 at ( 1,  1, -1).
					{1,  1,  1,  0, 1}, // Neighbour of position 7 at ( 1,  1,  0).
					{1,  1,  1,  1, 0}  // Neighbour of position 7 at ( 1,  1,  1).
				}
			}
		}
	};
	
	if (voxel->parent == NULL || voxel == sharedAncestor) {
		return NULL;
	} else {
		int position = voxelGetPosition(voxel);
		char tableX = (direction.x == 0 ? 1 : (direction.x > 0 ? 2 : 0));
		char tableY = (direction.y == 0 ? 1 : (direction.y > 0 ? 2 : 0));
		char tableZ = (direction.z == 0 ? 1 : (direction.z > 0 ? 2 : 0));
		char tableLookup[5] =
		{
			neighbourLookupTable[position][tableX][tableY][tableZ][0],
			neighbourLookupTable[position][tableX][tableY][tableZ][1],
			neighbourLookupTable[position][tableX][tableY][tableZ][2],
			neighbourLookupTable[position][tableX][tableY][tableZ][3],
			neighbourLookupTable[position][tableX][tableY][tableZ][4]
		};
		if (tableLookup[0] == 0) {
			// Neighbour voxel must be a sibling.
			return voxelGetChild(voxel->parent, tableLookup[4]);
		} else {
			// Neighbour voxel cannot be a sibling.
			Voxel *parentNeighbour = voxelGetNeighbour(voxel->parent, sharedAncestor, vectorLiteral(tableLookup[1], tableLookup[2], tableLookup[3]));
			if (parentNeighbour == NULL) {
				return NULL;
			} else {
				return voxelGetChild(parentNeighbour, tableLookup[4]);
			}
		}
	}
}

static unsigned int highDetailCubeDisplayLists[8][8][8];
static unsigned int mediumDetailCubeDisplayLists[8][8];
static unsigned int lowDetailCubeDisplayLists[8];

void					voxelDraw(Voxel *voxel, Voxel *displayVoxel, Vector center, float size, unsigned short maxDrawDepth, unsigned int cubeDisplayLists[8][8][8]) {
	// Must be called in an OpenGL context.
	static Vector directions[6] =
	{
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f}
	};
	
	float depth = voxelGetDepth(voxel);
	int i;
	float half = size / 2.0f;
	if (depth < maxDrawDepth && voxel->childCount > 0) {
		if (depth == maxDrawDepth - 3) {
			glTranslatef(center.x, center.y, center.z);
		}
	
		float quarter = size / 4.0f;
		for (i = 0; i < voxel->childCount; i++) {
			switch (voxelGetPosition(voxel->children[i])) {
				case 0:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y - quarter, center.z - quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 1:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y - quarter, center.z + quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 2:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y + quarter, center.z - quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 3:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y + quarter, center.z + quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 4:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y - quarter, center.z - quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 5:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y - quarter, center.z + quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 6:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y + quarter, center.z - quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				case 7:
					voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y + quarter, center.z + quarter), half, maxDrawDepth, cubeDisplayLists);
					break;
					
				default:
					break;
			}
		}
		
		if (depth == maxDrawDepth - 3) {
			glTranslatef(-center.x, -center.y, -center.z);
		}
	} else {
		unsigned char facesFlag = 0;
		Voxel *neighbour;
		for (i = 0; i < 6; i++) {
			neighbour = voxelGetNeighbour(voxel, NULL, directions[i]);
			if (
				neighbour == NULL ||
				(
					depth == voxelGetDepth(neighbour) &&
					neighbour->childCount > 0 &&
					neighbour->childCount < 8
				)
			) {
				facesFlag |= (1 << i);
			}
		}
		
		if (facesFlag != 0) {
			// Normal approximation is necessary, since we don't save any normal information.
			int j;
			int k;
			Vector normal = {0.0f, 0.0f, 0.0f};
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					for (k = -1; k <= 1; k++) {
						if (voxelGetNeighbour(voxel, NULL, vectorLiteral((float) i, (float) j, (float) k)) == NULL) {
							normal.x += (float) i;
							normal.y += (float) j;
							normal.z += (float) k;
						}
					}
				}
			}
			normal = vectorNormalize(normal);
			
			glNormal3f(normal.x, normal.y, normal.z);	
			
			glColor3f(voxelGetRed(voxel), voxelGetGreen(voxel), voxelGetBlue(voxel));
			
			glCallList(cubeDisplayLists[voxelGetPosition(voxel->parent->parent)][voxelGetPosition(voxel->parent)][voxelGetPosition(voxel)]);
		}
	}
}

void					voxelJoinColors(Voxel *voxel, unsigned short maxDrawDepth) {
	static Vector directions[6] =
	{
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f}
	};
	
	int i;
	if (voxelGetDepth(voxel) < maxDrawDepth - 1) {
		for (i = 0; i < voxel->childCount; i++) {
			voxelJoinColors(voxel->children[i], maxDrawDepth);
		}
	}
	
	if (voxelGetDepth(voxel) >= maxDrawDepth - 2 - 1) {
		int j;
		unsigned char facesFlag;
		int count = 0;
		unsigned short red = 0;
		unsigned short green = 0;
		unsigned short blue = 0;
		for (i = 0; i < voxel->childCount; i++) {
			facesFlag = 0;
			for (j = 0; j < 6; j++) {
				if (voxelGetNeighbour(voxel->children[i], NULL, directions[j]) == NULL) {
					facesFlag |= (1 << j);
				}
			}
			
			if (facesFlag != 0) {
				count++;

				red += voxel->children[i]->red;
				green += voxel->children[i]->green;
				blue += voxel->children[i]->blue;
			}
		}
		if (count > 0) {
			voxel->red = red / count;
			voxel->green = green / count;
			voxel->blue = blue / count;
		}
	}
}

void					voxelUpdateDisplay(Voxel *voxel, Vector center, float size, unsigned short maxDrawDepth) {
	// Must be called in an OpenGL context.
	static unsigned int cubeDisplayLists[3][8][8][8];
	if (cubeDisplayLists[0][8][8][8] == 0) {	
		Vector vertices[8] = {
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f,  0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f,  0.5f},
			{ 0.5f, -0.5f, -0.5f},
			{ 0.5f, -0.5f,  0.5f},
			{ 0.5f,  0.5f, -0.5f},
			{ 0.5f,  0.5f,  0.5f}
		};
		
		unsigned char vertexSequenceA[8] = {7, 5, 3, 1, 2, 0, 6, 4};
		unsigned char vertexSequenceB[8] = {3, 2, 7, 6, 5, 4, 1, 0};
		
		int i;
		int j;
		int k;
		int l;
		int m;
		Vector center[3];
		Vector vertex;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 8; j++) {
				switch (j) {
					case 0:
						center[0].x = -2.0f * (1 << i);
						center[0].y = -2.0f * (1 << i);
						center[0].z = -2.0f * (1 << i);
						break;
						
					case 1:
						center[0].x = -2.0f * (1 << i);
						center[0].y = -2.0f * (1 << i);
						center[0].z = +2.0f * (1 << i);
						break;
						
					case 2:
						center[0].x = -2.0f * (1 << i);
						center[0].y = +2.0f * (1 << i);
						center[0].z = -2.0f * (1 << i);
						break;
						
					case 3:
						center[0].x = -2.0f * (1 << i);
						center[0].y = +2.0f * (1 << i);
						center[0].z = +2.0f * (1 << i);
						break;
						
					case 4:
						center[0].x = +2.0f * (1 << i);
						center[0].y = -2.0f * (1 << i);
						center[0].z = -2.0f * (1 << i);
						break;
						
					case 5:
						center[0].x = +2.0f * (1 << i);
						center[0].y = -2.0f * (1 << i);
						center[0].z = +2.0f * (1 << i);
						break;
						
					case 6:
						center[0].x = +2.0f * (1 << i);
						center[0].y = +2.0f * (1 << i);
						center[0].z = -2.0f * (1 << i);
						break;
						
					case 7:
						center[0].x = +2.0f * (1 << i);
						center[0].y = +2.0f * (1 << i);
						center[0].z = +2.0f * (1 << i);
						break;
				}
			
				for (k = 0; k < 8; k++) {
					center[1] = center[0];
					switch (k) {
						case 0:
							center[1].x -= 1.0f * (1 << i);
							center[1].y -= 1.0f * (1 << i);
							center[1].z -= 1.0f * (1 << i);
							break;
						
						case 1:
							center[1].x -= 1.0f * (1 << i);
							center[1].y -= 1.0f * (1 << i);
							center[1].z += 1.0f * (1 << i);
							break;
						
						case 2:
							center[1].x -= 1.0f * (1 << i);
							center[1].y += 1.0f * (1 << i);
							center[1].z -= 1.0f * (1 << i);
							break;
						
						case 3:
							center[1].x -= 1.0f * (1 << i);
							center[1].y += 1.0f * (1 << i);
							center[1].z += 1.0f * (1 << i);
							break;
						
						case 4:
							center[1].x += 1.0f * (1 << i);
							center[1].y -= 1.0f * (1 << i);
							center[1].z -= 1.0f * (1 << i);
							break;
						
						case 5:
							center[1].x += 1.0f * (1 << i);
							center[1].y -= 1.0f * (1 << i);
							center[1].z += 1.0f * (1 << i);
							break;
						
						case 6:
							center[1].x += 1.0f * (1 << i);
							center[1].y += 1.0f * (1 << i);
							center[1].z -= 1.0f * (1 << i);
							break;
						
						case 7:
							center[1].x += 1.0f * (1 << i);
							center[1].y += 1.0f * (1 << i);
							center[1].z += 1.0f * (1 << i);
							break;
					}
				
					for (l = 0; l < 8; l++) {
						center[2] = center[1];
						switch (l) {
							case 0:
								center[2].x -= 0.5f * (1 << i);
								center[2].y -= 0.5f * (1 << i);
								center[2].z -= 0.5f * (1 << i);
								break;
						
							case 1:
								center[2].x -= 0.5f * (1 << i);
								center[2].y -= 0.5f * (1 << i);
								center[2].z += 0.5f * (1 << i);
								break;
						
							case 2:
								center[2].x -= 0.5f * (1 << i);
								center[2].y += 0.5f * (1 << i);
								center[2].z -= 0.5f * (1 << i);
								break;
						
							case 3:
								center[2].x -= 0.5f * (1 << i);
								center[2].y += 0.5f * (1 << i);
								center[2].z += 0.5f * (1 << i);
								break;
						
							case 4:
								center[2].x += 0.5f * (1 << i);
								center[2].y -= 0.5f * (1 << i);
								center[2].z -= 0.5f * (1 << i);
								break;
						
							case 5:
								center[2].x += 0.5f * (1 << i);
								center[2].y -= 0.5f * (1 << i);
								center[2].z += 0.5f * (1 << i);
								break;
						
							case 6:
								center[2].x += 0.5f * (1 << i);
								center[2].y += 0.5f * (1 << i);
								center[2].z -= 0.5f * (1 << i);
								break;
						
							case 7:
								center[2].x += 0.5f * (1 << i);
								center[2].y += 0.5f * (1 << i);
								center[2].z += 0.5f * (1 << i);
								break;
						}
					
						cubeDisplayLists[i][j][k][l] = glGenLists(1);
					
						glNewList(cubeDisplayLists[i][j][k][l], GL_COMPILE);
							glBegin(GL_TRIANGLE_STRIP);
								for (m = 0; m < 8; m++) {
									vertex = vectorAdd(
										center[2],
										vectorScale(vertices[vertexSequenceA[m]], 1 << i)
									);
							
									glVertex3f(vertex.x, vertex.y, vertex.z);
								}
							glEnd();
							glBegin(GL_TRIANGLE_STRIP);
								for (m = 0; m < 8; m++) {
									vertex = vectorAdd(
										center[2],
										vectorScale(vertices[vertexSequenceB[m]], 1 << i)
									);
						
									glVertex3f(vertex.x, vertex.y, vertex.z);
								}
							glEnd();
						glEndList();
					}
				}
			}
		}
	}
	
	if (voxelGetDisplayList(voxel) == 0) {
		voxelSetDisplayList(voxel, glGenLists(3));
	}
	
	voxelSetDisplayUpdateRequired(voxel, 0);
	
	voxelJoinColors(voxel, maxDrawDepth);
	
	glNewList(voxelGetDisplayList(voxel), GL_COMPILE);
		voxelDraw(voxel, voxel, center, size, maxDrawDepth, cubeDisplayLists[0]);
	glEndList();
	glNewList(voxelGetDisplayList(voxel) + 1, GL_COMPILE);
		voxelDraw(voxel, voxel, center, size, maxDrawDepth - 1, cubeDisplayLists[1]);
	glEndList();
	glNewList(voxelGetDisplayList(voxel) + 2, GL_COMPILE);
		voxelDraw(voxel, voxel, center, size, maxDrawDepth - 2, cubeDisplayLists[2]);
	glEndList();
}
