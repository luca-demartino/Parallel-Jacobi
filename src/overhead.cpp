#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread>
#include <barrier>
#include <numeric>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <sys/stat.h>
#include <sys/types.h>

#include "utimer.cpp"

using namespace std;
using namespace ff;
using filesystem::current_path;


int main(int argc, char *argv[]) 
{
    if(argc == 2 && strcmp(argv[1],"-help")==0) 
    {
        cout << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cout << "[TRIALS]: Number of trials"  << endl;
        cout << "[MAX_NW]: Maximum number of workers"  << endl;
        cout << "[N]: linear system dimension" << endl;
        exit(EXIT_FAILURE);
    }
    
    size_t trials =  static_cast<size_t> (stoi(argv[1]));
    size_t max_nw =  static_cast<size_t> (stoi(argv[2]));
    size_t n = static_cast<size_t> (stoi(argv[3]));
    int x = 0;
    long standard_overhead;
    long ff_overhead;

    ofstream standard_file;
    ofstream ff_file;
    string pathname_string = current_path().parent_path();
    pathname_string = pathname_string + "/results";
    const char* pathname = pathname_string.c_str();
    struct stat info;

    // if (stat(pathname, &info) == 0)
        //cout << pathname << " path just exists" << endl;

    // else
    if (stat(pathname, &info) != 0)
    {
        cout << "There is no results directory..." << endl;
        cout << "Creating directory..." << endl;
        int check;
        check = mkdir(pathname,0777);
        if (!check)
            cout << "Directory created!" << endl;
        else 
        {
            cerr << "Unable to create directory" << pathname << endl; 
            exit(EXIT_FAILURE);
        }
    }


    string filename =  pathname_string + "/standard_overhead-" + to_string(n) + ".csv";
    standard_file.open(filename, ios::app);

    auto on_completion = [&]() noexcept
    {
        x++;
    };

    for(size_t nw = 1; nw <= max_nw; nw++)
    {
        long double avg_time = 0;
        vector<thread> threads(nw);

        for(size_t i = 0; i < trials; i++) 
        {   
            {
                utimer u_timer = utimer("Standard Overhead time using " + to_string(nw) + " workers ", &standard_overhead, 0);
                barrier sync_point(nw, on_completion);
                size_t chunk_size {n / nw};

                auto body = [&](int id) 
                {                  
                    size_t start_chunk {id*chunk_size}; 
    		        size_t end_chunk {(id == (nw-1) ? n-1 : (id+1)*chunk_size)};

                    sync_point.arrive_and_wait();
                };

                for (size_t i = 0; i < nw; i++) 
                    threads[i] = thread(body, i);

                for (thread &t : threads) 
			        t.join();
            }
            avg_time += standard_overhead;
        }

        avg_time = avg_time / trials;
        cout << "Standard Average Overhead using " << nw << " workers: " << avg_time << " microseconds in " << trials << " trials" << endl;
        standard_file << nw << "\t" << avg_time << endl;
    }

    standard_file.close();


    filename =  pathname_string + "/ff_overhead-" + to_string(n) + ".csv";
    ff_file.open(filename, ios::app);

    for(size_t nw = 1; nw <= max_nw; nw++)
    {
        long double avg_time = 0;

        for(size_t i = 0; i < trials; i++)
        {
            {
                utimer u_timer = utimer("FastFlow Overhead time using " + to_string(nw) + " workers ", &ff_overhead, 0);
                ParallelFor pf(nw);
                pf.parallel_for(0, n, 1, 0, [&](size_t i){return;}, nw);
            }
            avg_time += ff_overhead;
        }
        avg_time = avg_time / trials;
        cout << "FastFlow Average Overhead using " << nw << " workers: " << avg_time << " microseconds in " << trials << " trials" << endl;
        ff_file << nw << "\t" << avg_time << endl;
    
    }
    
    ff_file.close();

    return 0;
}