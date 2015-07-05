CC=g++
EXE=simple_echo \
	http_demo	\
	buffer_event_demo

CFLAG+=-g
CFLAG+=-levent

all: $(EXE)

clean:
	rm -rf *.o $(EXE)

$(EXE): %: %.cpp
	$(CC) $(CFLAG) -o $@ $^
