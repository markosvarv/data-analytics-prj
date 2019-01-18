OBJS_LSH  = lsh.o lsh_helping.o dVector.o hFunction.o mutual_helping.o
ΟΒJS_CUBE = cube.o cube_helping.o dVector.o hFunction.o mutual_helping.o
SOURCE    = lsh.cpp cube.cpp lsh_helping.cpp cube_helping.cpp dVector.cpp hFunction.cpp mutual_helping.cpp
HEADER    = lsh_helping.h cube_helping.h dVector.h hFunction.h mutual_helping.h
CC        = g++
FLAGS     = -g -c -Wall -std=c++0x
# -g option enables debugging mode
# -c flag generates object code for separate files

all: lsh cube

lsh: $(OBJS_LSH)
	$(CC) -g $(OBJS_LSH) -o lsh

cube: cube.o cube_helping.o dVector.o hFunction.o mutual_helping.o
	$(CC) -g cube.o cube_helping.o dVector.o hFunction.o mutual_helping.o -o cube

cube.o: cube.cpp
	$(CC) $(FLAGS) cube.cpp

mutual_helping.o: mutual_helping.cpp
	$(CC) $(FLAGS) mutual_helping.cpp

lsh.o: lsh.cpp
	$(CC) $(FLAGS) lsh.cpp

lsh_helping.o: lsh_helping.cpp
	$(CC) $(FLAGS) lsh_helping.cpp

cube_helping.o: cube_helping.cpp
	$(CC) $(FLAGS) cube_helping.cpp

dVector.o: dVector.cpp
	$(CC) $(FLAGS) dVector.cpp

hFunction.o: hFunction.cpp
	$(CC) $(FLAGS) hFunction.cpp

# create/compile the individual files >>separately<<

# clean house
clean:
	rm -f $(OBJS_LSH) $(OBJS_CUBE) lsh cube

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
