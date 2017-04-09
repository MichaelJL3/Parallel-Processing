
t=4
n=2000

comp c:
	g++ -std=c++11 -o serial serial.cpp
	g++ -std=c++11 -fopenmp -o parallel parallel.cpp

run r:
	./parallel $(n) $(t)

serial s:
	./serial $(n)
