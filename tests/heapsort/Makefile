
INCS = -I../../src
LIBS = -lkdtree
CFLAGS = -std=gnu99
LDFLAGS = -L../../bin 
OBJS = heapsort.o
TARGET = ../../bin/heapsort

all: $(OBJS)
	gcc $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.c.o:
	gcc $(INCS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET) 
