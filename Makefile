.PHONY: dev 

build:
	g++ main.cpp shader.cpp -o main.o -lglfw3 -lGL -lpthread -ldl -lGLEW

run:
	./main.o

dev:
	reflex -r "\.[cpp|h|glsl]" -s -- bash -c "make build && make run"
