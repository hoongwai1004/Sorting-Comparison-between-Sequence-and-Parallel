# Sorting-Comparison-between-Sequence-and-Parallel
School Project CST331 (2018)

## Introduction
The parallel algorithm is an algorithm where the several operations may execute simultaneously. The execution will occur by many processing devices within a problem and combined at the end of the result. Many parallel algorithms are executed concurrently. Parallel algorithms in solving the problem may use the different algorithm where may more or less parallelizable. Embarrassingly parallel problems are the problems where it can easily be divided into a piece such as Rubik’s Cubes and hash decryption. 

## Camparison between Sequential and Parallel Bubble Sort, OpenMP, MPI and CUDA
The parallel bubble sort has better performance compared to the sequential bubble sort.This is due to the sequential bubble sort only comparing a single data with other data in the list while the parallel bubble sort compares with several data with other data at the same time. The parallel bubble sort goes beyond the limit of the sequential bubble sort. However, the parallel bubble sort needs more memory usage to compare to the sequential bubble sort.For example, when the parallel and sequential bubble sort run on an equal memory. When the data is small, the time use by parallel bubble sort is very efficient compared to the sequential bubble. However, the time used for parallel bubble sort will decreases as the data become larger where there has the limited memory. OpenMP, MPI and CUDA have its own advantages and limitations. OpenMp is relatively easy to implement and just involves few pragma directives to achieve desired tasks. OpenMp also can be used in recursive function but suffers from the limitations of memory. OpenMP still can run the program as serial code and the serial code statements usually do not need modification. MPI can runs on either shared or distributed memory architectures. MPI usually serve problem well which include large memory. MPI also utilized the advantages of the shared memory. CUDA exposed fast shared memory region that can be shared among threads. CUDA can full support for integer and bitwise operations, including integer texture lookups.
