run-triangle:
	g++ triangle.cpp -lglut -lGL -o triangle
	./triangle
	
run-cube:
	g++ cube.cpp -lglut -lGL -o cube
	./cube

run-rotatingCube:
	g++ rotatingCube.cpp -lglut -lGL -lGLU -o rotatingCube
	./rotatingCube
