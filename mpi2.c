/* bubble sort */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#define N 100000

void readfile(int num[N]);
int * merge(int *v1, int n1, int *v2, int n2);
void swap(int *v, int i, int j);
void sort(int *v, int n);
void mpi(int num[N], int * chunk);

int main(int argc, char **argv){
	int num[N];
	int * chunk;
	int rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0){
		printf("Getting data...\n");
		readfile(num);
		printf("Sorting data...\n\n");
	}
	mpi(num, chunk);
	if(rank == 0){
		printf("\nParallel bubble sort in mpi sucessfully.\n");
	}
	MPI_Finalize();
	return 0;
}

void readfile(int num[N]){
	int temp;
	int i;
	FILE *fp;
	fp = fopen("number.txt", "r");
	i = 0;
	if(fp == NULL){
		printf("Error loading file!!\n");
		exit(1);
	}else{
		while(!feof(fp)){
			fscanf(fp, "%d", &temp);
			num[i] = temp;
			i++;
		}
	}
	fclose(fp);
}

int * merge(int *v1, int n1, int *v2, int n2){
	int i,j,k;
	int * result;

	result = (int *)malloc((n1+n2)*sizeof(int));
	i=0; j=0; k=0;
	while(i<n1 && j<n2)
		if(v1[i]<v2[j]){
			result[k] = v1[i];
			i++; k++;
		}
		else{
			result[k] = v2[j];
			j++; k++;
		}
	if(i==n1)
		while(j<n2){
			result[k] = v2[j];
			j++; k++;
		}
	else
		while(i<n1){
			result[k] = v1[i];
			i++; k++;
		}
	return result;
}

void swap(int *v, int i, int j){
	int t;
	t = v[i];
	v[i] = v[j];
	v[j] = t;
}

void sort(int *v, int n){
	int i,j;
	for(i=n-2;i>=0;i--)
		for(j=0;j<=i;j++)
			if(v[j]>v[j+1])
				swap(v,j,j+1);
}

void mpi(int num[N], int * chunk){
	int * data;
	int * other;
	int m,n,id,p,s,j,r;
	int step;
	
	int i;
	struct timeval tv;
	struct timezone tz;
	double start, end, time, time1, time2, average;
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	
	start = 0;
	end = 0;
	time = 0;
	time1 = 0;
	time2 = 0;
	n = 10000;
	average = 0;
	if(id == 0){
		printf("Time execution for parallel bubble sort using mpi with 20 processors\n");
		printf("================================================================================\n");
		printf("   Number of data        1st time       2nd time       3rd time       average   \n");
		printf("================================================================================\n");
	}
	while (n <= N){
		for (i = 0; i < 3; i++){
			if(id == 0){
				srandom(clock());
				s = n/p;
				r = n%p;
				data = (int *)malloc((n+p-r)*sizeof(int));
				for(j=0;j<n;j++)
					data[j] = num[j];
				if(r!=0){
					for(j=n;j<n+p-r;j++)
					   data[j]=0;
					s=s+1;
				}
				
				gettimeofday(&tv, &tz);
				start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
			
				MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
				chunk = (int *)malloc(s*sizeof(int));
				MPI_Scatter(data,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);

				sort(chunk,s);
			}else{
				MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
				chunk = (int *)malloc(s*sizeof(int));
				MPI_Scatter(data,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);

				sort(chunk,s);
			}
			step = 1;
			while(step<p){
				if(id%(2*step)==0){
					if(id+step<p){
						MPI_Recv(&m,1,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
						other = (int *)malloc(m*sizeof(int));
						MPI_Recv(other,m,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
						chunk = merge(chunk,s,other,m);
						s = s+m;
					} 
				}else{
					int near = id-step;
					MPI_Send(&s,1,MPI_INT,near,0,MPI_COMM_WORLD);
					MPI_Send(chunk,s,MPI_INT,near,0,MPI_COMM_WORLD);
					break;
				}
				step = step*2;
			}
			if(id == 0){
				gettimeofday(&tv, &tz);
				end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
		
				if (i == 0)
					time = end - start;
				else if (i == 1)
					time1 = end - start;
				else if (i == 2)
					time2 = end - start;\
			}
		}
		if(id == 0){
			average = (time + time1 + time2) / 3;
			printf("      %i              %fs      %fs      %fs      %fs\n", n, time, time1, time2, average);
		}
		n += 10000;
	}
	
	if(id==0){
		FILE * fout;
		fout = fopen("update.txt","w");
		for(i=0;i<s;i++)
		   if (chunk[i] != 0)  
			fprintf(fout,"%d    ",chunk[i]);
		fclose(fout);
	}
}