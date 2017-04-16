# Author: Mosab Bazargani
# Date: 2016
# Last modified: Nov/2016
#

CC = g++
#FLAG = -c -Wall
#FLAG = -c
FLAG = -c -std=c++11
LFLAG = -lm

MAIN = Configuration.o TSPLIB.o LAHC.o Util.o Random.o TSP_mutation.o Statistic.o pLAHC.o pLAHC_s.o main.o

main: $(MAIN)
	$(CC) $(LFLAG) -o pLAHC $(MAIN)
	-rm -f *.o

#---------------------------------

main.o: src/main.cpp src/Configuration.h src/TSPLIB.h src/LAHC.h src/Util.h src/Random.h src/Statistic.h src/pLAHC.h src/pLAHC_s.h
	$(CC) $(FLAG) src/main.cpp

#---------------------------------

Configuration.o: src/Configuration.cpp src/Configuration.h src/Util.h
	$(CC) $(FLAG) src/Configuration.cpp

TSPLIB.o: src/TSPLIB.cpp src/TSPLIB.h src/Util.h
	$(CC) $(FLAG) src/TSPLIB.cpp

LAHC.o: src/LAHC.cpp src/LAHC.h src/Util.h src/TSPLIB.h src/Configuration.h src/Random.h src/TSP_mutation.h
	$(CC) $(FLAG) src/LAHC.cpp

Util.o: src/Util.cpp src/Util.h src/Random.h
	$(CC) $(FLAG) src/Util.cpp

Random.o: src/Random.cpp src/Random.h
	$(CC) $(FLAG) src/Random.cpp

TSP_mutation.o: src/TSP_mutation.cpp src/TSP_mutation.h src/Random.h src/Util.h
	$(CC) $(FLAG) src/TSP_mutation.cpp
	
Statistic.o: src/Statistic.cpp src/Util.h
	$(CC) $(FLAG) src/Statistic.cpp

pLAHC.o: src/pLAHC.cpp src/pLAHC.h
	$(CC) $(FLAG) src/pLAHC.cpp

pLAHC_s.o: src/pLAHC_s.cpp src/pLAHC_s.h
	$(CC) $(FLAG) src/pLAHC_s.cpp
	
clean:
	-rm -f *.o
