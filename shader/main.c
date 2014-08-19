#include "render.h"

static RenderEnv *renderEnv;

void loadMap(Voxel *voxel, float size, float atDepth, char *heightMapFilename, char *texMapFilename, Vector offset) {
	FILE *fil;
	
	fil = fopen(heightMapFilename, "r");
	
	unsigned int width;
	unsigned int height;
	
	fseek(fil, 18, SEEK_SET);
	fread(&width, 4, 1, fil);
	fseek(fil, 22, SEEK_SET);
	fread(&height, 4, 1, fil);
	
	printf("Loading heightmap \"%s\" (%dx%d).\n", heightMapFilename, width, height);
	
	unsigned char heightPixels[width][height][3];
	
	fseek(fil, 54, SEEK_SET);
	fread(heightPixels, 3, width * height, fil);
	
	fclose(fil);
	
	
	fil = fopen(texMapFilename, "r");
	
	fseek(fil, 18, SEEK_SET);
	fread(&width, 4, 1, fil);
	fseek(fil, 22, SEEK_SET);
	fread(&height, 4, 1, fil);
	
	printf("Loading texturemap \"%s\" (%dx%d).\n", texMapFilename, width, height);
	
	unsigned char texPixels[width][height][3];
	
	fseek(fil, 54, SEEK_SET);
	fread(texPixels, 3, width * height, fil);
	
	fclose(fil);
	
	
	int x;
	int y;
	int minZ;
	int i;
	int j;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			minZ = heightPixels[x][y][0];
			for (i = -1; i <= 1; i++) {
				for (j = -1; j <= 1; j++) {
					if (
						x + i > -1 &&
						x + i < width &&
						y + j > -1 &&
						y + j < height &&
						heightPixels[x + i][y + j][0] < minZ
					) {
						minZ = heightPixels[x + i][y + j][0];
					}
				}
			}
			
			for (i = minZ; i <= heightPixels[x][y][0]; i++) {
				voxelAdd(
					voxel,
					size,
					voxelCreate(
						(float) texPixels[x][y][2] / 255.0f,
						(float) texPixels[x][y][1] / 255.0f,
						(float) texPixels[x][y][0] / 255.0f
					),
					vectorAdd(vectorLiteral(x * size / pow(2, atDepth), y * size / pow(2, atDepth), i * size / pow(2, atDepth)), offset),
					atDepth
				);
			}
		}
	}
}

/*	Loader for Ken Silverman's simpler voxel format *.VOX, implemented as described in slab6.txt
 *
 */
void loadModel(Voxel *voxel, float size, float atDepth, char *filename, Vector offset) {
	FILE *fil;
	fil = fopen(filename, "r");
	
	int xsiz, ysiz, zsiz;
	fread(&xsiz, 4, 1, fil);
	fread(&ysiz, 4, 1, fil);
	fread(&zsiz, 4, 1, fil);
	
	unsigned char voxels[xsiz][ysiz][zsiz];
	fread(voxels, 1, xsiz * ysiz * zsiz, fil);
	
	unsigned char palette[256][3];
	fread(palette, 1, 768, fil);
	
	fclose(fil);
	
	unsigned char color;
	int x;
	int y;
	int z;
	float red;
	float green;
	float blue;
	for (z = 0; z < zsiz; z++) {
		for (y = 0; y < ysiz; y++) {
			for (x = 0; x < xsiz; x++) {
				if (voxels[x][y][z] != 255) {
					color = voxels[x][y][z];
					red = (float) palette[color][0] / 255.0f;
					green = (float) palette[color][1] / 255.0f;
					blue = (float) palette[color][2] / 255.0f;
					
					/*
					if (red == 0 && green == 0 && blue == 0) {
						green = 255;
					}
					*/
					
					voxelAdd(
						voxel,
						size,
						voxelCreate(red, green, blue),
						vectorAdd(vectorLiteral(x * size / pow(2, atDepth), y * size / pow(2, atDepth), (zsiz - z) * size / pow(2, atDepth)), offset),
						atDepth
					);
				}
			}
		}
	}
}

void processKeys(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 0.0f, 1.0f), -2.0f);
			break;
			
		case 'd':
			cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 0.0f, 1.0f), 2.0f);
			break;
			
		case 'w':
			cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 1.0f, 0.0f), 2.0f);
			break;
			
		case 's':
			cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 1.0f, 0.0f), -2.0f);
			break;
			
		case 'q':
			cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(1.0f, 0.0f, 0.0f), 2.0f);
			break;
			
		case 'e':
			cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(1.0f, 0.0f, 0.0f), -2.0f);
			break;
		
		case '+':
			renderEnv->scene.camera.zoom += 0.1f;
			renderUpdate();
			break;
			
		case '-':
			renderEnv->scene.camera.zoom -= 0.1f;
			renderUpdate();
			break;
			
		case '1':
			loadModel(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../models/cac.vox",
				vectorAdd(renderEnv->scene.camera.position, vectorScale(renderEnv->scene.camera.forward, 10.0f)));
			break;
			
		case '2':
			loadModel(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../models/desklamp.vox",
				vectorAdd(renderEnv->scene.camera.position, vectorScale(renderEnv->scene.camera.forward, 10.0f)));
			break;
			
		case '3':
			loadModel(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../models/globe.vox",
				vectorAdd(renderEnv->scene.camera.position, vectorScale(renderEnv->scene.camera.forward, 10.0f)));
			break;
			
		case '4':
			loadModel(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../models/spring.vox",
				vectorAdd(renderEnv->scene.camera.position, vectorScale(renderEnv->scene.camera.forward, 10.0f)));
			break;
			
		case '5':
			loadModel(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../models/wtc.vox",
				vectorAdd(renderEnv->scene.camera.position, vectorScale(renderEnv->scene.camera.forward, 10.0f)));
			break;
	}
	
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			cameraTranslateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, -2.0f, 0.0f));
			break;
			
		case GLUT_KEY_RIGHT:
			cameraTranslateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 2.0f, 0.0f));
			break;
			
		case GLUT_KEY_UP:
			cameraTranslateLocally(&renderEnv->scene.camera, vectorLiteral(2.0f, 0.0f, 0.0f));
			break;
			
		case GLUT_KEY_DOWN:
			cameraTranslateLocally(&renderEnv->scene.camera, vectorLiteral(-2.0f, 0.0f, 0.0f));
			break;
			
		case GLUT_KEY_PAGE_UP:
			cameraTranslateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 0.0f, 2.0f));
			break;
			
		case GLUT_KEY_PAGE_DOWN:
			cameraTranslateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 0.0f, -2.0f));
			break;
	}
	glutPostRedisplay();
}

static int lastMouseX = 0;
static int lastMouseY = 0;

void processMouseMotion(int x, int y) {
	cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 0.0f, 1.0f), (float) (x - lastMouseX) * 2.0f);
	cameraRotateLocally(&renderEnv->scene.camera, vectorLiteral(0.0f, 1.0f, 0.0f), (float) (y - lastMouseY) * 2.0f);
	
	lastMouseX = x;
	lastMouseY = y;
	
	glutPostRedisplay();
}

void processMousePassiveMotion(int x, int y) {
	lastMouseX = x;
	lastMouseY = y;
}

int main(int argc, char **argv) {
	/*
	Voxel *voxel = voxelCreate(0.0f, 0.0f, 0.0f);
	voxelPrint(voxel, "");
	
	voxelSetRed(voxel, 0.9f);
	voxelSetGreen(voxel, 0.6f);
	voxelSetBlue(voxel, 0.3f);
	
	voxelAddChild(voxel, voxelCreate(1.0f, 0.0f, 0.0f), 2);
	voxelPrint(voxel, "");
	
	voxelAddChild(voxel, voxelCreate(0.0f, 0.0f, 1.0f), 1);
	voxelPrint(voxel, "");
	
	Voxel *tempVox = voxelCreate(1.0f, 1.0f, 1.0f);
	voxelAdd(voxel, 3.0f, tempVox, vectorLiteral(1.6f, 1.6f, 0.0f), 2);
	voxelAdd(voxel, 3.0f, voxelCreate(1.0f, 0.0f, 1.0f), vectorLiteral(1.6f, 2.5f, 0.0f), 2);
	voxelPrint(voxel, "");
	printf("%d\n", (int) voxelGetNeighbour(tempVox, vectorLiteral(0.0f, 1.0f, 0.0f)));
	*/
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Voxel (Octree) Rendering");
	
	renderEnv = renderInit(
		sceneLiteral(
			voxelCreate(0.0f, 0.0f, 0.0f),
			512.0f,
			cameraLiteral(
				vectorLiteral(0.0f, 0.0f, 0.0f),
				vectorLiteral(1.0f, 0.0f, 0.0f),
				vectorLiteral(0.0f, 1.0f, 0.0f),
				vectorLiteral(0.0f, 0.0f, 1.0f),
				1.0f,
				45.0f,
				1.0f,
				1000.0f
			)
		),
		9,
		0.25f,
		0.5f
	);
	
	// loadModel(renderEnv->scene.root, renderEnv->scene.rootSize, 10, "../models/spring.vox", vectorLiteral(0.0f, 0.0f, 0.0f));
	// loadMap(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../maps/heightmap.bmp", "../maps/texmap.bmp", vectorLiteral(0.0f, 0.0f, 0.0f));
	// loadMap(renderEnv->scene.root, renderEnv->scene.rootSize, 9, "../maps/heightmap.bmp", "../maps/texmap.bmp", vectorLiteral(0.0f, 0.0f, 256.0f));
	// loadMap(renderEnv->scene.root, renderEnv->scene.rootSize, 10, "../maps/heightmapbig.bmp", "../maps/texmapbig.bmp", vectorLiteral(0.0f, 0.0f, 0.0f));
	// loadMap(renderEnv->scene.root, renderEnv->scene.rootSize, 8, "../maps/heightmapsmall.bmp", "../maps/texmapsmall.bmp", vectorLiteral(0.0f, 0.0f, 0.0f));
	// loadMap(renderEnv->scene.root, renderEnv->scene.rootSize, 8, "../maps/heightmapsmall.bmp", "../maps/texmapsmall.bmp", vectorLiteral(0.0f, 0.0f, 256.0f / 3.0f));
	// loadMap(renderEnv->scene.root, renderEnv->scene.rootSize, 8, "../maps/heightmapsmall.bmp", "../maps/texmapsmall.bmp", vectorLiteral(0.0f, 0.0f, 256.0f * 2.0f / 3.0f));
	
	Vector pos;
	for (pos.z = 0; pos.z < renderEnv->scene.rootSize; pos.z++) {
		for (pos.y = 0; pos.y < renderEnv->scene.rootSize; pos.y++) {
			for (pos.x = 0; pos.x < renderEnv->scene.rootSize; pos.x++) {
				if (pos.y == 0 && pos.z == 0) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.y == renderEnv->scene.rootSize - 1.0f && pos.z == 0) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.y == 0 && pos.z == renderEnv->scene.rootSize - 1.0f) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.y == renderEnv->scene.rootSize - 1.0f && pos.z == renderEnv->scene.rootSize - 1.0f) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == 0 && pos.z == 0) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == renderEnv->scene.rootSize - 1.0f && pos.z == 0) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == 0 && pos.z == renderEnv->scene.rootSize - 1.0f) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == renderEnv->scene.rootSize - 1.0f && pos.z == renderEnv->scene.rootSize - 1.0f) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == 0 && pos.y == 0) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == renderEnv->scene.rootSize - 1.0f && pos.y == 0) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == 0 && pos.y == renderEnv->scene.rootSize - 1.0f) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
				if (pos.x == renderEnv->scene.rootSize - 1.0f && pos.y == renderEnv->scene.rootSize - 1.0f) {
					voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
			
				// Floor
				if (pos.z == 0) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f), pos, 9);
				}
				
				// Walls
				if (pos.y == 0) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(30.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f), pos, 9);
				}
				if (pos.x == renderEnv->scene.rootSize - 1) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(40.0f / 255.0f, 40.0f / 255.0f, 40.0f / 255.0f), pos, 9);
				}
				
				// Cube
				if (pos.x >= 20 && pos.x < 40 && pos.y >= 20 && pos.y < 40 && pos.z >= 40 && pos.z < 60) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(120.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f), pos, 9);
				}
				
				// Pyramid
				if (abs(renderEnv->scene.rootSize / 2 - pos.x) + abs(renderEnv->scene.rootSize / 2 - pos.y) + pos.z < 60) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(20.0f / 255.0f, 120.0f / 255.0f, 20.0f / 255.0f), pos, 9);
				}
				
				// Sphere
				if ((pos.x - (renderEnv->scene.rootSize / 2 - 51)) * (pos.x - (renderEnv->scene.rootSize / 2 - 51)) +
					(pos.y - 51) * (pos.y - 51) +
					(pos.z - (renderEnv->scene.rootSize / 2 - 51)) * (pos.z - (renderEnv->scene.rootSize / 2 - 51)) <= 30 * 30) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(20.0f / 255.0f, 20.0f / 255.0f, 120.0f / 255.0f), pos, 9);
				}
				
				// Octahedron
				if (abs(renderEnv->scene.rootSize - 50 - pos.x) + abs(pos.y - 50) + abs(pos.z - 100) < 40) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(120.0f / 255.0f, 40.0f / 255.0f, 20.0f / 255.0f), pos, 9);
				}
				
				// Pillar
				if (pos.x >= renderEnv->scene.rootSize - 60 && pos.x < renderEnv->scene.rootSize - 40 &&
					pos.y >= renderEnv->scene.rootSize - 60 && pos.y < renderEnv->scene.rootSize - 40 &&
					pos.z >= 0 && pos.z < 200) {
					// voxelAdd(renderEnv->scene.root, renderEnv->scene.rootSize, voxelCreate(120.0f / 255.0f, 120.0f / 255.0f, 0.0f / 255.0f), pos, 9);
				}
			}
		}
	}
	
	glutKeyboardFunc(&processKeys);
	glutSpecialFunc(&processSpecialKeys);
	glutMotionFunc(&processMouseMotion);
	glutPassiveMotionFunc(&processMousePassiveMotion);
	glutDisplayFunc(&renderDisplay);
	glutIdleFunc(&renderDisplay);
	glutReshapeFunc(&renderReshape);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// glEnable(GL_LIGHTING);
	// glEnable(GL_COLOR_MATERIAL);
	glutMainLoop();
}
