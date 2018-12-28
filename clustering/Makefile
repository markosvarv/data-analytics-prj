SOURCE    = clustering.cpp clustering_helping.cpp Cluster.cpp dVector.cpp hFunction.cpp lsh.cpp cube.cpp init.cpp assignment.cpp
OBJS      = clustering.o clustering_helping.o Cluster.o dVector.o hFunction.o lsh.o cube.o init.o assignment.o
HEADER    = clustering_helping.h Cluster.h dVector.h hFunction.h lsh.h cube.h init.h assignment.h
CC        = g++
FLAGS     = -g -c -Wall -std=c++0x
# -g option enables debugging mode
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o clustering

clustering.o: clustering.cpp
	$(CC) $(FLAGS) clustering.cpp

clustering_helping.o: clustering_helping.cpp
	$(CC) $(FLAGS) clustering_helping.cpp

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp

dVector.o: dVector.cpp
	$(CC) $(FLAGS) dVector.cpp

hFunction.o: hFunction.cpp
	$(CC) $(FLAGS) hFunction.cpp

lsh.o: lsh.cpp
	$(CC) $(FLAGS) lsh.cpp

cube.o: cube.cpp
	$(CC) $(FLAGS) cube.cpp

init.o: init.cpp
	$(CC) $(FLAGS) init.cpp

assignment.o: assignment.cpp
	$(CC) $(FLAGS) assignment.cpp

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS) clustering

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
