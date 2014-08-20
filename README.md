OctRend
===

Maximilian Scherr, 2006-

I was once interested in voxel engines and to practice some C as well as OpenGL (both of which I have mostly forgotten or has fallen into deprecation) I wrote a few experiments on how a really naive volume rendering of voxels as cubes would fare. Level of detail is used to speed up and very trivially implementable thanks to the octree data structure used.

This comes right out of the attic (from 2009 with octree.c from 2006) and is filled with bugs and cobwebs.


Prototype (./protoype)
---

This implementations uses direct mode and is properly the best documented one.

The example scene:

![The example scene](/screenshot0.png?raw=true)

Farther away:

![Farther away](/screenshot1.png?raw=true)

Closer:

![Closer](/screenshot2.png?raw=true)

Light with shadows (very slow!):

![Light with shadows (very slow!)](/screenshot3.png?raw=true)

Another shadow:

![Another shadow](/screenshot4.png?raw=true)


Slow Cubes, Fast Cubes, Shader (./slowcubes, ./fastcubes, ./shader)
---

These implementations are somewhat simpler and use vertex lists instead of direct mode.
The difference between slow and fast cubes is the latter's use of lookup tables.

The shader implementation uses flat squares instead of cubes for rendering. Sadly,
I do not have the appropriate hardware (anymore) to test this implementation or show screenshots of it.

A voxel landscape (by ???):

![A voxel landscape (by ???)](/screenshot5.png?raw=true)

More detail:

![More detail](/screenshot6.png?raw=true)

Wild voxel models (by [Ken Silverman](http://advsys.net/ken/)) appear:

![Wild voxel models (by Ken Silverman) appear](/screenshot7.png?raw=true)

More detail:

![More detail](/screenshot8.png?raw=true)


Data
---

Model data is taken from Ken Sivlerman's [SLAB6](http://advsys.net/ken/download.htm#slab6). I cannot remember the origin of the height and texture map data and would be happy about pointers for proper attribution.
