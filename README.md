# tolima

Tolima is a from scratch (using no third party libaries such as OpenGL) 3D engine I wrote for educational purposes, mainly to learn C++. It reads [Wavefront .obj files](https://en.wikipedia.org/wiki/Wavefront_.obj_file) in conjuction with [Material Template Library files](https://fr.wikipedia.org/wiki/Material_Template_Library) and renders them with Gouraud shading using the Phong illumination model. The SDL library deals with displaying the scene.
To navigate in the scene, you can use standard FPS controls.

## Dependencies
SDL

## Usage
./tolima file.obj file.mtl

Keyboard navigation in the scene:
- `z` translates forward
- `s` translates backward
- `q` translates left
- `d` translates right
- `a` rotates left
- `e` rotates right
- `w` translates up
- `x` translates down
- `r` toggles wireframe mode

## Known problems
- Since no default shaders are implemented, not specifying the mtl file or having an mtl file with missing Phong illumination parameters (Ks, Kd or Ka) will cause the program to crash
- Poor performance
- Some class implementations should be reworked (mainly `Loader` and `Scene`)