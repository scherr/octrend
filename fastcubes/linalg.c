/* linalg.h
 *
 * Maximilian Scherr
 * 2009
 */

#include "linalg.h"

Vector			vectorLiteral(float x, float y, float z) {
    Vector resVect = {x, y, z};

    return resVect;
}

Vector 			vectorAdd(Vector vectorA, Vector vectorB) {
    Vector resVect = {
        vectorA.x + vectorB.x,
        vectorA.y + vectorB.y,
        vectorA.z + vectorB.z
    };

    return resVect;
}

Vector 			vectorSub(Vector vectorA, Vector vectorB) {
    Vector resVect = {
        vectorA.x - vectorB.x,
        vectorA.y - vectorB.y,
        vectorA.z - vectorB.z
    };

    return resVect;
}

Vector			vectorScale(Vector vector, float scale) {
    Vector resVect = {
        vector.x * scale,
        vector.y * scale,
        vector.z * scale
    };

    return resVect;
}

Vector			vectorRound(Vector vector) {
    Vector resVect = {
        rint(vector.x),
        rint(vector.y),
        rint(vector.z)
    };

    return resVect;
}

Vector			vectorFloor(Vector vector) {
    Vector resVect = {
        floor(vector.x),
        floor(vector.y),
        floor(vector.z)
    };

    return resVect;
}

Vector			vectorCeil(Vector vector) {
    Vector resVect = {
        ceil(vector.x),
        ceil(vector.y),
        ceil(vector.z)
    };

    return resVect;
}

float			vectorLength(Vector vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector			vectorNormalize(Vector vector) {
    float length = vectorLength(vector);
    if (length == 0.0f) {
        return vector;
    } else {
        return vectorScale(vector, 1.0f / length);
    }
}

Vector			vectorCrossProduct(Vector vectorA, Vector vectorB) {
    Vector resVect = {
        vectorA.y * vectorB.z - vectorA.z * vectorB.y,
        vectorA.z * vectorB.x - vectorA.x * vectorB.z,
        vectorA.x * vectorB.y - vectorA.y * vectorB.x
    };

    return resVect;
}

float			vectorDotProduct(Vector vectorA, Vector vectorB) {
    return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
}

float			vectorDistance(Vector pointA, Vector pointB) {
    float x = pointB.x - pointA.x;
    float y = pointB.y - pointA.y;
    float z = pointB.z - pointA.z;

    return sqrt(x * x + y * y + z * z);
}

Vector			vectorRotate(Vector axisPoint, Vector axisDir, Vector pointToRotate, float angle) {
    Vector resVect;

    angle = angle * M_PI / 180.0f;
    resVect.x =
        (axisPoint.x * (axisDir.y * axisDir.y + axisDir.z * axisDir.z) +
            axisDir.x * (-axisPoint.y * axisDir.y - axisPoint.z * axisDir.z + axisDir.x * pointToRotate.x + axisDir.y * pointToRotate.y + axisDir.z * pointToRotate.z) +
            ((pointToRotate.x - axisPoint.x) * (axisDir.y * axisDir.y + axisDir.z * axisDir.z) +
            axisDir.x * (axisPoint.y * axisDir.y + axisPoint.z * axisDir.z - axisDir.y * pointToRotate.y - axisDir.z * pointToRotate.z)) * cos(angle) +
                sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z) *
                (axisPoint.y * axisDir.z - axisPoint.z * axisDir.y - axisDir.z * pointToRotate.y + axisDir.y * pointToRotate.z) * sin(angle)) /
        (axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z);
    resVect.y =
        (axisPoint.y * (axisDir.x * axisDir.x + axisDir.z * axisDir.z) +
            axisDir.y * (-axisPoint.x * axisDir.x - axisPoint.z * axisDir.z + axisDir.x * pointToRotate.x + axisDir.y * pointToRotate.y + axisDir.z * pointToRotate.z) +
            ((pointToRotate.y - axisPoint.y) * (axisDir.x * axisDir.x + axisDir.z * axisDir.z) +
            axisDir.y * (axisPoint.x * axisDir.x + axisPoint.z * axisDir.z - axisDir.x * pointToRotate.x - axisDir.z * pointToRotate.z)) * cos(angle) +
                sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z) *
                (-axisPoint.x * axisDir.z + axisPoint.z * axisDir.x + axisDir.z * pointToRotate.x - axisDir.x * pointToRotate.z) * sin(angle)) /
        (axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z);
    resVect.z =
        (axisPoint.z * (axisDir.x * axisDir.x + axisDir.y * axisDir.y) +
            axisDir.z * (-axisPoint.x * axisDir.x - axisPoint.y * axisDir.y + axisDir.x * pointToRotate.x + axisDir.y * pointToRotate.y + axisDir.z * pointToRotate.z) +
            ((pointToRotate.z - axisPoint.z) * (axisDir.x * axisDir.x + axisDir.y * axisDir.y) +
            axisDir.z * (axisPoint.x * axisDir.x + axisPoint.y * axisDir.y - axisDir.x * pointToRotate.x - axisDir.y * pointToRotate.y)) * cos(angle) +
                sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z) *
                (axisPoint.x * axisDir.y - axisPoint.y * axisDir.x - axisDir.y * pointToRotate.x + axisDir.x * pointToRotate.y) * sin(angle)) /
        (axisDir.x * axisDir.x + axisDir.y * axisDir.y + axisDir.z * axisDir.z);

    return resVect;
}

Plane			planeConstruct(Vector pointA, Vector pointB, Vector pointC) {
    Plane resPlane;
    Vector dirAB = vectorSub(pointB, pointA);
    Vector dirAC = vectorSub(pointC, pointA);
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
    Vector normal = vectorNormalize(vectorCrossProduct(dirAC, dirAB));

    resPlane.a = normal.x;
    resPlane.b = normal.y;
    resPlane.c = normal.z;
    resPlane.d = vectorDotProduct(vectorScale(normal, -1.0f), pointA);

    return resPlane;
}

float			planeDistance(Plane plane, Vector point) {
    return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d;
}
