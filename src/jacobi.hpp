#include <iostream>
#include <vector>

using namespace std;

/// @brief Performs the Jacobi method in sequential mode
/// @param A is the matrix of the linear system Ax=b
/// @param b is the vector of known terms of the linear system Ax=b
/// @param prev_x is the vector of unknowns of the linear system Ax=b
/// @param K is the number of iteration of the Jacobi Method
/// @param sequential_time is the sequential time used to perform the entire algorithm
/// @param strong_debug insert any number to print the time of the algorithm for each trials, 0 to not print it
void sequential_jacobi(vector<vector<float>> &A, vector<float> &b, vector<float> &prev_x, size_t K, long &sequential_time, int strong_debug);

/// @brief Performs the Jacobi method in parallel using the standard library
/// @param A is the matrix of the linear system Ax=b
/// @param b is the vector of known terms of the linear system Ax=b
/// @param prev_x is the vector of unknowns of the linear system Ax=b
/// @param K is the number of iteration of the Jacobi Method
/// @param standard_parallel_time is the standard parallel time used to perform the entire algorithm
/// @param nw is the number of worker used to compute the Jacobi Method in parallel mode
/// @param strong_debug insert any number to print the time of the algorithm for each trials, 0 to not print it
void standard_parallel_jacobi(vector<vector<float>> &A, vector<float> &b, vector<float> &prev_x, size_t K, long &standard_parallel_time, size_t nw, int strong_debug);

/// @brief Performs the Jacobi method in parallel using FastFlow
/// @param A is the matrix of the linear system Ax=b
/// @param b is the vector of known terms of the linear system Ax=b
/// @param prev_x is the vector of unknowns of the linear system Ax=b
/// @param K is the number of iteration of the Jacobi Method
/// @param ff_time is the standard parallel time used to perform the entire algorithm using FastFlow
/// @param nw is the number of worker used to compute the Jacobi Method in parallel mode
/// @param strong_debug insert any number to print the time of the algorithm for each trials, 0 to not print it
void ff_jacobi(vector<vector<float>> &A, vector<float> &b, vector<float> &prev_x, size_t K, long &ff_time, size_t nw, int strong_debug);
