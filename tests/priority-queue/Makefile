
INCS = -I../../include 
LIBS = 
CFLAGS = -g -O2
LDFLAGS = -L../../bin 
OBJS = main.o
TARGET = ../../bin/priority-queue

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

main.o: ../../../kdtree/include/fixed_size_priority_queue.h

clean:
	rm *.o $(TARGET) 
