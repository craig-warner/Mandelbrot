mandel:	movie.h movie.c mandel.h mandel.c bmp.c bmp.h bmp_wr.c
	gcc -g -c bmp.c 
	gcc -g -c bmp_wr.c 
	gcc -g -c -O3 mandel.c
	gcc -g -c movie.c
	gcc bmp.o bmp_wr.o mandel.o movie.o -lpthread -o mandel

clean:
	rm *.o mandel

run:
	bsub -G svl_o2ps -C 16 -J "Mandel Test" mandel
        # kill with bkill	
