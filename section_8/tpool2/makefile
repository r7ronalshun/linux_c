SRCS=$(wildcard *.c)

OBJS=$(SRCS:.c=.o)

CC=gcc

INCLUDES=-I/

LIBS=-L/ -lpthread

CCFLAGS = -g -Wall -O0

cThreadPool : $(OBJS)

    $(CC) $^ -o $@ $(INCLUDES) $(LIBS)

%.o : %.cpp

    $(CC) -c $<$(CCFLAGS)


clean:

    rm *.o

    .PHONY:clean
