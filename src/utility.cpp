#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "utility.hpp"

using namespace std;

vector<vector<float>> generate_diagonal_dominant_matrix(size_t n, int seed) 
{
    srand(seed);
    vector<vector<float>> matrix(n, vector<float>(n));
    
    for (size_t i = 0; i < n; i++) 
    {
        float summation = 0;
        for (int j = 0; j < n; j++) 
        {
            if (i != j)
            {
                matrix[i][j] = (rand() % 100);
                summation += fabs(matrix[i][j]);
            }
        }
        matrix[i][i] = summation + 1;
    }
    return matrix;
}

vector<float> generate_vector(size_t n, int seed) 
{    
    srand(seed);
    vector<float> vector(n);

    for (size_t i = 0; i < n; i++) 
        vector[i] = (rand() % 100);
    return vector;
} 
