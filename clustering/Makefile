SOURCE    = clustering.cpp clustering_helping.cpp Cluster.cpp dVector.cpp
OBJS      = clustering.o clustering_helping.o Cluster.o dVector.o
HEADER    = Cluster.h dVector.h
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

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS) clustering

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
