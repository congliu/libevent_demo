CC=g++
EXE=simple_echo 			\
	http_demo				\
	buffer_event_old_demo	\
	buffer_event_demo


CFLAG+=-g
CFLAG+=-levent

all: $(EXE)

clean:
	rm -rf *.o *.dSYM $(EXE)

$(EXE): %: %.cpp
	$(CC) $(CFLAG) -o $@ $^
