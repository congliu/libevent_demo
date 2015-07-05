CC=g++
EXE=simple_echo
CFLAG+=-levent

all: $(EXE)

clean:
	rm -rf *.o $(EXE)

$(EXE): %: %.cpp
	$(CC) $(CFLAG) -o $@ $^
