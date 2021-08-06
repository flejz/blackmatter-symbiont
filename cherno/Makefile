.PHONY: build

build:
	g++ main.cpp -o main.o -lglfw3 -lGL -lpthread -ldl -lGLEW

run:
	./main.o

dev:
	reflex -r "\.[cpp|h]" -s -- bash -c "make build && make run"
