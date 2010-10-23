
INCS = -I../../include 
LIBS = 
CFLAGS = 
LDFLAGS = 
OBJS = range_query.o
TARGET = ../../bin/range-query

all: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) $(OBJS) -o $(TARGET) 

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET) 
