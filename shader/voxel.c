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

void					voxelDraw(Voxel *voxel, Voxel *displayVoxel, Vector center, float size, unsigned short maxDrawDepth) {
    // Must be called in an OpenGL context.
    static unsigned char facesLookupTable[64][25] =
    {
        {0 * 4 + 1,             8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  0 = 000000
        {1 * 4 + 1,             0, 2, 3, 1,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  1 = 000001
        {1 * 4 + 1,             4, 5, 7, 6,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  2 = 000010
        {2 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  3 = 000011
        {1 * 4 + 1,             0, 1, 5, 4,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  4 = 000100
        {2 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  5 = 000101
        {2 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  6 = 000110
        {3 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  7 = 000111
        {1 * 4 + 1,             2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  8 = 001000
        {2 * 4 + 1,             0, 2, 3, 1,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, //  9 = 001001
        {2 * 4 + 1,             4, 5, 7, 6,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 10 = 001010
        {3 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 11 = 001011
        {2 * 4 + 1,             0, 1, 5, 4,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 12 = 001100
        {3 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 13 = 001101
        {3 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 14 = 001110
        {4 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    8, 8, 8, 8,    8, 8, 8, 8}, // 15 = 001111
        {1 * 4 + 1,             0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 16 = 010000
        {2 * 4 + 1,             0, 2, 3, 1,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 17 = 010001
        {2 * 4 + 1,             4, 5, 7, 6,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 18 = 010010
        {3 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 19 = 010011
        {2 * 4 + 1,             0, 1, 5, 4,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 20 = 010100
        {3 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 21 = 010101
        {3 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 22 = 010110
        {4 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8}, // 23 = 010111
        {2 * 4 + 1,             2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 24 = 011000
        {3 * 4 + 1,             0, 2, 3, 1,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 25 = 011001
        {3 * 4 + 1,             4, 5, 7, 6,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 26 = 011010
        {4 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8}, // 27 = 011011
        {3 * 4 + 1,             0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 28 = 011100
        {4 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8}, // 29 = 011101
        {4 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8,    8, 8, 8, 8}, // 30 = 011110
        {5 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    8, 8, 8, 8}, // 31 = 011111
        {1 * 4 + 1,             1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 32 = 100000
        {2 * 4 + 1,             0, 2, 3, 1,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 33 = 100001
        {2 * 4 + 1,             4, 5, 7, 6,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 34 = 100010
        {3 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 35 = 100011
        {2 * 4 + 1,             0, 1, 5, 4,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 36 = 100100
        {3 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 37 = 100101
        {3 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 38 = 100110
        {4 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 39 = 100111
        {2 * 4 + 1,             2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 40 = 101000
        {3 * 4 + 1,             0, 2, 3, 1,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 41 = 101001
        {3 * 4 + 1,             4, 5, 7, 6,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 42 = 101010
        {4 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 43 = 101011
        {3 * 4 + 1,             0, 1, 5, 4,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 44 = 101100
        {4 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 45 = 101101
        {4 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 46 = 101110
        {5 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    1, 3, 7, 5,    8, 8, 8, 8}, // 47 = 101111
        {2 * 4 + 1,             0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 48 = 110000
        {3 * 4 + 1,             0, 2, 3, 1,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 49 = 110001
        {3 * 4 + 1,             4, 5, 7, 6,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 50 = 110010
        {4 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 51 = 110011
        {3 * 4 + 1,             0, 1, 5, 4,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 52 = 110100
        {4 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 53 = 110101
        {4 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 54 = 110110
        {5 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8}, // 55 = 110111
        {3 * 4 + 1,             2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8,    8, 8, 8, 8}, // 56 = 111000
        {4 * 4 + 1,             0, 2, 3, 1,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 57 = 111001
        {4 * 4 + 1,             4, 5, 7, 6,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 58 = 111010
        {5 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8}, // 59 = 111011
        {4 * 4 + 1,             0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8,    8, 8, 8, 8}, // 60 = 111100
        {5 * 4 + 1,             0, 2, 3, 1,    0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8}, // 61 = 111101
        {5 * 4 + 1,             4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5,    8, 8, 8, 8}, // 62 = 111110
        {6 * 4 + 1,             0, 2, 3, 1,    4, 5, 7, 6,    0, 1, 5, 4,    2, 6, 7, 3,    0, 4, 6, 2,    1, 3, 7, 5}  // 63 = 111111
    };
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
    float half = size / 2.0f;
    if (voxelGetDepth(voxel) < maxDrawDepth && voxel->childCount > 0) {
        float quarter = size / 4.0f;
        for (i = 0; i < voxel->childCount; i++) {
            switch (voxelGetPosition(voxel->children[i])) {
                case 0:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y - quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 1:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y - quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                case 2:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y + quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 3:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x - quarter, center.y + quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                case 4:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y - quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 5:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y - quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                case 6:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y + quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 7:
                    voxelDraw(voxel->children[i], displayVoxel, vectorLiteral(center.x + quarter, center.y + quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                default:
                    break;
            }
        }
    } else {
        unsigned char facesFlag = 0;
        Voxel *neighbour;
        for (i = 0; i < 6; i++) {
            neighbour = voxelGetNeighbour(voxel, NULL, directions[i]);
            if (
                neighbour == NULL ||
                (
                    voxelGetDepth(voxel) == voxelGetDepth(neighbour) &&
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

            glVertex3f(center.x, center.y, center.z);

            /*
            Vector vertices[8] = {
                {center.x - half, center.y - half, center.z - half},
                {center.x - half, center.y - half, center.z + half},
                {center.x - half, center.y + half, center.z - half},
                {center.x - half, center.y + half, center.z + half},
                {center.x + half, center.y - half, center.z - half},
                {center.x + half, center.y - half, center.z + half},
                {center.x + half, center.y + half, center.z - half},
                {center.x + half, center.y + half, center.z + half}
            };

            Vector vertex;
            for (i = 1; i < facesLookupTable[facesFlag][0]; i++) {
                vertex = vertices[facesLookupTable[facesFlag][i]];

                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
            */
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

    if (voxelGetDepth(voxel) >= maxDrawDepth - VOXEL_LOW_DETAIL_MAX_DRAW_DEPTH_DISTANCE - 1) {
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
    if (voxelGetDisplayList(voxel) == 0) {
        voxelSetDisplayList(voxel, glGenLists(3));
    }
    voxelSetDisplayUpdateRequired(voxel, 0);

    voxelJoinColors(voxel, maxDrawDepth);

    glNewList(voxelGetDisplayList(voxel), GL_COMPILE);
        glBegin(GL_POINTS);
            voxelDraw(voxel, voxel, center, size, maxDrawDepth);
        glEnd();
    glEndList();
    glNewList(voxelGetDisplayList(voxel) + 1, GL_COMPILE);
        glBegin(GL_POINTS);
            voxelDraw(voxel, voxel, center, size, maxDrawDepth - VOXEL_MEDIUM_DETAIL_MAX_DRAW_DEPTH_DISTANCE);
        glEnd();
    glEndList();
    glNewList(voxelGetDisplayList(voxel) + 2, GL_COMPILE);
        glBegin(GL_POINTS);
            voxelDraw(voxel, voxel, center, size, maxDrawDepth - VOXEL_LOW_DETAIL_MAX_DRAW_DEPTH_DISTANCE);
        glEnd();
    glEndList();
}
