/* linalg.c - 3 dimensional vector space linear algebra structures and functions
 *
 * Maximilian Scherr
 * 2006
 */

#include "linalg.h"

/* Not documented yet.
 *
 */
intVector intVectorLiteral(int x, int y, int z) {
    intVector resultVector = {x, y, z};
    return resultVector;
}

/* Not documented yet.
 *
 */
floatVector floatVectorLiteral(float x, float y, float z) {
    floatVector resultVector = {x, y, z};
    return resultVector;
}

/* Not documented yet.
 *
 */
intVector addIntVector(intVector vectorA, intVector vectorB) {
    // Not commented yet.
    intVector resultVector;
    resultVector.x = vectorA.x + vectorB.x;
    resultVector.y = vectorA.y + vectorB.y;
    resultVector.z = vectorA.z + vectorB.z;
    return resultVector;
}

/* Not documented yet.
 *
 */
intVector scalarMulIntVector(intVector vector, int scalar) {
    // Not commented yet.
    intVector resultVector;
    resultVector.x = vector.x * scalar;
    resultVector.y = vector.y * scalar;
    resultVector.z = vector.z * scalar;
    return resultVector;
}

/* Not documented yet.
 *
 */
intVector subIntVector(intVector vectorA, intVector vectorB) {
    // Not commented yet.
    return addIntVector(vectorA, scalarMulIntVector(vectorB, -1));
}

/* Not documented yet.
 *
 */
intVector scalarDivIntVector(intVector vector, int scalar) {
    // Not commented yet.
    intVector resultVector;
    resultVector.x = vector.x / scalar;
    resultVector.y = vector.y / scalar;
    resultVector.z = vector.z / scalar;
    return resultVector;
}

/* Not documented yet.
 *
 */
floatVector addFloatVector(floatVector vectorA, floatVector vectorB) {
    // Not commented yet.
    floatVector resultVector;
    resultVector.x = vectorA.x + vectorB.x;
    resultVector.y = vectorA.y + vectorB.y;
    resultVector.z = vectorA.z + vectorB.z;
    return resultVector;
}

/* Not documented yet.
 *
 */
floatVector scalarMulFloatVector(floatVector vector, float scalar) {
    // Not commented yet.
    floatVector resultVector;
    resultVector.x = vector.x * scalar;
    resultVector.y = vector.y * scalar;
    resultVector.z = vector.z * scalar;
    return resultVector;
}

/* Not documented yet.
 *
 */
floatVector subFloatVector(floatVector vectorA, floatVector vectorB) {
    // Not commented yet.
    return addFloatVector(vectorA, scalarMulFloatVector(vectorB, -1));
}

/* Not documented yet.
 *
 */
floatVector scalarDivFloatVector(floatVector vector, float scalar) {
    // Not commented yet.
    floatVector resultVector;
    resultVector.x = vector.x / scalar;
    resultVector.y = vector.y / scalar;
    resultVector.z = vector.z / scalar;
    return resultVector;
}

/* Not documented yet.
 *
 */
intVector roundFloatVector(floatVector vector) {
    // Not commented yet.
    intVector resultVector = {
        rint(vector.x),
        rint(vector.y),
        rint(vector.z)
    };
    return resultVector;
}

/* Not documented yet.
 *
 */
intVector floorFloatVector(floatVector vector) {
    // Not commented yet.
    intVector resultVector = {
        floor(vector.x),
        floor(vector.y),
        floor(vector.z)
    };
    return resultVector;
}

/* Not documented yet.
 *
 */
intVector ceilFloatVector(floatVector vector) {
    // Not commented yet.
    intVector resultVector = {
        ceil(vector.x),
        ceil(vector.y),
        ceil(vector.z)
    };
    return resultVector;
}

/* Not documented yet.
 *
 */
float lengthFloatVector(floatVector vector) {
    // Not commented yet.
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

/* Not documented yet.
 *
 */
floatVector normalizeFloatVector(floatVector vector) {
    // Not commented yet.
    floatVector resultVector = scalarDivFloatVector(vector, lengthFloatVector(vector));
    return resultVector;
}

/* Not documented yet.
 *
 */
floatVector crossProductFloatVector(floatVector vectorA, floatVector vectorB) {
    // Not commented yet.
    floatVector resultVector = {
        vectorA.y * vectorB.z - vectorA.z * vectorB.y,
        vectorA.z * vectorB.x - vectorA.x * vectorB.z,
        vectorA.x * vectorB.y - vectorA.y * vectorB.x
    };
    return resultVector;
}

/* Not documented yet.
 *
 */
float dotProductFloatVector(floatVector vectorA, floatVector vectorB) {
    // Not commented yet.
    return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
}

/* Not documented yet.
 *
 */
plane calculatePlane(floatVector pointA, floatVector pointB, floatVector pointC) {
    // Not commented yet.
    plane resultPlane;
    floatVector dirAB = subFloatVector(pointB, pointA);
    floatVector dirAC = subFloatVector(pointC, pointA);
    /* Cross product depends on coordinate system. Switch parameters if needed.
     *
     *  z    x  our coordinate system
     *  *   *
     *  *  *
     *  * *
     *  **
     *  o****y
     *
     */
    floatVector normal = normalizeFloatVector(crossProductFloatVector(dirAC, dirAB));

    resultPlane.a = normal.x;
    resultPlane.b = normal.y;
    resultPlane.c = normal.z;
    resultPlane.d = dotProductFloatVector(scalarMulFloatVector(normal, -1.0f), pointA);
    return resultPlane;
}

/* Not documented yet.
 *
 */
float calculateDistancePointPlane(floatVector point, plane plane) {
    // Not commented yet.
    return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d;
}

/* Not documented yet.
 *
 */
float calculateDistancePointPoint(floatVector pointA, floatVector pointB) {
    float x = pointB.x - pointA.x;
    float y = pointB.y - pointA.y;
    float z = pointB.z - pointA.z;

    return sqrt(x * x + y * y + z * z);
}

/* Not documented yet.
 *
 */
floatVector rotateAboutArbitraryAxis(floatVector axisPoint, floatVector axisDir, floatVector pointToRotate, float angle) {
    // Not commented yet.
    floatVector resultVector;

    angle = angle * M_PI / 180.0f;
    resultVector.x =
        (axisPoint.x * (axisDir.y * axisDir.y + axisDir.z * axisDir.z) +
            axisDir.x * (-axisPoint.y * axisDir.y - axisPoint.z * axisDir.z + axisDir.x * pointToRotate.x + axisDir.y * pointToRotate.y + axisDir.z * pointToRotate.z) +
            ((pointToRotate.x - axisPoint.x) * (axisDir.y * axisDir.y + axisDir.z * axisDir.z) +
            axisDir.x * (axisPoint.y * axisDir.y + axisPoint.z * axisDir.z - axisDir.y * pointToRotate.y - axisDir.z * pointToRotate.z)) * cos(angle) +
                sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z) *
                (axisPoint.y * axisDir.z - axisPoint.z * axisDir.y - axisDir.z * pointToRotate.y + axisDir.y * pointToRotate.z) * sin(angle)) /
        (axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z);
    resultVector.y =
        (axisPoint.y * (axisDir.x * axisDir.x + axisDir.z * axisDir.z) +
            axisDir.y * (-axisPoint.x * axisDir.x - axisPoint.z * axisDir.z + axisDir.x * pointToRotate.x + axisDir.y * pointToRotate.y + axisDir.z * pointToRotate.z) +
            ((pointToRotate.y - axisPoint.y) * (axisDir.x * axisDir.x + axisDir.z * axisDir.z) +
            axisDir.y * (axisPoint.x * axisDir.x + axisPoint.z * axisDir.z - axisDir.x * pointToRotate.x - axisDir.z * pointToRotate.z)) * cos(angle) +
                sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z) *
                (-axisPoint.x * axisDir.z + axisPoint.z * axisDir.x + axisDir.z * pointToRotate.x - axisDir.x * pointToRotate.z) * sin(angle)) /
        (axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z);
    resultVector.z =
        (axisPoint.z * (axisDir.x * axisDir.x + axisDir.y * axisDir.y) +
            axisDir.z * (-axisPoint.x * axisDir.x - axisPoint.y * axisDir.y + axisDir.x * pointToRotate.x + axisDir.y * pointToRotate.y + axisDir.z * pointToRotate.z) +
            ((pointToRotate.z - axisPoint.z) * (axisDir.x * axisDir.x + axisDir.y * axisDir.y) +
            axisDir.z * (axisPoint.x * axisDir.x + axisPoint.y * axisDir.y - axisDir.x * pointToRotate.x - axisDir.y * pointToRotate.y)) * cos(angle) +
                sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z) *
                (axisPoint.x * axisDir.y - axisPoint.y * axisDir.x - axisDir.y * pointToRotate.x + axisDir.x * pointToRotate.y) * sin(angle)) /
        (axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z);

    return resultVector;
}
