LIBS=-lGL -lGLEW -lglfw
FILES= main_file.cpp
main_file: $(FILES)
	g++ -o main_file $(FILES)  $(LIBS) -I.
