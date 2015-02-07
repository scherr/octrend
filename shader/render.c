/* render.c
 *
 * Maximilian Scherr
 * 2009
 */

#include "render.h"
#include "time.h"

static RenderEnv renderEnv;

RenderEnv *renderInit(Scene scene, unsigned short maxDrawDepth, float highMediumDetailBorder, float mediumLowDetailBorder) {
    renderEnv.scene = scene;
    renderEnv.viewWidth = 1;
    renderEnv.viewHeight = 1;
    renderEnv.maxDrawDepth = maxDrawDepth;
    renderEnv.highMediumDetailBorder = highMediumDetailBorder;
    renderEnv.mediumLowDetailBorder = mediumLowDetailBorder;

    renderEnv.shaderProgram = glCreateProgram();

    const char *shaderSource = STRINGIFY(
        uniform float pointSize;
        uniform float pointScale;
        uniform vec3 cameraPosition;
        void main()	{
            float distance = length(gl_Vertex - cameraPosition);
            gl_PointSize = pointSize * pointScale / distance;

            gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz, 1.0f);

            gl_FrontColor = gl_Color;
        }
    );
    unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &shaderSource, 0);
    glCompileShader(shader);
    glAttachShader(renderEnv.shaderProgram, shader);

    glLinkProgram(renderEnv.shaderProgram);
    int success = 0;
    glGetProgramiv(renderEnv.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        printf("Shader error!\n");
    }

    return &renderEnv;
}

void renderUpdate(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, renderEnv.viewWidth, renderEnv.viewHeight);

    float fieldOfView = renderEnv.scene.camera.fieldOfView / renderEnv.scene.camera.zoom;
    float aspectRatio = renderEnv.viewWidth / renderEnv.viewHeight;
    float    nearDist = renderEnv.scene.camera.nearDist;
    float     farDist = renderEnv.scene.camera.farDist;

    gluPerspective(fieldOfView, aspectRatio, nearDist, farDist);

    fieldOfView *= M_PI / 180.0f;

    renderEnv.scene.camera.nearHeight = 2.0f * tan(fieldOfView / 2.0f) * nearDist;
     renderEnv.scene.camera.nearWidth = renderEnv.scene.camera.nearHeight * aspectRatio;
     renderEnv.scene.camera.farHeight = 2.0f * tan(fieldOfView / 2.0f) * farDist;
      renderEnv.scene.camera.farWidth = renderEnv.scene.camera.farHeight * aspectRatio;

    cameraUpdate(&renderEnv.scene.camera);
}

void renderReshape(int width, int height) {
    if (height == 0) {
        height = 1;
    }

     renderEnv.viewWidth = width;
    renderEnv.viewHeight = height;

    renderUpdate();
}

static int renderedFrames;
time_t lastMeasureTime;

void renderDisplay(void) {
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
     *   / forward.x \               / 1 \               / 0 \               / 0 \
     *   | forward.y | = forward.x * | 0 | + forward.y * | 1 | + forward.z * | 0 |
     *   \ forward.z /               \ 0 /               \ 0 /               \ 1 /
     *
     *     / right.x \               / 1 \               / 0 \               / 0 \
     *     | right.y | =   right.x * | 0 | +   right.y * | 1 | +   right.z * | 0 |
     *     \ right.z /               \ 0 /               \ 0 /               \ 1 /
     *
     *        / up.x \               / 1 \               / 0 \               / 0 \
     *        | up.y | =      up.x * | 0 | +      up.y * | 1 | +      up.z * | 0 |
     *        \ up.z /               \ 0 /               \ 0 /               \ 1 /
     *
     *
     *   From the coefficients we can immediately construct a transformation matrix:
     *
     *       / forward.x forward.y forward.z \
     *   M = |   right.x   right.y   right.z |
     *       \      up.x      up.y      up.z /
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
             renderEnv.scene.camera.position.y,
             renderEnv.scene.camera.position.z,
            -renderEnv.scene.camera.position.x,
             renderEnv.scene.camera.position.y + renderEnv.scene.camera.forward.y,
             renderEnv.scene.camera.position.z + renderEnv.scene.camera.forward.z,
            -renderEnv.scene.camera.position.x - renderEnv.scene.camera.forward.x,
             renderEnv.scene.camera.up.y,
             renderEnv.scene.camera.up.z,
            -renderEnv.scene.camera.up.x
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

    if (renderEnv.scene.root != NULL) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        glUseProgram(renderEnv.shaderProgram);

        // This reqires "-DGL_GLEXT_PROTOTYPES" to be added to CFLAGS.
        glUniform1f(
            glGetUniformLocation(renderEnv.shaderProgram, "pointScale"),
            renderEnv.viewHeight / tan(renderEnv.scene.camera.fieldOfView * 0.5 / renderEnv.scene.camera.zoom * M_PI / 180.0f)
        );
        glUniform3f(
            glGetUniformLocation(renderEnv.shaderProgram, "cameraPosition"),
            renderEnv.scene.camera.position.x,
            renderEnv.scene.camera.position.y,
            renderEnv.scene.camera.position.z
        );

        renderDrawVoxel(
            renderEnv.scene.root,
            1,
            vectorLiteral(
                renderEnv.scene.rootSize / 2.0f,
                renderEnv.scene.rootSize / 2.0f,
                renderEnv.scene.rootSize / 2.0f
            ),
            renderEnv.scene.rootSize,
            renderEnv.maxDrawDepth
        );

        glUseProgram(0);

        glutSwapBuffers();
    }

    renderedFrames++;
    if (time(NULL) - lastMeasureTime >= 1) {
        printf("FPS: %d\n", renderedFrames);
        renderedFrames = 0;
        lastMeasureTime = time(NULL);
    }
}

void renderDrawVoxel(Voxel *voxel, int frustCull, Vector center, float size, unsigned short maxDrawDepth) {
    int i;
    float distance;
    if (frustCull) {
        int count = 0;
        for (i = 0; i < 6; i++) {
            distance = planeDistance(renderEnv.scene.camera.frustrum[5 - i], center);
            if (distance < -size) {
                return;
            }
            if (distance >= size) {
                count++;
            }
        }

        if (count == 6) {
            frustCull = 0;
        }
    } else {
        distance = planeDistance(renderEnv.scene.camera.frustrum[0], center);
    }

    unsigned short depth = voxelGetDepth(voxel);
    if (depth == VOXEL_DISPLAY_VOXEL_DEPTH) {
        // This should be moved to a background thread ... or something similar.
        if (voxelIsDisplayUpdateRequired(voxel)) {
            voxelUpdateDisplay(
                voxel,
                center,
                size,
                maxDrawDepth
            );
        }

        i = 0;
        if (distance > renderEnv.highMediumDetailBorder * renderEnv.scene.camera.farDist) {
            // This reqires "-DGL_GLEXT_PROTOTYPES" to be added to CFLAGS.
            glUniform1f(glGetUniformLocation(renderEnv.shaderProgram, "pointSize"), pow(2.0f, VOXEL_MEDIUM_DETAIL_MAX_DRAW_DEPTH_DISTANCE));
            i++;
            if (distance > renderEnv.mediumLowDetailBorder * renderEnv.scene.camera.farDist) {
                // This reqires "-DGL_GLEXT_PROTOTYPES" to be added to CFLAGS.
                glUniform1f(glGetUniformLocation(renderEnv.shaderProgram, "pointSize"), pow(2.0f, VOXEL_LOW_DETAIL_MAX_DRAW_DEPTH_DISTANCE));
                i++;
            }
        } else {
            // This reqires "-DGL_GLEXT_PROTOTYPES" to be added to CFLAGS.
            glUniform1f(glGetUniformLocation(renderEnv.shaderProgram, "pointSize"), 1.0f);
        }

        glCallList(voxelGetDisplayList(voxel) + i);
    } else if (depth < VOXEL_DISPLAY_VOXEL_DEPTH) {
        float half = size / 2.0f;
        float quarter = size / 4.0f;
        for (i = 0; i < voxel->childCount; i++) {
            switch (voxelGetPosition(voxel->children[i])) {
                case 0:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x - quarter, center.y - quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 1:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x - quarter, center.y - quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                case 2:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x - quarter, center.y + quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 3:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x - quarter, center.y + quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                case 4:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x + quarter, center.y - quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 5:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x + quarter, center.y - quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                case 6:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x + quarter, center.y + quarter, center.z - quarter), half, maxDrawDepth);
                    break;

                case 7:
                    renderDrawVoxel(voxel->children[i], frustCull, vectorLiteral(center.x + quarter, center.y + quarter, center.z + quarter), half, maxDrawDepth);
                    break;

                default:
                    break;
            }
        }
    }
}
