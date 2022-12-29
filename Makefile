CC=gcc
EXE=jsnake.app
LFLAGS=-lcurses

$(EXE):
	$(CC) ./src/main.c $(LFLAGS) -o $(EXE)

clean:
	rm -rf $(EXE)
