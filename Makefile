CC = gcc
CFLAG = -Wall -Wextra -ltask_helper -L ./util
EXE = text
OBJ = text.o
DEPS = util/compare.h

build: $(EXE)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAG)

$(EXE): $(OBJ)
	$(CC) $^ $(CFLAG) -o $(EXE)

run:
	./text

.PHONY: clean

clean:
	rm -f *.o $(EXE)
