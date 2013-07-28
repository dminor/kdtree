INCS = 
LIBS = -lann
CFLAGS = -g -O2
LDFLAGS = -g
OBJS = knn.o
TARGET = ../../bin/ann-knn-query

all: $(OBJS)
	g++ $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

.cpp.o:
	g++ $(INCS) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET) 
