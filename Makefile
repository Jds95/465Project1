SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

# Added the linkers to the LDFLAGS
SDL_LDFLAGS += -lSDL2_net
SDL_LDFLAGS += -lSDL2_ttf
SDL_LDFLAGS += -lSDL2_image

main: main.o
	g++  main.o -o main $(SDL_LDFLAGS)

main.o: main.cpp
	g++ -std=c++11 -c -Iincludes $(SDL_CFLAGS) main.cpp

run:
	./main


client:
	./a.out localhost


clean:	
	rm -f  main main.o
