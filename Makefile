triangle:
	g++ triangle.cpp -lglut -lGL -o triangle.out
	./triangle.out
	
cube:
	g++ cube.cpp -lglut -lGL -o cube.out
	./cube.out

rotatingCube:
	g++ rotatingCube.cpp -lglut -lGL -lGLU -o rotatingCube.out
	./rotatingCube.out
