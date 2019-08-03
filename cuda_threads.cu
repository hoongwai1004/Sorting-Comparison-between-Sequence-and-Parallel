#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <cuda.h>
#define LINE 100000

void readfile(int num[LINE]){
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

void printfile(int num[LINE]){
	int i;
	FILE *fp = fopen("update.txt", "w");
	for (i = 0; i < LINE; i++)
		fprintf(fp, "%d   ", num[i]);
	fclose(fp);
}

void copyData(int num[LINE], int num1[LINE]){
	int i;
	for(i = 0; i < LINE; i++)
		num1[i] = num[i];
}

__global__ void even(int *dnum, int n){
	int k = threadIdx.x + blockIdx.x * blockDim.x;
	int temp;
	k = k * 2;
	if(k <= n - 2){
		if(dnum[k] > dnum[k + 1]){
			temp = dnum[k];
			dnum[k] = dnum[k + 1];
			dnum[k + 1] = temp;
		}
	}
}

__global__ void odd(int *dnum, int n){
	int k = threadIdx.x + blockIdx.x * blockDim.x;
	int temp;
	k = k * 2 + 1;
	if(k <= n - 2){
		if(dnum[k] > dnum[k + 1]){
			temp = dnum[k];
			dnum[k] = dnum[k + 1];
			dnum[k + 1] = temp;
		}
	}
}

void docuda(int *dnum, int threads){
	int i, add;
	add = 0;
	if(LINE % threads != 0)
		add = 1;
	else
		add = 0;
	for(i = 0; i < LINE; i++){
		even<<<LINE / threads + add, threads>>>(dnum, LINE);
		odd<<<LINE / threads + add, threads>>>(dnum, LINE);
	}
}

void cuda(int num[LINE], int num1[LINE]){
	int threads, block, i, add;
	int *dnum;
	struct timeval tv;
	struct timezone tz;
	double start, end, time, time1, time2, average;
	
	start = 0;
	end = 0;
	time = 0;
	time1 = 0;
	time2 = 0;
	average = 0;
	threads = 2;
	block = 0;
	printf("Time execution for parallel bubble sort using CUDA based on threads per block\n");
	printf("====================================================================================================\n");
	printf("   Block size       Number of threads        1st time       2nd time       3rd time       average   \n");
	printf("====================================================================================================\n");
	while (threads <= 1024){
		if(LINE % threads != 0)
			add = 1;
		else
			add = 0;
		block = LINE / threads + add;
		for (i = 0; i < 3; i++){
			copyData(num, num1);
			cudaMalloc(&dnum, LINE*sizeof(int));
			cudaMemcpy(dnum, num, LINE*sizeof(int), cudaMemcpyHostToDevice);
			gettimeofday(&tv, &tz);
			start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
			docuda(dnum, threads);
			gettimeofday(&tv, &tz);
			end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
			cudaMemcpy(num, dnum, LINE*sizeof(int), cudaMemcpyDeviceToHost);

			if (i == 0)
				time = end - start;
			else if (i == 1)
				time1 = end - start;
			else if (i == 2)
				time2 = end - start;
		}
		average = (time + time1 + time2) / 3;
		printf("      %i                 %i                %fs      %fs      %fs      %fs\n", block, threads, time, time1, time2, average);
		threads = threads * 2;
	}
}

int main(){
	int num[LINE];
	int num1[LINE];
	
	printf("Getting data...\n");
	readfile(num);
	printf("Sorting data...\n\n");
	cuda(num, num1);
	printfile(num);
	printf("\nParallel bubble sort in CUDA sucessfully.\n");
	return 0;
}