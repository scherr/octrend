/* scene.c
 *
 * Maximilian Scherr
 * 2009
 */

#include "scene.h"

/* Not documented yet.
 *
 */
scene createScene(octree *octreeRoot, camera *camera, float ambientLightColRed, float ambientLightColGreen, float ambientLightColBlue) {
    // Not commented yet.
    scene newScene = {octreeRoot, camera, ambientLightColRed, ambientLightColGreen, ambientLightColBlue, 0};

    return newScene;
}

/* Not documented yet.
 *
 */
camera *createCamera(float fieldOfView, float nearDist, float farDist, char ortho) {
    // Not commented yet.
    camera *newCamera;

    newCamera = malloc(sizeof(camera));

    newCamera->position.x = 0.0f;
    newCamera->position.y = 0.0f;
    newCamera->position.z = 0.0f;
    newCamera->direction.x = 1.0f;
    newCamera->direction.y = 0.0f;
    newCamera->direction.z = 0.0f;
    newCamera->up.x = 0.0f;
    newCamera->up.y = 0.0f;
    newCamera->up.z = 1.0f;
    newCamera->right.x = 0.0f;
    newCamera->right.y = 1.0f;
    newCamera->right.z = 0.0f;
    newCamera->zoom = 1.0f;
    newCamera->fieldOfView = fieldOfView;
    newCamera->nearDist = nearDist;
    newCamera->farDist = farDist;
    newCamera->nearHeight = 0;
    newCamera->nearWidth = 0;
    newCamera->farHeight = 0;
    newCamera->farWidth = 0;
    newCamera->ortho = ortho;

    return newCamera;
}

/* Not documented yet.
 *
 */
int updateCameraAnnotations(camera *cam) {
    // Not commented yet.
    floatVector up = cam->up;
    floatVector down = scalarMulFloatVector(up, -1.0f);
    floatVector right = cam->right;
    floatVector left = scalarMulFloatVector(right, -1.0f);
    floatVector nearLeft = addFloatVector(addFloatVector(cam->position, scalarMulFloatVector(cam->direction, cam->nearDist)), scalarMulFloatVector(left, cam->nearWidth / 2.0f));
    floatVector nearRight = addFloatVector(addFloatVector(cam->position, scalarMulFloatVector(cam->direction, cam->nearDist)), scalarMulFloatVector(right, cam->nearWidth / 2.0f));
    floatVector nearBottomLeft = addFloatVector(nearLeft, scalarMulFloatVector(down, cam->nearHeight / 2.0f));
    floatVector nearBottomRight = addFloatVector(nearRight, scalarMulFloatVector(down, cam->nearHeight / 2.0f));
    floatVector nearTopRight = addFloatVector(nearRight, scalarMulFloatVector(up, cam->nearHeight / 2.0f));
    floatVector nearTopLeft = addFloatVector(nearLeft, scalarMulFloatVector(up, cam->nearHeight / 2.0f));
    floatVector farLeft = addFloatVector(addFloatVector(cam->position, scalarMulFloatVector(cam->direction, cam->farDist)), scalarMulFloatVector(left, cam->farWidth / 2.0f));
    floatVector farRight = addFloatVector(addFloatVector(cam->position, scalarMulFloatVector(cam->direction, cam->farDist)), scalarMulFloatVector(right, cam->farWidth / 2.0f));
    floatVector farBottomLeft = addFloatVector(farLeft, scalarMulFloatVector(down, cam->farHeight / 2.0f));
    floatVector farBottomRight = addFloatVector(farRight, scalarMulFloatVector(down, cam->farHeight / 2.0f));
    floatVector farTopRight = addFloatVector(farRight, scalarMulFloatVector(up, cam->farHeight / 2.0f));
    floatVector farTopLeft = addFloatVector(farLeft, scalarMulFloatVector(up, cam->farHeight / 2.0f));

    /*
    printf("nearBottomLeft  %f %f %f\n", nearBottomLeft.x, nearBottomLeft.y, nearBottomLeft.z);
    printf("nearBottomRight %f %f %f\n", nearBottomRight.x, nearBottomRight.y, nearBottomRight.z);
    printf("nearTopRight    %f %f %f\n", nearTopRight.x, nearTopRight.y, nearTopRight.z);
    printf("nearTopLeft     %f %f %f\n", nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);

    printf("farBottomLeft  %f %f %f\n", farBottomLeft.x, farBottomLeft.y, farBottomLeft.z);
    printf("farBottomRight %f %f %f\n", farBottomRight.x, farBottomRight.y, farBottomRight.z);
    printf("farTopRight    %f %f %f\n", farTopRight.x, farTopRight.y, farTopRight.z);
    printf("farTopLeft     %f %f %f\n", farTopLeft.x, farTopLeft.y, farTopLeft.z);
    */

    // Near.
    cam->frustrum[0] = calculatePlane(nearBottomLeft, nearTopLeft, nearTopRight);
    // Far.
    cam->frustrum[1] = calculatePlane(farBottomRight, farTopRight, farTopLeft);
    // Left.
    cam->frustrum[2] = calculatePlane(nearBottomLeft, farBottomLeft, farTopLeft);
    // Right.
    cam->frustrum[3] = calculatePlane(nearBottomRight, farTopRight, farBottomRight);
    // Bottom.
    cam->frustrum[4] = calculatePlane(nearBottomLeft, nearBottomRight, farBottomRight);
    // Top.
    cam->frustrum[5] = calculatePlane(nearTopLeft, farTopLeft, farTopRight);

    return 0;
}

/* Not documented yet.
 *
 */
void translateCamera(camera *cam, floatVector translation) {
    // Not commented yet.
    cam->position = addFloatVector(cam->position, translation);

    updateCameraAnnotations(cam);
}

/* Not documented yet.
 *
 */
void translateCameraLocally(camera *cam, floatVector translation) {
    // Not commented yet.
    cam->position = addFloatVector(cam->position, scalarMulFloatVector(cam->direction, translation.x));
    cam->position = addFloatVector(cam->position, scalarMulFloatVector(cam->right, translation.y));
    cam->position = addFloatVector(cam->position, scalarMulFloatVector(cam->up, translation.z));

    updateCameraAnnotations(cam);
}

/* Not documented yet.
 *
 */
void rotateCamera(camera *cam, floatVector axis, float angle) {
    // Not commented yet.
    floatVector tempCoords = {0.0f, 0.0f, 0.0f};

    cam->direction = rotateAboutArbitraryAxis(tempCoords, axis, cam->direction, angle);
    cam->up = rotateAboutArbitraryAxis(tempCoords, axis, cam->up, angle);
    cam->right = rotateAboutArbitraryAxis(tempCoords, axis, cam->right, angle);

    updateCameraAnnotations(cam);
}

/* Not documented yet.
 *
 */
void rotateCameraLocally(camera *cam, floatVector axis, float angle) {
    // Not commented yet.
    floatVector tempCoords = {0.0f, 0.0f, 0.0f};

    cam->up = rotateAboutArbitraryAxis(tempCoords, cam->direction, cam->up, axis.x * angle);
    cam->right = rotateAboutArbitraryAxis(tempCoords, cam->direction, cam->right, axis.x * angle);

    cam->direction = rotateAboutArbitraryAxis(tempCoords, cam->right, cam->direction, axis.y * angle);
    cam->up = rotateAboutArbitraryAxis(tempCoords, cam->right, cam->up, axis.y * angle);

    cam->direction = rotateAboutArbitraryAxis(tempCoords, cam->up, cam->direction, axis.z * angle);
    cam->right = rotateAboutArbitraryAxis(tempCoords, cam->up, cam->right, axis.z * angle);

    updateCameraAnnotations(cam);
}

/* Not documented yet.
 *
 */
light *createLight(floatVector position, float radius, float intDecr, float colRed, float colGreen, float colBlue, char trace, float traceStart) {
    // Not commented yet.
    light *newLight;

    newLight = malloc(sizeof(light));
    newLight->position = position;
    newLight->radius = radius;
    newLight->intDecr = intDecr;
    newLight->colRed = colRed;
    newLight->colGreen = colGreen;
    newLight->colBlue = colBlue;
    newLight->trace = trace;
    newLight->traceStart = traceStart;

    return newLight;
}

/* Not documented yet.
 *
 */
int addLight(scene *scene, light *lightToAdd) {
    // Not commented yet.
    if (scene->lightCount == 32) {
        return 1;
    }

    scene->lights[scene->lightCount] = lightToAdd;
    scene->lightCount++;

    return 0;
}

/* Not documented yet.
 *
 */
int removeLight(scene *scene, light *lightToRemove) {
    // Not commented yet.
    unsigned int i;
    unsigned int count = 0;
    light *tempLights[32];

    for (i = 0; i < scene->lightCount; i++) {
        if (scene->lights[i] != lightToRemove) {
            tempLights[count] = scene->lights[i];
            count++;
        }
    }

    for (i = 0; i < count; i++) {
        scene->lights[i] = tempLights[i];
    }

    scene->lightCount = count;

    return 0;
}
