CC=g++

all: hourly_distribution

.PHONY: all clean

point.o: ../cloud/point.cpp ../cloud/point.hpp
	$(CC) -c ../cloud/point.cpp

cloud.o: ../cloud/cloud.cpp ../cloud/point.hpp ../cloud/cloud.hpp ../cloud/distribution.hpp
	$(CC) -c ../cloud/cloud.cpp

distribution.o: ../cloud/distribution.cpp ../cloud/distribution.hpp
	$(CC) -c ../cloud/distribution.cpp

hourly_distribution: hourly_distribution.cpp point.o cloud.o distribution.o
	$(CC) -o hourly_distribution hourly_distribution.cpp point.o cloud.o distribution.o

clean:
	rm -vf hourly_distribution *.o *~
