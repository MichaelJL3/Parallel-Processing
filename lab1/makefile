<<<<<<< HEAD

compiler=mpicc
runner=mpiexec
flags=-Wall -Wextra -Werror
std=-std=c99
prgm=lab1
ext=c
lib=-lm
err=.001

#process size and matrix size lists for testing
PROCS := 1 2 4 8 16 32 64
SIZES := 2 4 8 16 32 64 128 256 512 1024 2048 4096

#default number of processes
n=4

comp c:
	$(compiler) -g $(flags) $(std) -o $(prgm) $(prgm).$(ext) $(lib)

run r:
	$(runner) -n $(n) $(prgm) $(f)

serial-c s-c:
	g++ -std=c++11 -o lab1Serial lab1Serial.cpp

serial-r s-r:
	cat $(f) | ./lab1Serial

genTests gen compile-tests c-t:
	@for f in $(SIZES); do\
		./gengs $${f} $(err); \
		mv $${f}.txt gen/$${f}.txt; \
	done 

runTests tests r-t:
	#@for n in $(PROCS); do \
 	#	for f in $(SIZES); do\
    #        $(runner) -n $${n} $(prgm) gen/$${f}.txt > tests/$${f}_$(prgm).txt;\
	#		./gsref gen/$${f}.txt > tests/$${f}_gsref.txt;\
	#		cmp --silent tests/$${f}_$(prgm).txt tests/$${f}_gsref.txt || echo "Err: test failed on: $${f}";\
	#	done \
	#done

	for f in $(SIZES); do\
        $(runner) -n $(n) $(prgm) gen/$${f}.txt > tests/$${f}_$(prgm).txt;\
		./gsref gen/$${f}.txt > tests/$${f}_gsref.txt;\
		cmp --silent tests/$${f}_$(prgm).txt tests/$${f}_gsref.txt || echo "Err: test failed on: $${f}";\
	done 

timeTests t-t:
	@for n in $(PROCS); do \
 		for f in $(SIZES); do\
 			echo "Testing size: $${f} with $${n} processes" >> times/$${f}_$(prgm).txt;\
			for t in 1 2 3 4 5; do\
            	{ time $(runner) -n $${n} $(prgm) gen/$${f}.txt >/dev/null; } 2>> times/$${f}_$(prgm).txt;\
			done \
		done \
	done

timed:
	{ time $(runner) -n $(n) $(prgm) gen/$(f).txt >/dev/null; }

clean:
	rm tests/*.txt
	rmdir tests
	rm gen/*.txt
	rmdir gen
	rm times/*.txt
	rmdir times
	rm ./lab1
	rm ./lab1Serial
=======

compiler=mpicc
runner=mpiexec
flags=-Wall -Wextra -Werror
std=-std=c99
prgm=lab1
ext=c
lib=-lm
err=.001

#process size and matrix size lists for testing
PROCS := 1 2 4 8 16 32 64
SIZES := 2 4 8 16 32 64 128 256 512 1024 2048 4096

#default number of processes
n=4

#comp
comp c:
	$(compiler) -g $(flags) $(std) -o $(prgm) $(prgm).$(ext) $(lib)

#run
run r:
	$(runner) -n $(n) $(prgm) $(f)

#compile serial
serial-c s-c:
	g++ -std=c++11 -o serial/gsS serial/gsS.cpp

#run the serial version
serial-r s-r:
	cat $(f) | ./serial/gsS

#compile all the tests for the sizes
genTests gen compile-tests g-t:
	@for f in $(SIZES); do\
		./gengs $${f} $(err); \
		mv $${f}.txt gen/$${f}.txt; \
	done 

#checks that all the results for each 
conTests consistent c-t:
	@for n in $(PROCS); do\
		for f in $(SIZES); do\
	        $(runner) -n $${n} $(prgm) gen/$${f}.txt > tests/temp.txt;\
			cmp --silent tests/$${f}_$(prgm).txt tests/temp.txt || echo "Err: test failed on: $${f}";\
		done \
	done

#compare the output between gsref and gs ensure that consistency test has passed first
runTests tests r-t:
	for f in $(SIZES); do\
        $(runner) -n $(n) $(prgm) gen/$${f}.txt > tests/$${f}_$(prgm).txt;\
		./gsref gen/$${f}.txt > tests/$${f}_gsref.txt;\
		cmp --silent tests/$${f}_$(prgm).txt tests/$${f}_gsref.txt || echo "Err: test failed on: $${f}";\
	done 

#tests all process with all times 5 times to be used later for averaging
timeTests t-t:
	@for n in $(PROCS); do \
 		for f in $(SIZES); do\
 			echo "Testing size: $${f} with $${n} processes" >> times/$${f}_$(prgm).txt;\
			for t in 1 2 3 4 5; do\
            	{ time $(runner) -n $${n} $(prgm) gen/$${f}.txt >/dev/null; } 2>> times/$${f}_$(prgm).txt;\
			done \
		done \
	done

#runs and prints the times
timed:
	{ time $(runner) -n $(n) $(prgm) gen/$(f).txt >/dev/null; }

#deletes the files assumes the folders have all been created
clean:
	rm tests/*.txt
	rmdir tests
	rm gen/*.txt
	rmdir gen
	rm times/*.txt
	rmdir times
	rm ./lab1
	rm ./lab1Serial
>>>>>>> 92d5f696920997afd7b9dfae5a01b33ef08a2a39
