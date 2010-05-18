
INCS = -I ../include
LIBS = 
CFLAGS = -std=gnu99 -fpic
OBJS = kdtree.o
BINDIR = ../bin
TARGET = libkdtree.so

all: $(OBJS)
	gcc $(LIBS) $(FLAGS) -shared -Wl,-soname,$(TARGET) $(OBJS) -o $(BINDIR)/$(TARGET)

.c.o:
	gcc $(INCS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(BINDIR)/$(TARGET) 
