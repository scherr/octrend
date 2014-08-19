/* linalg.h - 3 dimensional vector space linear algebra structures and functions
 *
 * Maximilian Scherr
 * 2006
 */

#ifndef LINALG_H
#define LINALG_H

#include <math.h>

/* Not documented yet.
 *
 */
typedef struct intVector {
	int x;
	int y;
	int z;
} intVector;

/* Not documented yet.
 *
 */
typedef struct intExtendedVector {
	int x;
	int y;
	int z;
	float length;
} intExtendedVector;

/* Not documented yet.
 *
 */
typedef struct floatVector {
	float x;
	float y;
	float z;
} floatVector;

/* Not documented yet.
 *
 */
typedef struct floatExtendedVector {
	float x;
	float y;
	float z;
	float length;
} floatExtendedVector;

/* Not documented yet.
 *
 */
typedef struct doubleVector {
	double x;
	double y;
	double z;
} doubleVector;

/* Not documented yet.
 *
 */
typedef struct doubleExtendedVector {
	double x;
	double y;
	double z;
	double length;
} doubleExtendedVector;

/* Not documented yet.
 *
 */
typedef struct intMatrix {
	int x1;
	int x2;
	int x3;
	int y1;
	int y2;
	int y3;
	int z1;
	int z2;
	int z3;
} intMatrix;

/* Not documented yet.
 *
 */
typedef struct floatMatrix {
	float x1;
	float x2;
	float x3;
	float y1;
	float y2;
	float y3;
	float z1;
	float z2;
	float z3;
} floatMatrix;

/* Not documented yet.
 *
 */
typedef struct doubleMatrix {
	double x1;
	double x2;
	double x3;
	double y1;
	double y2;
	double y3;
	double z1;
	double z2;
	double z3;
} doubleMatrix;

/* Not documented yet.
 *
 */
typedef struct plane {
	float a;
	float b;
	float c;
	float d;
} plane;

intVector intVectorLiteral(int x, int y, int z);
floatVector floatVectorLiteral(float x, float y, float z);
intVector addIntVector(intVector vectorA, intVector vectorB);
intVector scalarMulIntVector(intVector vector, int scalar);
intVector subIntVector(intVector vectorA, intVector vectorB);
intVector scalarDivVector(intVector vector, int scalar);
floatVector addFloatVector(floatVector vectorA, floatVector vectorB);
floatVector scalarMulFloatVector(floatVector vector, float scalar);
floatVector subFloatVector(floatVector vectorA, floatVector vectorB);
floatVector scalarDivFloatVector(floatVector vector, float scalar);
intVector roundFloatVector(floatVector vector);
intVector floorFloatVector(floatVector vector);
intVector ceilFloatVector(floatVector vector);
float lengthFloatVector(floatVector vector);
floatVector normalizeFloatVector(floatVector vector);
floatVector crossProductFloatVector(floatVector vectorA, floatVector vectorB);
float dotProductFloatVector(floatVector vectorA, floatVector vectorB);
plane calculatePlane(floatVector pointA, floatVector pointB, floatVector pointC);
float calculateDistancePointPlane(floatVector point, plane plane);
float calculateDistancePointPoint(floatVector pointA, floatVector pointB);
floatVector rotateAboutArbitraryAxis(floatVector axisPoint, floatVector axisDir, floatVector pointToRotate, float angle);
#endif
