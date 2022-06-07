LIBS=-lGL -lGLEW -lglfw -lSOIL -lassimp
FILES= main_file.cpp shaderprogram.cpp map.cpp fence.cpp column.cpp cube.cpp objects.cpp 
HEADERS= shaderprogram.h map.h fence.h column.h Camera.h objects.h cube.h 
main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES) $(LIBS) -I.
