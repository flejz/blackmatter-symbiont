.PHONY: dev 

build:
	g++ *.cpp include/*.cpp -omain.o -lglfw3 -lGL -lpthread -ldl -lGLEW

run:
	./main.o

dev:
	reflex -vsr "\.[cpp|h|glsl|fs|vs]" -- bash -c "make build && make run"
