CC = g++ -Wall
STATICLIB = libargvparser.a


CFLAGS= -g $(DEFINES)

SRCS = argvparser.cpp

OBJS = argvparser.o

all: $(STATICLIB)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

$(STATICLIB): $(OBJS)
	$(AR) r $@ $(OBJS)

clean:
	rm -f *.o
	rm -f $(STATICLIB)
