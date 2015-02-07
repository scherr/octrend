/* rendering.c
 *
 * Maximilian Scherr
 * 2009
 */

#include "rendering.h"
#include "time.h"

/* Not documented yet.
 *
 */
unsigned char facesLookupTable[64][25] =
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

/* Not documented yet.
 *
 */
renderSettings currentRenderSettings;

int renderedVertices;
int renderedFrames;
time_t lastMeasureTime;

/* Not documented yet.
 *
 */
renderSettings *initializeRendering(scene scene, unsigned char maxDetailLevel, float detailBorders[]) {
    // Not commented yet.
    unsigned int i;

    if (scene.octreeRoot == NULL || scene.camera == NULL || detailBorders == NULL) {
        return NULL;
    }

    currentRenderSettings.scene = scene;
    currentRenderSettings.maxDetailLevel = maxDetailLevel;
    for (i = 0; i < 8; i++) {
        currentRenderSettings.detailBorders[i] = detailBorders[i];
    }

    return &currentRenderSettings;
}

/* Not documented yet.
 *
 */
void updateView(void) {
    // Not commented yet.
    float aspectRatio;
    float fieldOfView;
    float nearDist;
    float farDist;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, currentRenderSettings.viewWidth, currentRenderSettings.viewHeight);

    if (currentRenderSettings.scene.camera == NULL) {
        // This should never happen!
        currentRenderSettings.scene.camera = createCamera(45.0f, 1.0f, 1000.0f, 0);
    }

    fieldOfView = currentRenderSettings.scene.camera->fieldOfView / currentRenderSettings.scene.camera->zoom;
    aspectRatio = currentRenderSettings.viewWidth / currentRenderSettings.viewHeight;
    nearDist = currentRenderSettings.scene.camera->nearDist;
    farDist = currentRenderSettings.scene.camera->farDist;

    if (currentRenderSettings.scene.camera->ortho == 0) {
        gluPerspective(fieldOfView, aspectRatio, nearDist, farDist);

        fieldOfView *= M_PI / 180.0f;
        currentRenderSettings.scene.camera->nearHeight = 2.0f * tan(fieldOfView / 2.0f) * nearDist;
        currentRenderSettings.scene.camera->nearWidth = currentRenderSettings.scene.camera->nearHeight * aspectRatio;
        currentRenderSettings.scene.camera->farHeight = 2.0f * tan(fieldOfView / 2.0f) * farDist;
        currentRenderSettings.scene.camera->farWidth = currentRenderSettings.scene.camera->farHeight * aspectRatio;
    } else {
        currentRenderSettings.scene.camera->nearHeight = fieldOfView;
        currentRenderSettings.scene.camera->nearWidth = fieldOfView * aspectRatio;
        currentRenderSettings.scene.camera->farHeight = fieldOfView;
        currentRenderSettings.scene.camera->farWidth = fieldOfView * aspectRatio;

        glOrtho(
            -currentRenderSettings.scene.camera->nearWidth / 2.0f,
            currentRenderSettings.scene.camera->nearWidth / 2.0f,
            -currentRenderSettings.scene.camera->nearHeight / 2.0f,
            currentRenderSettings.scene.camera->nearHeight / 2.0f,
            nearDist,
            farDist
        );
    }

    updateCameraAnnotations(currentRenderSettings.scene.camera);
}

/* Not documented yet.
 *
 */
void changeSize(int width, int height) {
    // Not commented yet.
    if (height == 0) {
        height = 1;
    }

    currentRenderSettings.viewWidth = width;
    currentRenderSettings.viewHeight = height;

    updateView();
}

/* Not documented yet.
 *
 */
void renderScene(void) {
    // Not commented yet.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* We want to transform coordinates in respect to the standard (world) basis to coordinates in respect to the camera's orientation and position.
     * The camera's basis is defined by it's direction, right and up vectors. Since we are not using homogenous coordinates (OpenGL does though)
     * we have to transform in two passes: translation & orientation.
     *
     * Translation:
     * -----------
     *
     *   Simply translate a vector to transform by -position (of camera).
     *
     *
     * Orientation:
     * -----------
     *
     *   What we need is a matrix for change of basis. First let's describe the camera vectors as a linear combination of standard basis vectors:
     *
     *   / direction.x \                 / 1 \                 / 0 \                 / 0 \
     *   | direction.y | = direction.x * | 0 | + direction.y * | 1 | + direction.z * | 0 |
     *   \ direction.z /                 \ 0 /                 \ 0 /                 \ 1 /
     *
     *       / right.x \                 / 1 \                 / 0 \                 / 0 \
     *       | right.y | =     right.x * | 0 | +     right.y * | 1 | +     right.z * | 0 |
     *       \ right.z /                 \ 0 /                 \ 0 /                 \ 1 /
     *
     *          / up.x \                 / 1 \                 / 0 \                 / 0 \
     *          | up.y | =        up.x * | 0 | +        up.y * | 1 | +        up.z * | 0 |
     *          \ up.z /                 \ 0 /                 \ 0 /                 \ 1 /
     *
     *
     *   From the coefficients we can immediately construct a transformation matrix:
     *
     *       / direction.x direction.y direction.z \
     *   M = |     right.x     right.y     right.z |
     *       \        up.x        up.y        up.z /
     *
     *
     * Finally:
     * -------
     *
     *   For each vector w in respect to the world's basis we can calculate vector c in respect to the camera's position and orientation:
     *
     * 	 c = M * (v - position)
     *
     *
     *   Now, this long explanation was mainly for general understanding and a possible software-only implementations in the future.
     *   Since OpenGL does that more efficiently (and since I'm not so bright when it comes to linear algebra) with the gluLookAt(...)
     *   function, we just delegate the work.
     *
     *
     * One last thing:
     * --------------
     *
     *   We use a different coordinate system than OpenGL does.
     *
     *     z    x  our coordinate system    y   -z  OpenGL's coordinate system    /   y     OpenGL's coordinate system \
     *     *   *                            *   *   (mind the minus in -z!)       |   *                                |
     *     *  *                             *  *                                  |   *                                |
     *     * *                              * *                                   |   o**x                             |
     *     **                               **                                    |  *                                 |
     *     o****y                           o****x                                \ z                                  /
     *
     *
     *   So, again we have to transform. This is not so much of a problem though. There are to ways to do this:
     *
     *	 Either manually changing / switching coordinates ...:
     *   ----------------------------------------------------
     *
     *     x (OpenGL) =  y (our)    x (our) = -z (OpenGL)
     *     y (OpenGL) =  z (our)    y (our) =  x (OpenGL)
     *     z (OpenGL) = -x (our)    z (our) =  y (OpenGL)
     *
     *
     *   ... or a (properly ordered) chain of transformations:
     *   ----------------------------------------------------
     *
     *     S(axis, factor): Scale matrix.
     *     R(axis, angle) : Rotation matrix.
     *     v              : Vector in our coordinate system.
     *     g              : Vector in OpenGL coordinate system.
     *
     *     g = S(z, -1)  * R(x, -90°) * R(z, -90°) * v
     *     v = R(z, 90°) * R(x, 90°)  * S(z, -1)   * g
     *
     *     Of course there are several possible chains of transformation with the same effect (and I won't list all of them).
     *     Again we will let OpenGL do the hard work with glScalef(...) and glRotatef(...).
     *
     *
     * Well then ...
     *  
     * BEGIN (Transformation block)
     *
     */
        // From the point / vector perspective this is done last. The point / vector is transformed to coordinates in respect to
        // the camera's position and orientation. The coordinates of the camera position and orientation are manually adapted to
        // the coordinate system of OpenGL.
        gluLookAt(
            currentRenderSettings.scene.camera->position.y,
            currentRenderSettings.scene.camera->position.z,
            -currentRenderSettings.scene.camera->position.x,
            currentRenderSettings.scene.camera->position.y + currentRenderSettings.scene.camera->direction.y,
                currentRenderSettings.scene.camera->position.z + currentRenderSettings.scene.camera->direction.z,
            -currentRenderSettings.scene.camera->position.x - currentRenderSettings.scene.camera->direction.x,
            currentRenderSettings.scene.camera->up.y,
            currentRenderSettings.scene.camera->up.z,
            -currentRenderSettings.scene.camera->up.x
        );

        // The OpenGL "transformation logic" is reversed (which is a good thing), because all these functions create a matrix and mutliply
        // the current transformation matrix with this matrix. So from the point / vector perspective to be drawn later we first
        // rotate about it the z axis by -90 degrees, then rotate it about the x axis by -90 degrees and finally scale it along the z axis by
        // the factor -1. After this the point / vector is in OpenGL coordinates.
        glScalef(1.0f, 1.0f, -1.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        // Remember, alternatively we could not do these transformations but instead manually adapt coordinates to the OpenGL coordinate system
        // in every future OpenGL function call.

        // This transformation also works:
        // glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        // glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        // glScalef(1.0f, -1.0f, 1.0f);
    /* END (Transformation block)
     *
     * ... we are finished with the transformations.
     *
     */

    renderedVertices = 0;

    if (currentRenderSettings.scene.octreeRoot != NULL && currentRenderSettings.scene.camera != NULL) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawCheck(currentRenderSettings.scene.octreeRoot, currentRenderSettings.scene.camera);
        glutSwapBuffers();
    }

    renderedFrames++;
    if (time(NULL) - lastMeasureTime >= 1) {
        printf("FPS: %d\n", renderedFrames);
        renderedFrames = 0;
        lastMeasureTime = time(NULL);
    }

    // printf("Rendered vertices: %d\n", renderedVertices);
}

/* Not documented yet.
 *
 */
unsigned char calculateViewDependentFacesFlag(voxel *voxelToDraw, camera *cam) {
    // Not commented yet.
    unsigned char facesFlag = 0;

    if (cam->position.x < voxelToDraw->center.x) {
        facesFlag = facesFlag | 1;
    } else {
        facesFlag = facesFlag | 2;
    }
    if (cam->position.y < voxelToDraw->center.y) {
        facesFlag = facesFlag | 4;
    } else {
        facesFlag = facesFlag | 8;
    }
    if (cam->position.z < voxelToDraw->center.z) {
        facesFlag = facesFlag | 16;
    } else {
        facesFlag = facesFlag | 32;
    }

    return facesFlag;
}

/* Not documented yet.
 *
 */
void drawVoxel(octree *node, float distance, unsigned char detailLevel, camera *cam) {
    // Not commented yet.
    voxel *voxelToDraw = (voxel *) node->value;
    unsigned char facesFlag = calculateViewDependentFacesFlag(voxelToDraw, cam);
    // This border check seems to work. Maybe needs some tinkering.
    char atDetailBorder = getDetailLevel(distance - voxelToDraw->radius * 2.0f, cam) < detailLevel;
    unsigned int i;
    unsigned int j;
    float tempDist;
    float lightIntensity;
    float lightColRed = 1.0f;
    float lightColGreen = 1.0f;
    float lightColBlue = 1.0f;
    floatVector tempVector;
    octree *tempNode;
    intVector topCoords = {0, 0, 0};
    octree *rayHit;
    floatVector vertex;

    // Here we choose a different faces flag depending on whether we are drawing at the boundary between two detail levels.
    if (atDetailBorder) {
        facesFlag = facesFlag & voxelToDraw->facesFlag;
    } else {
        facesFlag = facesFlag & voxelToDraw->levelFacesFlag;
    }

    if (facesFlag == 0) {
        return;
    }

    if (voxelToDraw->lighting != 0) {
        for (i = 0; i < currentRenderSettings.scene.lightCount; i++) {
            tempDist = calculateDistancePointPoint(currentRenderSettings.scene.lights[i]->position, voxelToDraw->center);
            if (tempDist <= currentRenderSettings.scene.lights[i]->radius) {
                if (currentRenderSettings.scene.lights[i]->intDecr > 0.0f) {
                    lightIntensity = -((1.0f / currentRenderSettings.scene.lights[i]->intDecr) / currentRenderSettings.scene.lights[i]->radius) * tempDist +
                        (1.0f / currentRenderSettings.scene.lights[i]->intDecr);
                    if (lightIntensity > 1.0f) {
                        lightIntensity = 1.0f;
                    }
                } else {
                    lightIntensity = 1.0f;
                }

                if (currentRenderSettings.scene.lights[i]->trace == 0) {
                    lightColRed += currentRenderSettings.scene.lights[i]->colRed * lightIntensity;
                    lightColGreen += currentRenderSettings.scene.lights[i]->colGreen * lightIntensity;
                    lightColBlue += currentRenderSettings.scene.lights[i]->colBlue * lightIntensity;
                } else {
                    // The idea to solve the border detail glitch problem I experienced for a long time is as follows:
                    //
                    //   1. Find a start point away from the border voxel / node to the camera (light source probably
                    //      works as well, maybe better ... testing). Why did I choose the camera? Because the plane towards
                    //      the camera is "cut off" at the detail borders. We have yet to find out whether we finally got rid
                    //      of the glitches.
                    //
                    //   2. From that point we try to cast a ray to the original voxel / node, thus we find the outermost
                    //      voxel / node, i.e. the "first" voxel seen by the camera (or light source).
                    //      Since castRay always returns the first hit we need to know which voxel / node that would be if
                    //      we were to cast a ray towards the original voxel / node.
                    //
                    //   3. With this outermost voxel / node we do the actual light tracing, through which we decide whether
                    //		the original voxel / node is lit.
                    //
                    //
                    //   Of course, in case we are not at the detail border we just perform the tests on the original
                    //   voxel / node itself as we did before.
                    if (atDetailBorder) {
                        // 4.0f seems to work fine, but this factor may need adjustment later after testing.
                        tempVector = subFloatVector(voxelToDraw->center, scalarMulFloatVector(cam->direction, voxelToDraw->radius * 4.0f));
                        tempNode = castRay(
                            currentRenderSettings.scene.octreeRoot,
                            node->level,
                            tempVector,
                            cam->direction
                        );
                    } else {
                        tempNode = node;
                    }

                    if (tempNode != NULL) {
                        // Crude pre-check. Helps speeding things up (can lead to small artifacts though).
                        tempVector = normalizeFloatVector(subFloatVector(((voxel *) tempNode->parent->value)->center, currentRenderSettings.scene.lights[i]->position));
                        rayHit = castRay(
                            currentRenderSettings.scene.octreeRoot,
                            tempNode->level,
                            addFloatVector(currentRenderSettings.scene.lights[i]->position, scalarMulFloatVector(tempVector, currentRenderSettings.scene.lights[i]->traceStart)),
                            tempVector
                        );
                        if (rayHit != NULL && isOctreeDescendantOf(rayHit, tempNode->parent)) {
                            lightColRed += currentRenderSettings.scene.lights[i]->colRed * lightIntensity;
                            lightColGreen += currentRenderSettings.scene.lights[i]->colGreen * lightIntensity;
                            lightColBlue += currentRenderSettings.scene.lights[i]->colBlue * lightIntensity;

                            continue;
                        }

                        // Accurate after-check to cover the missed faces.
                        for (j = 0; j < 6; j++) {
                            tempVector = normalizeFloatVector(subFloatVector(((voxel *) tempNode->value)->faces[j], currentRenderSettings.scene.lights[i]->position));
                            rayHit = castRay(
                                currentRenderSettings.scene.octreeRoot,
                                tempNode->level,
                                addFloatVector(currentRenderSettings.scene.lights[i]->position, scalarMulFloatVector(tempVector, currentRenderSettings.scene.lights[i]->traceStart)),
                                tempVector
                            );
                            if (rayHit == tempNode) {
                                lightColRed += currentRenderSettings.scene.lights[i]->colRed * lightIntensity;
                                lightColGreen += currentRenderSettings.scene.lights[i]->colGreen * lightIntensity;
                                lightColBlue += currentRenderSettings.scene.lights[i]->colBlue * lightIntensity;

                                break;
                            }
                        }
                    }
                }
            }
        }

        lightColRed *= currentRenderSettings.scene.ambientLightColRed;
        lightColGreen *= currentRenderSettings.scene.ambientLightColGreen;
        lightColBlue *= currentRenderSettings.scene.ambientLightColBlue;

        if (lightColRed > 1.0f) {
            lightColRed = 1.0f;
        }
        if (lightColGreen > 1.0f) {
            lightColGreen = 1.0f;
        }
        if (lightColBlue > 1.0f) {
            lightColBlue = 1.0f;
        }
    }

    /*
    if (atDetailBorder) {
        lightColRed = 1.0f;
        lightColGreen = 0.0f;
        lightColBlue = 0.0f;
    }
    */

    glColor4f(voxelToDraw->colRed * lightColRed, voxelToDraw->colGreen * lightColGreen, voxelToDraw->colBlue * lightColBlue, voxelToDraw->colAlpha);

    /*
    glBegin(GL_POINTS);
        glVertex3f(voxelToDraw->center.x, voxelToDraw->center.y, voxelToDraw->center.z);
    glEnd();
    */

    renderedVertices += facesLookupTable[facesFlag][0];

    glBegin(GL_QUADS);
        for (i = 1; i < facesLookupTable[facesFlag][0]; i++) {
            vertex = voxelToDraw->vertices[facesLookupTable[facesFlag][i]];
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    glEnd();
}

/* Not documented yet.
 *
 */
unsigned char getDetailLevel(float distance, camera *cam) {
    // Not commented yet.
    unsigned char i;

    for (i = 0; i < 8; i++) {
        if (currentRenderSettings.maxDetailLevel + i >= currentRenderSettings.scene.octreeRoot->level) {
            return (currentRenderSettings.maxDetailLevel + i - 1) / cam->zoom;
        }

        if (distance < currentRenderSettings.detailBorders[i]) {
            return (currentRenderSettings.maxDetailLevel + i) / cam->zoom;
        }
    }

    return (currentRenderSettings.maxDetailLevel + 8) / cam->zoom;
}

/* Not documented yet.
 *
 */
void drawCheck(octree *node, camera *cam) {
    // Not commented yet.
    voxel *annotatedVoxel = (voxel *) node->value;
    unsigned int i;
    unsigned int count = 0;
    float distance;
    unsigned char detailLevel;

    if (annotatedVoxel->facesFlag == 0) {
        return;
    }

    for (i = 0; i < 6; i++) {
        distance = calculateDistancePointPlane(annotatedVoxel->center, cam->frustrum[5 - i]);
        if (distance < -annotatedVoxel->radius) {
            return;
        }
        if (distance >= annotatedVoxel->radius) {
            count++;
        }
    }

    detailLevel = getDetailLevel(distance, cam);
    if (node->terminal != 0 || node->level <= detailLevel) {
        drawVoxel(node, distance, detailLevel, cam);

        return;
    }

    if (count == 6) {
        for (i = 0; i < 8; i++) {
            if (node->children[i] != NULL) {
                drawNoCheck(node->children[i], cam);
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (node->children[i] != NULL) {
                drawCheck(node->children[i], cam);
            }
        }
    }
}

/* Not documented yet.
 *
 */
void drawNoCheck(octree *node, camera *cam) {
    // Not commented yet.
    voxel *annotatedVoxel = (voxel *) node->value;
    unsigned int i;
    float distance;
    unsigned char detailLevel;

    // This works, but I want to test on higher levels as well.
    if (annotatedVoxel->facesFlag == 0) {
        return;
    }

    distance = calculateDistancePointPlane(annotatedVoxel->center, cam->frustrum[0]);
    detailLevel = getDetailLevel(distance, cam);
    if (node->terminal != 0 || node->level <= detailLevel) {
        drawVoxel(node, distance, detailLevel, cam);

        return;
    }

    for (i = 0; i < 8; i++) {
        if (node->children[i] != NULL) {
            drawNoCheck(node->children[i], cam);
        }
    }
}

/* Slow but working!!!
 *
 */
octree *castRay(octree *topNode, unsigned char terminalLevel, floatVector origin, floatVector direction) {
    // Not commented yet.
    intVector topCoords = {0, 0, 0};
    intVector neighDir;
    float par;
    intVector step = {direction.x >= 0 ? 1 : -1, direction.y >= 0 ? 1 : -1, direction.z >= 0 ? 1 : -1};
    floatVector parMax;
    floatVector parDelta;
    octree virtNode = findVirtualOctree(floorFloatVector(origin), terminalLevel, topNode, topCoords);
    unsigned char level;
    float size;

    direction = normalizeFloatVector(direction);

    while (virtNode.terminal == 0 && virtNode.parent != NULL) {
        if (virtNode.level == terminalLevel && virtNode.childCount > 0) {
            break;
        }

        level = virtNode.level;
        size = (float) (1 << level);

        // Note: 0.01f might probably be even smaller. The crucial point here is "crossing the border".
        if (step.x == 1) {
            parMax.x = (size - fmod(origin.x, size)) / direction.x;
            parDelta.x = size / direction.x;
        } else {
            parMax.x = (fmod(origin.x, size) + 0.01f) / -direction.x;
            parDelta.x = size / -direction.x;
        }
        if (step.y == 1) {
            parMax.y = (size - fmod(origin.y, size)) / direction.y;
            parDelta.y = size / direction.y;
        } else {
            parMax.y = (fmod(origin.y, size) + 0.01f) / -direction.y;
            parDelta.y = size / -direction.y;
        }
        if (step.z == 1) {
            parMax.z = (size - fmod(origin.z, size)) / direction.z;
            parDelta.z = size / direction.z;
        } else {
            parMax.z = (fmod(origin.z, size) + 0.01f) / -direction.z;
            parDelta.z = size / -direction.z;
        }

        while (virtNode.terminal == 0 && virtNode.parent != NULL && virtNode.level == level && virtNode.childCount == 0) {
            neighDir.x = 0;
            neighDir.y = 0;
            neighDir.z = 0;
            if (parMax.x < parMax.y) {
                if (parMax.x < parMax.z) {
                    neighDir.x = step.x;
                    parMax.x += parDelta.x;
                } else {
                    neighDir.z = step.z;
                    parMax.z += parDelta.z;
                }
            } else {
                if (parMax.y < parMax.z) {
                    neighDir.y = step.y;
                    parMax.y += parDelta.y;
                } else {
                    neighDir.z = step.z;
                    parMax.z += parDelta.z;
                }
            }

            virtNode = findNeighbour(virtNode, neighDir);
        }

        if (virtNode.level == terminalLevel && virtNode.childCount > 0) {
            break;
        }

        if (virtNode.terminal == 0 && virtNode.parent != NULL) {
            if (neighDir.x != 0) {
                par = parMax.x - parDelta.x;
            }
            if (neighDir.y != 0) {
                par = parMax.y - parDelta.y;
            }
            if (neighDir.z != 0) {
                par = parMax.z - parDelta.z;
            }

            origin = addFloatVector(origin, scalarMulFloatVector(direction, par));

            // Note: Rounding (and stupidity) problems require testing from topNode.
            virtNode = findVirtualOctree(floorFloatVector(origin), terminalLevel, topNode, topCoords);
        }
    }

    if (virtNode.parent == NULL) {
        return NULL;
    }

    return virtNode.parent->children[virtNode.position];
}
