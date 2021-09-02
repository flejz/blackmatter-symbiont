.PHONY: normalize dev 

build:
	g++ *.cpp include/*.cpp -omain.o -lglfw3 -lGL -lpthread -ldl -lGLEW -lassimp

run: build
	./main.o

dev: normalize
	reflex -sr "\.[cpp|h|glsl|fs|vs]" -- bash -c "make run"

normalize:
	sed -i "s/glad\/glad\.h/GL\/glew\.h/" main.cpp
	sed -i "s/<stb_image\.h>/\"include\/stb_image\.h\"/" main.cpp
	sed -i "s/<learnopengl\/filesystem\.h>/\"include\/filesystem\.h\"/" main.cpp
	sed -i "s/<learnopengl\/shader_m\.h>/\"include\/shader_m\.h\"/" main.cpp
	sed -i "s/<learnopengl\/camera\.h>/\"include\/camera\.h\"/" main.cpp
	sed -i "s/!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)/glewInit() != GLEW_OK/" main.cpp
