all:
	g++ -I src/include -L src/lib -o salsa_platformer.exe main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	./main