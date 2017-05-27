
#include <cuda.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctime>

using namespace std;

//assignment constraints prevent the optimization of this function
//better approach would have been to take a max per block
//then sort the resulting maxes, with blocks of thread size 1024 you can cut down the serial
//sort from array size to (array size)/1024 or 2^(n-10) where n=log2(array size) assuming it is a power of 2

__global__ void getmaxcu(const unsigned int* numI, unsigned int* numO, const unsigned int size){
	extern __shared__ unsigned int sarr[];

	//thread id
	const unsigned int tdx=threadIdx.x;

	//global location in array
	unsigned int gdx=(blockDim.x)*blockIdx.x+threadIdx.x;

	//compare two initially
	sarr[tdx]=numI[gdx];

    for(unsigned int s=blockDim.x/2; s>0; s>>=1){
        if (tdx<s)
            sarr[tdx]=max(sarr[tdx],sarr[tdx+s]);
        __syncthreads();
    }

	if(!tdx){
		//atomically swap output value for multiple blocks
		atomicMax(numO, sarr[0]);
	}
}

int main(int argc, char *argv[]){

	cudaError_t err;
	unsigned int *numbers_temp_in, *numbers_temp_out;
	unsigned int *numbers_in, *numbers_out;
	unsigned int size;

	//blocksize is 1024 based on cuda5 (original test source) and then my own GPU after all the servers crashed
	const unsigned int blockSize=1024;

	//check for proper args
    if(argc!=2){
       printf("usage: maxseq num\nnum = size of the array\n");
       exit(1);
    }
   
	//get the size and then determine number of blocks in grid
    size = atol(argv[1]);  
	const unsigned int gridSize=ceil((float)size/blockSize);

	//numbers_out = single max element to return
	numbers_out=(unsigned int *)malloc(sizeof(unsigned int));
	if(!numbers_out){
		printf("Failed Allocation\n");
		exit(1);
	} 

	//numbers_in = array of randomized inputs based on size
	numbers_in=(unsigned int *)malloc(size*sizeof(unsigned int));
	if(!numbers_in){
		printf("Failed Allocation\n");
		exit(1);
	} 

	//input the array
    srand(time(NULL)); // setting a seed for the random number generator
    for(unsigned int i=0; i<size; ++i)
       numbers_in[i] = rand()%size;  

	//allocate the memory in the GPU
	cudaMalloc((void**)&numbers_temp_in, sizeof(unsigned int)*size);
	cudaMalloc((void**)&numbers_temp_out, sizeof(unsigned int));

	//copt over the inputs to the device
    cudaMemcpy((void*)numbers_temp_in, (void*)numbers_in, size*sizeof(unsigned int), cudaMemcpyHostToDevice);

	//set the grid/block dimensions
	dim3 dimGrid(gridSize);
	dim3 dimBlock(blockSize);

	//run the gpu max
	getmaxcu<<<dimGrid,dimBlock,blockSize*sizeof(unsigned int)>>>(numbers_temp_in, numbers_temp_out, size);

	//print out some information
	cout<<"Starting GPU Calculation:\nBlocksize: "<<blockSize<<"\nBlocks in grid: "<<gridSize<<"\n";

	//wait until completion
	cudaThreadSynchronize();
	err=cudaGetLastError();
	if(err!=cudaSuccess){
		printf("Err: %s\n", cudaGetErrorString(err));
		exit(1);
	}

	//get the output back from the device
	cudaMemcpy((void*)numbers_out, (void*)numbers_temp_out, sizeof(unsigned int), cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();
	err=cudaGetLastError();
	if(err!=cudaSuccess){
		printf("Err Getting Output: %s\n", cudaGetErrorString(err));
		exit(1);
	}

	//print the max value	
	cout<<"The maximum number in the array is: "<<*numbers_out<<"\n";

	//free memory
	cudaFree(numbers_temp_in);
	cudaFree(numbers_temp_out);
	free(numbers_in);
	free(numbers_out);

	return 0;
}
