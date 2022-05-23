LIBS=-lGL -lGLEW -lglfw -lSOIL
FILES= main_file.cpp shaderprogram.cpp
HEADERS= shaderprogram.h
main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
