#include "scene.h"
#include "rendering.h"

char currentLightIndex = 0;

void processKeys(unsigned char key, int x, int y) {
	floatVector tempVector = {0.0f, 0.0f, 0.0f};
	switch (key) {
		case '0':
			currentLightIndex = 0;
			break;
			
		case '1':
			currentLightIndex = 1;
			break;
			
		case '2':
			currentLightIndex = 2;
			break;
			
		case '3':
			currentLightIndex = 3;
			break;
			
		// ...
		
		case 'a':
			tempVector.z = 1.0f;
			rotateCameraLocally(currentRenderSettings.scene.camera, tempVector, -2.0f);
			break;
			
		case 'd':
			tempVector.z = 1.0f;
			rotateCameraLocally(currentRenderSettings.scene.camera, tempVector, 2.0f);
			break;
			
		case 'w':
			tempVector.y = 1.0f;
			rotateCameraLocally(currentRenderSettings.scene.camera, tempVector, 2.0f);
			break;
			
		case 's':
			tempVector.y = 1.0f;
			rotateCameraLocally(currentRenderSettings.scene.camera, tempVector, -2.0f);
			break;
			
		case 'q':
			tempVector.x = 1.0f;
			rotateCameraLocally(currentRenderSettings.scene.camera, tempVector, 2.0f);
			break;
			
		case 'e':
			tempVector.x = 1.0f;
			rotateCameraLocally(currentRenderSettings.scene.camera, tempVector, -2.0f);
			break;
		
		case 'h':
			if (currentRenderSettings.scene.lightCount > 0) {
				currentRenderSettings.scene.lights[currentLightIndex]->position.y -= 2.0f;
			}
			break;
			
		case 'k':
			if (currentRenderSettings.scene.lightCount > 0) {
				currentRenderSettings.scene.lights[currentLightIndex]->position.y += 2.0f;
			}
			break;
				
		case 'u':
			if (currentRenderSettings.scene.lightCount > 0) {
				currentRenderSettings.scene.lights[currentLightIndex]->position.x += 2.0f;
			}
			break;
			
		case 'j':
			if (currentRenderSettings.scene.lightCount > 0) {
				currentRenderSettings.scene.lights[currentLightIndex]->position.x -= 2.0f;
			}
			break;
			
		case 'o':
			if (currentRenderSettings.scene.lightCount > 0) {
				currentRenderSettings.scene.lights[currentLightIndex]->position.z += 2.0f;
			}
			break;
			
		case 'l':
			if (currentRenderSettings.scene.lightCount > 0) {
				currentRenderSettings.scene.lights[currentLightIndex]->position.z -= 2.0f;
			}
			break;
		
		case '+':
			currentRenderSettings.scene.camera->zoom += 0.1f;
			updateView();
			break;
			
		case '-':
			currentRenderSettings.scene.camera->zoom -= 0.1f;
			updateView();
			break;
	}
	
	/*
	printf("position %f %f %f\n", currentRenderSettings.scene.camera->position.x, currentRenderSettings.scene.camera->position.y, currentRenderSettings.scene.camera->position.z);
	printf("direction %f %f %f\n", currentRenderSettings.scene.camera->direction.x, currentRenderSettings.scene.camera->direction.y, currentRenderSettings.scene.camera->direction.z);
	printf("up %f %f %f\n", currentRenderSettings.scene.camera->up.x, currentRenderSettings.scene.camera->up.y, currentRenderSettings.scene.camera->up.z);
	printf("right %f %f %f\n", currentRenderSettings.scene.camera->right.x, currentRenderSettings.scene.camera->right.y, currentRenderSettings.scene.camera->right.z);
	*/
	
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
	floatVector tempVector = {0.0f, 0.0f, 0.0f};
	switch (key) {
		case GLUT_KEY_LEFT:
			tempVector.y = -2.0f;
			translateCameraLocally(currentRenderSettings.scene.camera, tempVector);
			break;
			
		case GLUT_KEY_RIGHT:
			tempVector.y = 2.0f;
			translateCameraLocally(currentRenderSettings.scene.camera, tempVector);
			break;
			
		case GLUT_KEY_UP:
			tempVector.x = 2.0f;
			translateCameraLocally(currentRenderSettings.scene.camera, tempVector);
			break;
			
		case GLUT_KEY_DOWN:
			tempVector.x = -2.0f;
			translateCameraLocally(currentRenderSettings.scene.camera, tempVector);
			break;
			
		case GLUT_KEY_PAGE_UP:
			tempVector.z = 2.0f;
			translateCameraLocally(currentRenderSettings.scene.camera, tempVector);
			break;
			
		case GLUT_KEY_PAGE_DOWN:
			tempVector.z = -2.0f;
			translateCameraLocally(currentRenderSettings.scene.camera, tempVector);
			break;
	}
	
	/*
	printf("position %f %f %f\n", currentRenderSettings.scene.camera->position.x, currentRenderSettings.scene.camera->position.y, currentRenderSettings.scene.camera->position.z);
	printf("direction %f %f %f\n", currentRenderSettings.scene.camera->direction.x, currentRenderSettings.scene.camera->direction.y, currentRenderSettings.scene.camera->direction.z);
	printf("up %f %f %f\n", currentRenderSettings.scene.camera->up.x, currentRenderSettings.scene.camera->up.y, currentRenderSettings.scene.camera->up.z);
	printf("right %f %f %f\n", currentRenderSettings.scene.camera->right.x, currentRenderSettings.scene.camera->right.y, currentRenderSettings.scene.camera->right.z);
	*/
	
	glutPostRedisplay();
}

int lastMouseX = 0;
int lastMouseY = 0;

void processMouseMotion(int x, int y) {
	floatVector yAxis = {0.0f, 1.0f, 0.0f};
	floatVector zAxis = {0.0f, 0.0f, 1.0f};
	
	rotateCameraLocally(currentRenderSettings.scene.camera, zAxis, (float) (x - lastMouseX) * 2.0f);
	rotateCameraLocally(currentRenderSettings.scene.camera, yAxis, (float) (y - lastMouseY) * 2.0f);
	
	lastMouseX = x;
	lastMouseY = y;
	
	glutPostRedisplay();
}

void processMousePassiveMotion(int x, int y) {
	lastMouseX = x;
	lastMouseY = y;
}

/*	Loader for Ken Silverman's simpler voxel format *.VOX, implemented as described in slab6.txt
 *
 */
void loadModel(octree *node, char *filename, intVector offset) {
	FILE *fil;
	fil = fopen(filename, "r");
	
	long xsiz, ysiz, zsiz;
	fread(&xsiz, 4, 1, fil);
	fread(&ysiz, 4, 1, fil);
	fread(&zsiz, 4, 1, fil);
	
	unsigned char voxels[xsiz][ysiz][zsiz];
	fread(voxels, 1, xsiz * ysiz * zsiz, fil);
	
	unsigned char palette[256][3];
	fread(palette, 1, 768, fil);
	
	fclose(fil);
	
	intVector topCoords = {0, 0, 0};
	unsigned char color;
	intVector pos;
	for (pos.z = 0; pos.z < zsiz; pos.z++) {
		for (pos.y = 0; pos.y < ysiz; pos.y++) {
			for (pos.x = 0; pos.x < xsiz; pos.x++) {
				if (voxels[pos.x][pos.y][pos.z] != 255) {
					color = voxels[pos.x][pos.y][pos.z];
					addTerminalOctree(
						addIntVector(intVectorLiteral(pos.x, pos.y, zsiz - pos.z), offset),
						0,
						createVoxel(
							(float) palette[color][0] / 255.0f,
							(float) palette[color][1] / 255.0f,
							(float) palette[color][2] / 255.0f,
							1.0f,
							0
						),
						node,
						topCoords,
						(int (*)(void *)) &deleteVoxel,
						(void *(*)(void *)) &cloneVoxel
					);
				}
			}
		}
	}
}

int main(int argc, char **argv) {
	intVector topCoords = {0, 0, 0};
	intVector pos;
	scene testScene;
	octree *octreeRoot = createOctree(8, createVoxel(0.0f, 0.0f, 0.0f, 0.0f, 1), 0.0f);
	floatVector lightPos = {0.0f, 0.0f, 0.0f};
	light *tempLight;
	float detailBorders[8] = {100.0f, 200.0f, 300.0f, 400.0f, 500.0f, 600.0f, 700.0f, 800.0f};
	for (pos.z = 0; pos.z < 1 << octreeRoot->level; pos.z++) {
		for (pos.y = 0; pos.y < 1 << octreeRoot->level; pos.y++) {
			for (pos.x = 0; pos.x < 1 << octreeRoot->level; pos.x++) {
				/*
				// Cuboid
				if (pos.x > 30 && pos.x < 50 && pos.y > 30 && pos.y < 50 && pos.z < (1 << octreeRoot->level) / 2) {
					addTerminalOctree(pos, 0, createVoxel(1.0f, 1.0f, 0.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Pyramid
				if (pos.x + pos.y + pos.z < 50) {
					addTerminalOctree(pos, 0, createVoxel(1.0f, 1.0f, 1.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Cube
				if ((1 << octreeRoot->level) - pos.x < 20 && (1 << octreeRoot->level) - pos.y < 20 && (1 << octreeRoot->level) - pos.z < 20) {
					addTerminalOctree(pos, 0, createVoxel(0.0f, 1.0f, 1.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}

				// Octahedron
				if (abs((1 << octreeRoot->level) - 20 - pos.x) + abs(pos.y - 20) + abs(pos.z - (1 << octreeRoot->level) / 2) < 20) {
					addTerminalOctree(pos, 0, createVoxel(1.0f, 0.0f, 1.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}

				// Floor
				if ((pos.z == 1) && (pos.x < (1 << octreeRoot->level) / 3 || pos.x > (1 << octreeRoot->level) * 2 / 3)) {
					addTerminalOctree(pos, 0, createVoxel(0.0f, 0.0f, 1.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}

				// Walls
				if ((pos.y == 1 || pos.y == (1 << octreeRoot->level) - 2) && (pos.x % 20 < 10)) {
					addTerminalOctree(pos, 0, createVoxel(0.0f, 1.0f, 0.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}				
				if ((pos.x == 1 || pos.x == (1 << octreeRoot->level) - 2) && (pos.y % 20 < 10)) {
					addTerminalOctree(pos, 0, createVoxel(1.0f, 0.0f, 0.0f, 1.0f), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				*/
				
				// Floor
				if (pos.z == 0) {
					addTerminalOctree(pos, 0, createVoxel(0.2f, 0.2f, 0.2f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Walls
				if (pos.y == 0) {
					addTerminalOctree(pos, 0, createVoxel(0.4f, 0.4f, 0.4f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				if (pos.x == (1 << octreeRoot->level) - 1) {
					addTerminalOctree(pos, 0, createVoxel(0.6f, 0.6f, 0.6f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Cube
				if (pos.x >= 20 && pos.x < 40 && pos.y >= 20 && pos.y < 40 && pos.z >= 40 && pos.z < 60) {
					addTerminalOctree(pos, 0, createVoxel(0.5f, 0.125f, 0.125f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Pyramid
				if (abs((1 << octreeRoot->level) / 2 - pos.x) + abs((1 << octreeRoot->level) / 2 - pos.y) + pos.z < 60) {
					addTerminalOctree(pos, 0, createVoxel(0.125f, 0.5f, 0.125f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Sphere
				if ((pos.x - ((1 << octreeRoot->level) / 2 - 51)) * (pos.x - ((1 << octreeRoot->level) / 2 - 51)) +
					(pos.y - 51) * (pos.y - 51) +
					(pos.z - ((1 << octreeRoot->level) / 2 - 51)) * (pos.z - ((1 << octreeRoot->level) / 2 - 51)) <= 30 * 30) {
					addTerminalOctree(pos, 0, createVoxel(0.125f, 0.125f, 0.5f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Octahedron
				if (abs((1 << octreeRoot->level) - 50 - pos.x) + abs(pos.y - 50) + abs(pos.z - 100) < 40) {
					addTerminalOctree(pos, 0, createVoxel(0.5f, 0.25f, 0.125f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
				
				// Pillar
				if (pos.x >= (1 << octreeRoot->level) - 60 && pos.x < (1 << octreeRoot->level) - 40 &&
					pos.y >= (1 << octreeRoot->level) - 60 && pos.y < (1 << octreeRoot->level) - 40 &&
					pos.z >= 0 && pos.z < 200) {
					addTerminalOctree(pos, 0, createVoxel(0.6f, 0.5f, 0.0f, 1.0f, 1), octreeRoot, topCoords, (int (*)(void *)) &deleteVoxel, (void *(*)(void *)) &cloneVoxel);
				}
			}
		}
	}

	/*
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(0, 0, 0));
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(0, 80, 0));
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(0, 160, 0));
	
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(80, 0, 0));
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(80, 80, 0));
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(80, 160, 0));
	
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(160, 0, 0));
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(160, 80, 0));
	loadModel(octreeRoot, "../models/wtc.vox", intVectorLiteral(160, 160, 0));
	*/
	
	printf("Scene generated. %d terminal nodes.\n", countTerminalOctrees(octreeRoot));
	
	// compressOctreeDownward(octreeRoot, octreeRoot->level, (int (*)(void*, void*)) &equalVoxels, (int (*)(void*)) &deleteVoxel);
	// printf("Scene compressed. %d terminal nodes.\n", countTerminalOctrees(octreeRoot));
	
	/*
	pos.x = 35;
	pos.y = 35;
	pos.z = 63;
	printVoxel((voxel *) findTerminalOctree(pos, 0, octreeRoot, topCoords)->value);
	
	pos.x = 40;
	pos.y = 40;
	pos.z = 40;
	printVoxel((voxel *) findTerminalOctree(pos, 0, octreeRoot, topCoords)->value);
	
	while (1) {}
	*/
	
	updateVoxelAnnotations(octreeRoot, topCoords);
	
	testScene = createScene(octreeRoot, createCamera(50.0f, 1.0f, 1000.0f, 0), 0.25f, 0.25f, 0.25f);
	
	lightPos.x = 100.0f;
	lightPos.y = 100.0f;
	lightPos.z = 100.0f;
	tempLight = createLight(lightPos, 100.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0.0f);
	addLight(&testScene, tempLight);
	
	lightPos.x = 20.0f;
	lightPos.y = 20.0f;
	lightPos.z = 20.0f;
	tempLight = createLight(lightPos, 100.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0, 0.0f);
	addLight(&testScene, tempLight);
	
	lightPos.x = 100.0f;
	lightPos.y = 20.0f;
	lightPos.z = 60.0f;
	tempLight = createLight(lightPos, 100.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0.0f);
	addLight(&testScene, tempLight);

	/*
	lightPos.x = 200.0f;
	lightPos.y = 200.0f;
	lightPos.z = 200.0f;
	tempLight = createLight(lightPos, 200.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1, 0.0f);
	addLight(&testScene, tempLight);
	*/
	
	initializeRendering(testScene, 0, &detailBorders[0]);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(320, 240);
	glutCreateWindow("Voxel (Octree) Rendering");
	glutKeyboardFunc(&processKeys);
	glutSpecialFunc(&processSpecialKeys);
	glutMotionFunc(&processMouseMotion);
	glutPassiveMotionFunc(&processMousePassiveMotion);
	glutDisplayFunc(&renderScene);
	glutIdleFunc(&renderScene);
	glutReshapeFunc(&changeSize);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	glShadeModel(GL_FLAT);
	// glPolygonMode(GL_FRONT, GL_LINE);
	glutMainLoop();
	
	return 0;
}

