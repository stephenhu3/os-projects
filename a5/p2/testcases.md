Successive Over-relaxation Algorithm
Computes solution for 3 x 3 system and converges
Passed
Computes solution for 4 x 4 system and converges
Passed
Computes solution for 5 x 5 system and converges
Passed
Computes solution for 6 x 6 system and converges
Passed
Computes solution accordingly for relaxation factor between (0, 1)
Passed
Computes solution accordingly for relaxation factor equal to 1
Passed
Computes solution accordingly for relaxation factor between (1, 2)
Passed

Shared Memory and Multiple Processes
n processes are created each solving for Xn-1
Passed
Each process can access shared memory accordingly
Passed
No memory leaks or segmentation faults
Passed

// 3 x 3 system of equations
double A[n][n] = { { 12.0, 3.0, -5.0 }, { 1.0, 5.0, 3.0 }, { 3.0, 7.0, 13.0 } };
double b[n] = { 1, 28, 76 };
X1 = 0.999995 
X2 = 3.000005 
X3 = 3.999997 
CORRECT

// 4 x 4 system of equations
double A[n][n] = { { 5, 0, 0, 0}, { 0, 8, 0, 0}, { 2, 3, 4, 5}, { 3, 3, 2, 3} };
double b[n] = { 60, 56, 3, 3 };
X1 = 12.000000 
X2 = 7.000000 
X3 = 71.997364 
X4 = -65.998099 
CORRECT

// 5 x 5 system of equations
double A[n][n] = { { -7, 1, 0, 1, 1 }, { 1, 1, 0, 0, 1}, { 1, 1, 5, 0, 1}, { 1, -1, 1, 1, 2}, { 1, 0, 1, 0, 1} };
double b[n] = { 1, 0, 9, 1, 0 };
X1 = 0.514283 
X2 = 1.799998 
X3 = 1.800000
X4 = 5.114274 
X5 = -2.314282 
CORRECT


// 6 x 6 system of equations
double A[n][n] = { { 9, 0, 0, 0, 0, 0 }, { 1, 2, 0, 0, 0, 0}, { 0, 0, 1, 0, 0, 0}, { 0, 1, 7, -1, 0, 1}, { 1, 1, 1, 1, 1, 1}, { 7, 0, 1, 12, 1, 1} };
double b[n] = { 90, 15, 16, 19, -20, 10 };
X1 = 10.000000 
X2 = 2.500000 
X3 = 16.000000 
X4 = -2.499998 
X5 = 51.999976 
X6 = -98.000000 
CORRECT