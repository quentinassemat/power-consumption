CC=g++

all: seasonality_detection

.PHONY: all clean

point.o: ../cloud/point.cpp ../cloud/point.hpp
	$(CC) -c ../cloud/point.cpp

cloud.o: ../cloud/cloud.cpp ../cloud/point.hpp ../cloud/cloud.hpp ../cloud/distribution.hpp
	$(CC) -c ../cloud/cloud.cpp

distribution.o: ../cloud/distribution.cpp ../cloud/distribution.hpp
	$(CC) -c ../cloud/distribution.cpp

# différents fonctions de test de notre programme
test.o: test.cpp test.hpp ../cloud/point.hpp ../cloud/cloud.hpp
	$(CC) -c test.cpp

seasonality_detection: seasonality_detection.cpp point.o cloud.o distribution.o test.o
	$(CC) -o seasonality_detection seasonality_detection.cpp point.o cloud.o distribution.o test.o

clean:
	rm -vf seasonality_detection *.o *~
