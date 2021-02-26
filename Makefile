run: metro
	./metro 

metro: metro.c
	gcc -O2 -Wall -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT "metro.c" -o "metro" -luvsqgraphics -L/usr/lib/x86_64-linux-gnu -lSDL -lm -lSDL_ttf

clean: 
	rm -f metro
	ls -l
