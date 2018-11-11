OBJS      = cluster.o dVector.o
SOURCE    = cluster.cpp dVector.cpp
HEADER    = dVector.h
CC        = g++
FLAGS     = -g -c -Wall -std=c++0x
# -g option enables debugging mode
# -c flag generates object code for separate files



all: $(OBJS)
	$(CC) -g $(OBJS) -o cluster

cluster.o: cluster.cpp
	$(CC) $(FLAGS) cluster.cpp

dVector.o: dVector.cpp
	$(CC) $(FLAGS) dVector.cpp

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS_LSH) $(OBJS_CUBE) lsh cube

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
