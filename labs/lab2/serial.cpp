
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

#include <fstream>

const int MAX=1000000;
const char* filename="serial_output.txt";

void sieve(int* ar, int n){
	++n;

	int m=floor(n/2.0);

	for(int i=3; i<=m; i+=2){
		if(!ar[i]){
			for(int j=i+i; j<n; j+=i){
				ar[j]=1;
			}
		}
	}
}

void output(int* ar, int n){
	std::ofstream fd;
	int dist=0, prev=2;

  	fd.open (filename, std::ofstream::out);
  	
  	//include 2 as first prime
  	fd << "1 2 0\n"; 
	for(int i=3, count=1; i<n; i+=2){
		if(!ar[i]){
			dist=i-prev;
			prev=i;
			fd << ++count << " " << i << " " << dist << "\n";
		}
	}

  	fd.close();
}

int main(int argv, char** argc){
	double st, en;
	int n;
	int *primes;

	if(argv<2){
		puts("Error: not enough arguments\n./serial N");
		return 1;
	}else{
		n=atoi(argc[1]);
	}

	if(n>MAX||n<2){
		printf("Error: n must be between 2 and %d\n", MAX);
		return 1;
	}

	primes=new int[n+1];

	st=clock();
	sieve(primes, n);
	en=clock();

	output(primes, n);

	printf("Run-Time %f for range 2-%d\n", (double)(en-st)/CLOCKS_PER_SEC, n);

	delete primes;

	return 0;
}
