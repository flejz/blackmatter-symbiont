.PHONY: dev 

build:
	g++ *.cpp -omain.o -lglfw3 -lGL -lpthread -ldl -lGLEW
	# g++ *.cpp -omain.o -lglfw3 -lGL -lpthread -ldl -lGLEW -Wall

run:
	./main.o

dev:
	reflex -r "\.[cpp|h|glsl]" -s -- bash -c "make build && make run"
