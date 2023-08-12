
save:
	git add .
	git commit -m "$(message)"
	git push -u origin

all:
g++ -I src/include -L src/lib -o build/taco_platformer.exe main.cpp game.cpp entities/*.cpp utils/map/*.cpp utilities.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer