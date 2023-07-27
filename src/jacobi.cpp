#include <iostream>
#include <vector>

#include <thread>
#include <barrier>

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#include "utimer.cpp"
#include "utility.hpp"
#include "jacobi.hpp"

using namespace std;
using namespace ff;


void sequential_jacobi(vector<vector<float>> &A, vector<float> &b, vector<float> &prev_x, size_t K, long &sequential_time, int strong_debug) 
{
	size_t n = b.size();
	vector<float> new_x(n);
	float summatory;

	{	
		utimer u_timer = utimer("Sequential Time ", &sequential_time, strong_debug);
		for (size_t k = 0; k < K; k++) 
		{
			for (size_t i = 0; i < n; i++) 
			{
				summatory = 0;
				for (size_t j = 0; j < n; j++)
					summatory = ((i == j) * summatory) + ((i != j) * (summatory + A[i][j] * prev_x[j]));
				new_x[i] = (b[i] - summatory) / A[i][i];
			}
			prev_x.swap(new_x);
		}
	}
}


void standard_parallel_jacobi(vector<vector<float>> &A, vector<float> &b, vector<float> &prev_x, size_t K, long &standard_parallel_time, size_t nw, int strong_debug)
{
	size_t n = b.size();
	vector<float> new_x(n);

	{
		string timer_string = "Standard Parallel Time using " + to_string(nw) + " workers ";
		utimer u_timer = utimer(timer_string, &standard_parallel_time, strong_debug);

		size_t chunk_size {n / nw};

		auto on_completion = [&]() noexcept 
		{
        	K--;
        	prev_x.swap(new_x);
    	};

		barrier sync_point(nw, on_completion);

		auto body = [&](size_t id) 
		{
			size_t start_chunk {id*chunk_size}; 
    		size_t end_chunk {(id == (nw-1) ? n-1 : (id+1)*chunk_size)}; 
			while (K > 0) 
			{
				for (size_t i = start_chunk; i<=end_chunk; i++) 
				{
					float summatory = 0;
					for (size_t j = 0; j < n; j++) 
						summatory = ((i == j) * summatory) + ((i != j) * (summatory + A[i][j] * prev_x[j]));
					new_x[i] = (b[i] - summatory) / A[i][i];
				}
				sync_point.arrive_and_wait();
        	}
    	};

		vector<thread> threads(nw);

		for (size_t i = 0; i < nw; i++) 
            threads[i] = thread(body, i);

		for (thread &t : threads) 
			t.join();
	}
} 


void ff_jacobi(vector<vector<float>> &A, vector<float> &b, vector<float> &prev_x, size_t K, long &ff_time, size_t nw, int strong_debug)
{
	size_t n = b.size();
	vector<float> new_x(n);

    {
		string timer_string = "FastFlow Time using " + to_string(nw) + " workers ";
		utimer u_timer = utimer(timer_string, &ff_time, strong_debug);

		ParallelFor pf(nw);

		for (size_t k = 0; k < K; k++) 
		{
			pf.parallel_for(0, n, 1, 0, [&](size_t i)
			{
				float summatory = 0;
				for(size_t j = 0; j < n; j++)
					summatory = ((i == j) * summatory) + ((i != j) * (summatory + A[i][j] * prev_x[j]));
				new_x[i] = (b[i] - summatory) / A[i][i];
			}, nw);
			prev_x.swap(new_x);
		}
    }
}