# Commands to Run Programs

gcc is for C files, g++ is for C++ files, `-fopenmp` enables OpenMP.

| Program | Compile | Run |
|---|---|---|
| 1a Pi (OpenMP) | `g++ -fopenmp program1a.cpp -o p1a` | `./p1a` |
| 1b Pi (MPI) | `mpic++ program1b.cpp -o p1b` | `mpirun -np 4 ./p1b` |
| 2 Matrix multiplication | `g++ -fopenmp program2.cpp -o p2` | `./p2` |
| 3 Sieve (cache) | `g++ -O2 -fopenmp program3.cpp -o p3` | `./p3` |
| 4a B&W image | `g++ -fopenmp program4a.cpp -o p4a -lgd` | `OMP_SCHEDULE="dynamic,10" ./p4a in.png out.png` |
| 4b Thread colours | `g++ -fopenmp program4b.cpp -o p4b -lgd` | `OMP_SCHEDULE="static,50" ./p4b out.png` |
| 5 MPI message | `mpicc program5.c -o p5` | `mpirun -np 4 ./p5` |
| 6 Word search | `g++ -fopenmp program6.cpp -o p6` | `./p6` (needs `words.txt`) |
| 7 OpenACC matrix | `nvc -acc program7.c -o p7` | `./p7` |

Set thread count with `export OMP_NUM_THREADS=4` (programs 3, 4a, 6).

