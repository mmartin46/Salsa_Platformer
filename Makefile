all:
	g++ -I src/include -L src/lib -o taco_platformer.exe main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	./taco_platformer.exe