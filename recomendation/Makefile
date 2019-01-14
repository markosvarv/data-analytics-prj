SOURCE    = recommendation.cpp recommendation_helping.cpp Tweet.cpp clustering.cpp Cluster.cpp lsh.cpp hFunction.cpp dVector.cpp
OBJS      = recommendation.o recommendation_helping.o Tweet.o clustering.o Cluster.o lsh.o hFunction.o dVector.o
HEADER    = recommendation_helping.h Tweet.h clustering.h Cluster.h lsh.h hFunction.h dVector.h
CC        = g++
FLAGS     = -g -c -Wall -std=c++0x
# -g option enables debugging mode
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o recomendation

recommendation.o: recommendation.cpp
	$(CC) $(FLAGS) recommendation.cpp
recommendation_helping.o: recommendation_helping.cpp
	$(CC) $(FLAGS) recommendation_helping.cpp
Tweet.o: Tweet.cpp
	$(CC) $(FLAGS) Tweet.cpp
clustering.o: clustering.cpp
	$(CC) $(FLAGS) clustering.cpp
Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp
lsh.o: lsh.cpp
	$(CC) $(FLAGS) lsh.cpp
hFunction.o: hFunction.cpp
	$(CC) $(FLAGS) hFunction.cpp
dVector.o: dVector.cpp
	$(CC) $(FLAGS) dVector.cpp

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS) recomendation

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
