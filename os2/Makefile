BIN=	conc

SRCS=	tester.c concurrent_queue.c
OBJS=	tester.o concurrent_queue.o

CC=	gcc
CFLAGS= -g -pthread

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -pthread -c $< 

all:	$(BIN)

conc:	$(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

clean:
	-rm -f $(OBJS) lsh

#gcc -o conc -include concurrent_queue.c tester.c 
