SOURCE    = recomendation.cpp recomendation_helping.cpp Tweet.cpp clustering.cpp Cluster.cpp dVector.cpp
OBJS      = recomendation.o recomendation_helping.o Tweet.o clustering.o Cluster.o dVector.o
HEADER    = recomendation_helping.h Tweet.h clustering.h Cluster.h dVector.h
CC        = g++
FLAGS     = -g -c -Wall -std=c++0x
# -g option enables debugging mode
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o recomendation

recomendation.o: recomendation.cpp
	$(CC) $(FLAGS) recomendation.cpp
recomendation_helping.o: recomendation_helping.cpp
	$(CC) $(FLAGS) recomendation_helping.cpp
dVector.o: dVector.cpp
	$(CC) $(FLAGS) dVector.cpp
Tweet.o: Tweet.cpp
	$(CC) $(FLAGS) Tweet.cpp
clustering.o: clustering.cpp
	$(CC) $(FLAGS) clustering.cpp
Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS) recomendation

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
