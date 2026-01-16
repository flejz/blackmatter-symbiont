# Blackmatter Symbiont

An OpenGL 3D visualization project featuring animated spheres with dynamic lighting.

## Dependencies

- GLFW3
- GLEW
- GLM
- Assimp

On Arch/Manjaro:
```bash
sudo pacman -S glfw glew glm assimp
```

On Ubuntu/Debian:
```bash
sudo apt install libglfw3-dev libglew-dev libglm-dev libassimp-dev
```

## Building

Using CMake:
```bash
mkdir build && cd build
cmake ..
make
```

Or manually with g++:
```bash
g++ -o main ./main.cpp ./include/stb_image.cpp $(pkg-config --libs glfw3 assimp glew)
```

## Running

```bash
./build/blackmatter-symbiont
```

Controls:
- WASD - Move camera
- Mouse - Look around
- Scroll - Zoom
- ESC - Exit

---

## Resources

https://docs.gl
https://learnopengl.com/

past:
https://learnopengl.com/Getting-started/Hello-Triangle
* Vertex input
* Element Buffer Objects

https://learnopengl.com/Getting-started/Shaders
* layout, in and out variables
* vecn objects

https://learnopengl.com/Getting-started/Textures
* texture loading and orientation
* texture units to load several textures to the fragment shader

https://learnopengl.com/Getting-started/Transformations

current:
https://learnopengl.com/Lighting/Basic-Lighting

### assets

https://github.com/JoeyDeVries/LearnOpenGL
https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/sphere.cpp

### sphere

https://www.songho.ca/opengl/gl_sphere.html
https://sketchfab.com/3d-models/sphere-b31b12ffa93a40f48c9d991b6f168f4d
https://sketchfab.com/3d-models/sphere-design-79d172a4508c4e26b39660e07b90bb13
https://sketchfab.com/3d-models/carbon-60-sphere-5ef45e632eb8431998400ea73ddc51a5
https://sketchfab.com/3d-models/scifi-hexsphere-cb364832b9994b768dba6245e6b3f51b
https://sketchfab.com/3d-models/geodesic-1da7711c55a64c6ea55d3499822d6ac2

### c meanings
https://cdecl.org/
