
t=4
n=2000
size=10000
threads=1 2 5 10 20 40 80 100

comp c:
	g++ -std=c++11 -fopenmp -o genprime genprimes.c

run r:
	./parallel $(n) $(t)

table:
	for threads in $(threads); do \
		./parallel $(size) $$threads >> table_$(size).csv; \
	done

serial s:
	./serial $(n)
