
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

#include <fstream>

#include <omp.h>

const int MAX=1000000;
const int CHUNK=4;
const char* ext=".txt";

void ompSieve(char* ar, int t, int n){
	//int m=floor((n+1)/2.0);
	int m=sqrt(n);
	int incr;

	#pragma omp parallel for num_threads(t) schedule(static, CHUNK) private(incr)
	for(int i=3; i<=m; i+=2){ 
		if(!ar[i]){
			incr=i+i;
			for(int j=i+incr; j<=n; j+=incr){
				ar[j]=1;
			}
		}
	}
}

void output(char* ar, char* t, char* size, int n){
	std::ofstream fd;
	int dist=0, prev=2;

  	fd.open (std::string(size)+ext, std::ofstream::out);
  	
  	//include 2 as first prime
  	fd << "1, 2, 0\n"; 
	for(int i=3, count=1; i<n; i+=2){
		if(!ar[i]){
			dist=i-prev;
			prev=i;
			fd << ++count << ", " << i << ", " << dist << "\n";
		}
	}

  	fd.close();
}

int main(int argv, char** argc){
	double st, en;
	int n, t=1;
	char *primes;

	if(argv<3){
		puts("Error: not enough arguments\n./serial N T");
		return 1;
	}else{
		n=atoi(argc[1]);
		t=atoi(argc[2]);
	}

	if(n>MAX||n<2){
		printf("Error: n must be between 2 and %d\n", MAX);
		return 1;
	}

	primes=new char[n+1]();

	st=omp_get_wtime();
	ompSieve(primes, t, n);
	en=omp_get_wtime();

	output(primes, argc[2], argc[1], n);

	printf("Run-Time %f with %d threads for range 2-%d\n", en-st, t, n);

	delete primes;

	return 0;
}
