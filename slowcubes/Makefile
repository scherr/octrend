# CFLAGS=-lm -lGL -lglut -O3
# LDFLAGS=-lm -lGL -lglut -O3

CFLAGS=-framework GLUT -framework OpenGL -framework Cocoa -O3
LDFLAGS=-framework GLUT -framework OpenGL -framework Cocoa -O3

all: main

main: linalg.o voxel.o scene.o render.o main.o

*.o: *.c

clean:
	rm -f *.o
	rm -f main
