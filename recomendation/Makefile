SOURCE    = recomendation.cpp
OBJS      = recomendation.o
HEADER    =
CC        = g++
FLAGS     = -g -c -Wall -std=c++0x
# -g option enables debugging mode
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o recomendation

recomendation.o: recomendation.cpp
	$(CC) $(FLAGS) recomendation.cpp

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS) recomendation

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
