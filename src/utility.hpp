#include <iostream>
#include <vector>

using namespace std;

/// @brief Method to generate a diagonal dominant matrix
/// @param n size of the nxn matrix
/// @param seed seed of the srand function
/// @return A random diagonal dominat matrix of float 
vector<vector<float>> generate_diagonal_dominant_matrix(size_t n, int seed);

/// @brief Method to generate a random vector
/// @param n size of the vector
/// @param seed seed of the srand function
/// @return A random vector of float 
vector<float> generate_vector(size_t n, int seed);
