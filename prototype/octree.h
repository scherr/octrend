/* octree.h - octree data structure with functions
 *
 * Maximilian Scherr
 * 2006
 */

#ifndef OCTREE_H
#define OCTREE_H

#include <stdlib.h>
#include <stdio.h>
#include "linalg.h"

/* octree:
 * -------
 *
 *   An octree is designed for subpartitioning 3 dimensional space. The name implies its structure.
 *   It's a tree with each node having up to 8 children (don't get confused by the binary tree examples,
 *   but octrees are hard to draw). It can be used to store volume data more efficiently than (for
 *   example) a 3 dimensional array provided that the following conditions are met:
 *
 *     - Only a small part (< half) of the space is filled with "values" (more about those later).
 *
 *     - If a tree node has 8 children with equal "values" contained, the children should be merged
 *       (they are deleted and this "value" is saved in the node if that's not already the case).
 *
 *     - Memory leaks have to be avoided by any means.
 *
 *     - Octree functions are fast.
 *
 *
 *   What follows is a description of each part of the data structure implementation.
 *
 *
 *   level:
 *   ------
 *
 *     This part indicates long the longest path to a leaf from an octree node is allowed to be at max.
 *     I'll explain it with examples using binary trees:
 *
 *         O <--------- level = 0
 *
 *         O <--------- level = 1
 *        / \
 *       O   O <------- level = 0
 *
 *           _O_ <----- level = 2
 *          /   \
 *         O     O <--- level = 1
 *        / \   / \
 *       O   O O   O <- level = 0
 *
 *           _O_ <----- level = 2
 *          /   \
 *         O     O <--- level = 1
 *        / \     \
 *       O   O     O <- level = 0
 *
 *
 *     There doesn't HAVE to be a path of that length but it's allowed to exist (and may be created):
 *
 *           _O_ <----- level = 2
 *          /   \
 *         O     O <--- level = 1
 *
 *                   <- level = 0
 *
 *           _O_ <----- level = 2
 *          /   \
 *         O     O <--- level = 1
 *                \
 *                 O <- level = 0
 *
 *
 *   position:
 *   ---------
 *
 *     This part indicates where the octree node is positioned inside of its parent node.
 *     The range is from 0 to 7. Now what does that number mean (besides the position in the children
 *     array...later)? With an octree we are subdividing a cube into 8 cubes. In this implementation
 *     you can visualize the position by examining the binary value and interpretating it as a 3
 *     dimensional vector which tells us where the node (= subcube) can be found inside the parent
 *     (= cube):
 *
 *       bin(0) = 000 => (0, 0, 0)
 *       bin(1) = 001 => (0, 0, 1)
 *       bin(2) = 010 => (0, 1, 0)
 *       bin(3) = 011 => (0, 1, 1)
 *       bin(4) = 100 => (1, 0, 0)
 *       bin(5) = 101 => (1, 0, 1)
 *       bin(6) = 110 => (1, 1, 0)
 *       bin(7) = 111 => (1, 1, 1)
 *
 *
 *     I'll illustrate this with quadtrees, the 2 dimensional equivalent of octrees:
 *
 *       +--------+--------+
 *       |        |        |
 *       | (1, 0) | (1, 1) |
 *       | <= 10  | <= 11  |
 *       | = 2    | = 3    |
 *       |        |        |
 *       +--------+--------+
 *       |        |        |
 *       | (0, 0) | (0, 1) |
 *       | <= 00  | <= 01  |
 *       | = 0    | = 1    |
 *       |        |        |
 *       +--------+--------+
 *
 *       +-------+-------+
 *       |       |       |
 *       |       |       |
 *       |       |       |
 *       |   2   |   3   |
 *       |       |       |
 *       |       |       |
 *       |       |       |
 *       +-------+---+---+
 *       |       |   |   |
 *       |       | 2 | 3 | <-- This subsubsquare is at position 3 inside of the subsquare at position 1...
 *       |       |   |   |
 *       |   0   +---+---+ <- This subsquare is at position 1 inside of the square
 *       |       |   |   |
 *       |       | 0 | 1 | <-- This subsubsquare is at position 1 inside of the subsquare at position 1...
 *       |       |   |   |
 *       +-------+---+---+
 *
 *
 *   parent:
 *   -------
 *
 *     A pointer to the parent of the octree node is stored here. In case it's is the root parent is NULL.
 *
 *
 *   childCount:
 *   -----------
 *
 *     The amount of children, the octree node has.
 *
 *
 *   children:
 *   ---------
 *
 *     This part is an array of pointers to the children of the octree node, for each position one.
 *     In case there's no child at a given position the corresponding array value is NULL.
 *     Read the section about the position part for more information on positioning.
 *
 *
 *   value:
 *   ------
 *
 *     The value part is an untyped pointer to some value assigned to the octree node, for example this
 *     could be a color value or something more complex. You decide.
 *
 *
 *	 terminal:
 *   ---------
 *
 *     This element indicates whether the octree node is terminal (preferably 1) or not (0).
 *
 */
typedef struct octree {
    unsigned char level;
    unsigned char position;
    struct octree *parent;
    unsigned char childCount;
    struct octree *children[8];
    void *value;
    unsigned char terminal;
} octree;

/* Not documented yet.
 *
 */
extern char neighbourLookupTable[8][3][3][3][5];

octree *createOctree(unsigned char level, void *value, unsigned char terminal);
octree *cloneOctree(octree *node, void *(*cloneValue)(void *));
int deleteOctree(octree *node, int (*deleteValue)(void*));
int removeOctree(octree *node, int (*deleteValue)(void*));
octree *getTopLevelNode(octree *node);
int insertOctree(octree *node, octree *nodeToBeInserted, unsigned char position, int (*deleteValue)(void *));
int addOctree(octree *node, unsigned char position, void *value, int (*deleteValue)(void *));
int expandOctree(octree *node, unsigned char levels, void *(*cloneValue)(void *));
int updateOctreeDownward(octree *node, unsigned char level, int (*joinValues)(void *[], void **));
int updateOctreeUpward(octree *node, unsigned char level, int (*joinValues)(void *[], void **));
int collapseOctree(octree *node, int (*deleteValue)(void*));
int compressOctreeDownward(octree *node, unsigned char level, int (*equalValues)(void*, void*), int (*deleteValue)(void*));
int compressOctreeUpward(octree *node, unsigned char level, int (*equalValues)(void*, void*), int (*deleteValue)(void*));
intVector getOctreeCoordinates(octree *node, unsigned char topLevel, intVector topCoordinates);
octree *findTerminalOctree(intVector coordinates, unsigned char terminalLevel, octree *topNode, intVector topCoordinates);
octree *addTerminalOctree(intVector coordinates, unsigned char level, void *value, octree *topNode, intVector topCoordinates,
        int (*deleteValue)(void *), void *(*cloneValue)(void *));
intVector getVirtualOctreeCoordinates(octree node, unsigned char topLevel, intVector topCoordinates);
octree findVirtualOctree(intVector coordinates, unsigned char terminalLevel, octree *topNode, intVector topCoordinates);
octree findNeighbour(octree currentVirtualNode, intVector direction);
int isOctreeDescendantOf(octree *maybeDescendant, octree *node);
int isOctreeAncestorOf(octree *maybeAncestor, octree *node);
int isRecursivelyTerminalOctree(octree *node);
int isNeighbourOf(octree *maybeNeighbour, octree *node);
int printOctree(octree *node);
unsigned int countOctrees(octree *node);
unsigned int countTerminalOctrees(octree *node);
int drawOctreeZPlane(octree *node, int z, unsigned char terminalLevel);
#endif
