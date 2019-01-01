all: main

main: main.o myepoll.o sock5.o wrap.o
	cc -Wall -g $^ -o $@

main.o: main.c myepoll.h
myepoll.o: myepoll.c myepoll.h sock5.h wrap.h
sock5.o: sock5.c sock5.h wrap.h
wrap.o: wrap.c wrap.h

clean:
	-rm main *.o *~
