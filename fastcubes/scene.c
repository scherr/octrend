/* scene.c
 *
 * Maximilian Scherr
 * 2009
 */

#include "scene.h"

Camera			cameraLiteral(Vector position, Vector forward, Vector right, Vector up, float zoom, float fieldOfView, float nearDist, float farDist) {
    Camera resCam = {
        position,
        forward,
        right,
        up,
        zoom,
        fieldOfView,
        nearDist,
        farDist,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        {
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f}
        }
    };
    return resCam;
}

Scene			sceneLiteral(Voxel *root, float rootSize, Camera camera) {
    Scene resSce = {root, rootSize, camera};
    return resSce;
}

void			cameraUpdate(Camera *camera) {
    Vector    up = camera->up;
    Vector  down = vectorScale(   up, -1.0f);
    Vector right = camera->right;
    Vector  left = vectorScale(right, -1.0f);

    Vector        nearLeft = vectorAdd(vectorAdd(camera->position, vectorScale(camera->forward, camera->nearDist)), vectorScale(left , camera->nearWidth / 2.0f));
    Vector       nearRight = vectorAdd(vectorAdd(camera->position, vectorScale(camera->forward, camera->nearDist)), vectorScale(right, camera->nearWidth / 2.0f));

    Vector  nearBottomLeft = vectorAdd( nearLeft, vectorScale(down, camera->nearHeight / 2.0f));
    Vector nearBottomRight = vectorAdd(nearRight, vectorScale(down, camera->nearHeight / 2.0f));
    Vector    nearTopRight = vectorAdd(nearRight, vectorScale(up  , camera->nearHeight / 2.0f));
    Vector     nearTopLeft = vectorAdd( nearLeft, vectorScale(up  , camera->nearHeight / 2.0f));

    Vector        farLeft = vectorAdd(vectorAdd(camera->position, vectorScale(camera->forward, camera->farDist)), vectorScale(left , camera->farWidth / 2.0f));
    Vector       farRight = vectorAdd(vectorAdd(camera->position, vectorScale(camera->forward, camera->farDist)), vectorScale(right, camera->farWidth / 2.0f));

    Vector  farBottomLeft = vectorAdd(farLeft , vectorScale(down, camera->farHeight / 2.0f));
    Vector farBottomRight = vectorAdd(farRight, vectorScale(down, camera->farHeight / 2.0f));
    Vector    farTopRight = vectorAdd(farRight, vectorScale(up  , camera->farHeight / 2.0f));
    Vector     farTopLeft = vectorAdd(farLeft , vectorScale(up  , camera->farHeight / 2.0f));

    /*
    printf(" nearBottomLeft %f %f %f\n", nearBottomLeft.x, nearBottomLeft.y, nearBottomLeft.z);
    printf("nearBottomRight %f %f %f\n", nearBottomRight.x, nearBottomRight.y, nearBottomRight.z);
    printf("   nearTopRight %f %f %f\n", nearTopRight.x, nearTopRight.y, nearTopRight.z);
    printf("    nearTopLeft %f %f %f\n", nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);

    printf(" farBottomLeft %f %f %f\n", farBottomLeft.x, farBottomLeft.y, farBottomLeft.z);
    printf("farBottomRight %f %f %f\n", farBottomRight.x, farBottomRight.y, farBottomRight.z);
    printf("   farTopRight %f %f %f\n", farTopRight.x, farTopRight.y, farTopRight.z);
    printf("    farTopLeft %f %f %f\n", farTopLeft.x, farTopLeft.y, farTopLeft.z);
    */

    camera->frustrum[0] = planeConstruct(nearBottomLeft , nearTopLeft    , nearTopRight);   // Near.
    camera->frustrum[1] = planeConstruct(farBottomRight , farTopRight    , farTopLeft);     // Far.
    camera->frustrum[2] = planeConstruct(nearBottomLeft , farBottomLeft  , farTopLeft);     // Left.
    camera->frustrum[3] = planeConstruct(nearBottomRight, farTopRight    , farBottomRight); // Right.
    camera->frustrum[4] = planeConstruct(nearBottomLeft , nearBottomRight, farBottomRight); // Bottom.
    camera->frustrum[5] = planeConstruct(nearTopLeft    , farTopLeft     , farTopRight);    // Top.
}

void			cameraTranslate(Camera *camera, Vector translation) {
    camera->position = vectorAdd(camera->position, translation);

    cameraUpdate(camera);
}

void			cameraTranslateLocally(Camera *camera, Vector translation) {
    camera->position = vectorAdd(camera->position, vectorScale(camera->forward, translation.x));
    camera->position = vectorAdd(camera->position, vectorScale(camera->right  , translation.y));
    camera->position = vectorAdd(camera->position, vectorScale(camera->up     , translation.z));

    cameraUpdate(camera);
}

void			cameraRotate(Camera *camera, Vector axis, float angle) {
    Vector tempVect = {0.0f, 0.0f, 0.0f};

    camera->forward = vectorRotate(tempVect, axis, camera->forward, angle);
         camera->up = vectorRotate(tempVect, axis, camera->up     , angle);
      camera->right = vectorRotate(tempVect, axis, camera->right  , angle);

    cameraUpdate(camera);
}

void			cameraRotateLocally(Camera *camera, Vector axis, float angle) {
    Vector tempVect = {0.0f, 0.0f, 0.0f};

       camera->up = vectorRotate(tempVect, camera->forward, camera->up   , axis.x * angle);
    camera->right = vectorRotate(tempVect, camera->forward, camera->right, axis.x * angle);

    camera->forward = vectorRotate(tempVect, camera->right, camera->forward, axis.y * angle);
         camera->up = vectorRotate(tempVect, camera->right, camera->up     , axis.y * angle);

    camera->forward = vectorRotate(tempVect, camera->up, camera->forward, axis.z * angle);
      camera->right = vectorRotate(tempVect, camera->up, camera->right  , axis.z * angle);

    cameraUpdate(camera);
}
