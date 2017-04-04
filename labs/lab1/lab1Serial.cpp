
#include <iostream>

using namespace std;

float **a;
float *x;
float *y;
float *b;
float err;
int n;

void compute(){
	float sum;

	for(int i=0; i<n; i++){
		sum=b[i];
		for(int j=0; j<i; j++){
			sum-=a[i][j]*x[j];
		}

		for(int j=i+1; j<n; j++){
			sum-=a[i][j]*x[j];
		}

		y[i]=sum/a[i][i];
	}
}

float abs(float n){
	return (n<0?0-n:n);
}

bool eval(){
	float xi;

	for(int i=0; i<n; i++){
		xi=y[i];
		if(abs((xi-x[i])/xi)>err)
			return false;
	}

	return true;
}

void copy(){
	for(int i=0; i<n; i++){
		x[i]=y[i];
	}
}

void getMtx(){
	cin>>n>>err;

	a=new float*[n];
	b=new float[n];
	x=new float[n];
	y=new float[n];

	for(int i=0; i<n; i++){
		cin>>x[i];
	}

	for(int i=0; i<n; i++){
		a[i]=new float[n];
		for(int j=0; j<n; j++){
			cin>>a[i][j];
		}
		cin>>b[i];
	}
}

int main(){
	int count=0;
	bool done;

	getMtx();

	do{
		count++;
		compute();

		done=eval();
		copy();

	}while(!done);

	for(int i=0; i<n; i++){
		cout<<x[i]<<endl;
	}

	cout<<count<<endl;

	delete x;
	delete y;
	delete b;
	delete []a;

	return 0;
}