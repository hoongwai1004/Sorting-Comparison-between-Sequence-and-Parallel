#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#define LINE 100000

void readfile(int num[LINE]);
void printfile(int num[LINE]);
void copyData(int num[LINE], int num1[LINE]);
void doopenmp(int num[LINE], int line);
void openmp(int num[LINE], int num1[LINE]);

int main(){
	int num[LINE];
	int num1[LINE];
	
	printf("Getting data...\n");
	readfile(num);
	printf("Sorting data...\n\n");
	openmp(num, num1);
	printfile(num1);
	printf("\nParallel bubble sort in openMP sucessfully.\n");
	return 0;
}

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

void doopenmp(int num[LINE], int line){
	int changes, i;
	int temp;

	changes = 1;
	while (changes){
		omp_set_num_threads(20);
		#pragma omp parallel private(temp)
		{
			changes = 0;
			#pragma omp for reduction(+:changes)
			for (i = 0; i < line - 1; i += 2){
				if (num[i] > num[i + 1]){
					temp = num[i];
					num[i] = num[i + 1];
					num[i + 1] = temp;
					++changes;
				}
			}

			#pragma omp for reduction(+:changes)
			for (i = 1; i < line - 1; i += 2){
				if (num[i] > num[i + 1]){
					temp = num[i];
					num[i] = num[i + 1];
					num[i + 1] = temp;
					++changes;
				}
			}
		}
	}
}

void openmp(int num[LINE], int num1[LINE]){
	int line, i;
	struct timeval tv;
	struct timezone tz;
	double start, end, time, time1, time2, average;
	
	start = 0;
	end = 0;
	time = 0;
	time1 = 0;
	time2 = 0;
	line = 10000;
	average = 0;
	printf("Time execution for parallel bubble sort using openmp with 20 threads\n");
	printf("================================================================================\n");
	printf("   Number of data        1st time       2nd time       3rd time       average   \n");
	printf("================================================================================\n");
	while (line <= LINE){
		for (i = 0; i < 3; i++){
			copyData(num, num1);
			gettimeofday(&tv, &tz);
			start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
			doopenmp(num1, line);
			gettimeofday(&tv, &tz);
			end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;

			if (i == 0)
				time = end - start;
			else if (i == 1)
				time1 = end - start;
			else if (i == 2)
				time2 = end - start;
		}
		average = (time + time1 + time2) / 3;
		printf("      %i              %fs      %fs      %fs      %fs\n", line, time, time1, time2, average);
		line += 10000;
	}
}