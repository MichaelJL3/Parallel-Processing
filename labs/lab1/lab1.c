#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

/*** Skeleton for Lab 1 ***/

/***** Globals ******/
float **a; /* The coefficients */
float *x;  /* The unknowns */
float *b;  /* The constants */
float err; /* The absolute relative error */
int num = 0;  /* number of unknowns */


/****** Function declarations */
void check_matrix(); /* Check whether the matrix will converge */
void get_input();  /* Read input from file */

/********************************/



/* Function definitions: functions are ordered alphabetically ****/
/*****************************************************************/

/* 
   Conditions for convergence (diagonal dominance):
   1. diagonal element >= sum of all other elements of the row
   2. At least one diagonal element > sum of all other elements of the row
 */
void check_matrix()
{
  int bigger = 0; /* Set to 1 if at least one diag element > sum  */
  int i, j;
  float sum = 0;
  float aii = 0;
  
  for(i = 0; i < num; i++)
  {
    sum = 0;
    aii = fabs(a[i][i]);
    
    for(j = 0; j < num; j++)
       if( j != i)
	 sum += fabs(a[i][j]);
       
    if( aii < sum)
    {
      printf("The matrix will not converge.\n");
      exit(1);
    }
    
    if(aii > sum)
      bigger++;
    
  }
  
  if( !bigger )
  {
     printf("The matrix will not converge\n");
     exit(1);
  }
}


/******************************************************/
/* Read input from file */
/* After this function returns:
 * a[][] will be filled with coefficients and you can access them using a[i][j] for element (i,j)
 * x[] will contain the initial values of x
 * b[] will contain the constants (i.e. the right-hand-side of the equations
 * num will have number of variables
 * err will have the absolute error that you need to reach
 */
void get_input(char filename[])
{
  FILE * fp;
  int i,j;  
 
  fp = fopen(filename, "r");
  if(!fp)
  {
    printf("Cannot open file %s\n", filename);
    exit(1);
  }

 fscanf(fp,"%d ",&num);
 fscanf(fp,"%f ",&err);

 /* Now, time to allocate the matrices and vectors */
 a = (float**)malloc(num * sizeof(float*));
 if( !a)
  {
	printf("Cannot allocate a!\n");
	exit(1);
  }

 for(i = 0; i < num; i++) 
  {
    a[i] = (float *)malloc(num * sizeof(float)); 
    if( !a[i])
  	{
		printf("Cannot allocate a[%d]!\n",i);
		exit(1);
  	}
  }
 
 x = (float *) malloc(num * sizeof(float));
 if( !x)
  {
	printf("Cannot allocate x!\n");
	exit(1);
  }


 b = (float *) malloc(num * sizeof(float));
 if( !b)
  {
	printf("Cannot allocate b!\n");
	exit(1);
  }

 /* Now .. Filling the blanks */ 

 /* The initial values of Xs */
 for(i = 0; i < num; i++)
	fscanf(fp,"%f ", &x[i]);
 
 for(i = 0; i < num; i++)
 {
   for(j = 0; j < num; j++)
     fscanf(fp,"%f ",&a[i][j]);
   
   /* reading the b element */
   fscanf(fp,"%f ",&b[i]);
 }
 
 fclose(fp); 

}


/************************************************************/

void compute(int first_i, int last_i, float *y){
  float sum;
  int indx=0;

  for(int i=first_i; i<last_i; i++){
    sum=b[i];
    for(int j=0; j<i; j++){
      sum-=a[i][j]*x[j];
    }

    for(int j=i+1; j<num; j++){
      sum-=a[i][j]*x[j];
    }

    y[indx++]=sum/a[i][i];
  }
}

float absolute(float n){
  return (n<0?0-n:n);
}

int eval(int first_i, int last_i, float *y){
  float yi;
  int indx=0;

  for(int i=first_i; i<last_i; i++){
    yi=y[indx++];
    if(absolute((yi-x[i])/yi)>err)
      return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{

 int i;
 int nit = 0; /* number of iterations */
 int my_rank, comm_sz, first_i, last_i, loc_n;
 int pass, done;        //used for boolean logic
 float *y=NULL;         //used to hold newX values

 MPI_Init(&argc, &argv);

 MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
 MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

 if( argc != 2)
 {
   printf("Usage: gsref filename\n");
   exit(1);
 }
  
 /* Read the input file and fill the global data structure above */ 
 get_input(argv[1]);
 
 /* Check for convergence condition */
 /* This function will exit the program if the coffeicient will never converge to 
  * the needed absolute error. 
  * This is not expected to happen for this programming assignment.
  */
 check_matrix(); 

 MPI_Comm comm;
 if(my_rank>num-1){
  MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, my_rank, &comm);
  MPI_Finalize();
  exit(0);
 }
 else
  MPI_Comm_split(MPI_COMM_WORLD, 0, my_rank, &comm);

 MPI_Comm_size(comm, &comm_sz);

 loc_n = ceil((float)num/comm_sz);
 first_i=my_rank*loc_n;
 last_i=first_i+loc_n;

 if(last_i>num)
  last_i=num;

 if(num>1&&!(num%2)){
  x=realloc(x, sizeof(float)*comm_sz*loc_n);
 }

 y=malloc(sizeof(float)*loc_n);

 do{

  nit++;
  compute(first_i, last_i, y);
  pass=eval(first_i, last_i, y);

  MPI_Allreduce(&pass, &done, 1, MPI_INT, MPI_MAX, comm);
  MPI_Allgather(y, loc_n, MPI_FLOAT, x, loc_n, MPI_FLOAT, comm);

 }while(done);

 MPI_Comm_free(&comm);

 if(!my_rank){
   /* Writing to the stdout */
   /* Keep that same format */
   for( i = 0; i < num; i++){
     printf("%f\n",x[i]);
     free(a[i]);
   }
   
   printf("total number of iterations: %d\n", nit);
 }else{
   for(i=0; i<num; i++){
    free(a[i]);
   }
 }

 free(x);
 free(a);
 free(b);

 if(y)
  free(y);

 MPI_Finalize();

 exit(0);

}
