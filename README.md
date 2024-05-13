for linux:

gcc final.c init.c imporation.c play.c mainmenu.c option.c  -lSDL2 -lSDL2_image -lSDL2_ttf 




for windows:

gcc final.c init.c imporation.c play.c mainmenu.c option.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
