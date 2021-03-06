CC=gcc

old_main: old_main.c animation.o display.o music.o player.o
	$(CC) old_main.c animation.o display.o music.o player.o -o old_main $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_acodec-5 allegro_audio-5 --libs --cflags)


allegro_test:
	$(CC) allegro_test.c -o allegro_test $(pkg-config allegro-5 allegro_font-5 --libs --cflags) 

main: main.c animation.o display.o music.o player.o
	$(CC) main.c animation.o display.o music.o player.o -o main $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_acodec-5 allegro_audio-5 --libs --cflags)

animation.o: animation.c animation.h
	$(CC) -c animation.c -o animation.o 

display.o: display.c display.h
	$(CC) -c display.c -o display.o $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 --libs --cflags)

music.o: music.c music.h
	$(CC) -c music.c -o music.o $(shell pkg-config allegro-5 allegro_acodec-5 allegro_audio-5 --libs --cflags)

player.o: player.c player.h
	$(CC) -c player.c -o player.o $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 --libs --cflags)

clean:
	rm -rf *.o