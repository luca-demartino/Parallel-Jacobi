#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>

#include "utility.hpp"
#include "jacobi.hpp"

using namespace std;
using filesystem::current_path;


int main(int argc, char *argv[]) 
{
    if(argc == 2 && strcmp(argv[1],"-help")==0) 
    {
        cout << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cout << "[N]: linear system dimension" << endl;
        cout << "[SEED]: seed number for the matrix and array generation" << endl;
        cout << "[K]: number of iterations of the Jacobi Method" << endl;
        cout << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
        cout << "[MODE]: implementation mode types: " << endl; 
        cout << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
        cout << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
        cout << "[FILENAME]: filename without the .csv extension" << endl;
        cout << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
        cout << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
        exit(EXIT_FAILURE);
    } 
    
    if(argc < 9)
    {
        cerr << "Too few parameters!" << endl;
        cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cerr << "[N]: linear system dimension" << endl;
        cerr << "[SEED]: seed number for the matrix and array generation" << endl;
        cerr << "[K]: number of iterations of the Jacobi Method" << endl;
        cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
        cerr << "[MODE]: implementation mode types: " << endl; 
        cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
        cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
        cerr << "[FILENAME]: filename without the .csv extension" << endl;
        cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
        cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
        exit(EXIT_FAILURE);
    }

    size_t n =  static_cast<size_t> (stoi(argv[1]));
    if(n < static_cast<size_t> (1))
    {
        cerr << "Size cannot be less than one!" << endl;
        cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cerr << "[N]: linear system dimension" << endl;
        cerr << "[SEED]: seed number for the matrix and array generation" << endl;
        cerr << "[K]: number of iterations of the Jacobi Method" << endl;
        cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
        cerr << "[MODE]: implementation mode types: " << endl; 
        cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
        cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
        cerr << "[FILENAME]: filename without the .csv extension" << endl;
        cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
        cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
        exit(EXIT_FAILURE);
    }

    int seed = stoi(argv[2]);

    size_t K =  static_cast<size_t> (stoi(argv[3]));
    if(K < static_cast<size_t> (1))
    {
        cerr << "K cannot be less than one!" << endl;
        cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cerr << "[N]: linear system dimension" << endl;
        cerr << "[SEED]: seed number for the matrix and array generation" << endl;
        cerr << "[K]: number of iterations of the Jacobi Method" << endl;
        cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
        cerr << "[MODE]: implementation mode types: " << endl; 
        cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
        cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
        cerr << "[FILENAME]: filename without the .csv extension" << endl;
        cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
        cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
        exit(EXIT_FAILURE);
    }

    size_t trials =  static_cast<size_t> (stoi(argv[4]));
    if(trials < static_cast<size_t> (1))
    {
        cerr << "The number of trials cannot be less than one!" << endl;
        cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cerr << "[N]: linear system dimension" << endl;
        cerr << "[SEED]: seed number for the matrix and array generation" << endl;
        cerr << "[K]: number of iterations of the Jacobi Method" << endl;
        cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
        cerr << "[MODE]: implementation mode types: " << endl; 
        cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
        cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
        cerr << "[FILENAME]: filename without the .csv extension" << endl;
        cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
        cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
        exit(EXIT_FAILURE);
    }

    string mode = argv[5];

    enum e_mode 
    {
        SEQUENTIAL = 0,
        STANDARD_PARALLEL = 1,
        FAST_FLOW = 2,
        ERROR = 3
    };

    int mode_result;

    if(mode == "seq")
        mode_result = SEQUENTIAL;
    else if (mode == "std_par")
        mode_result = STANDARD_PARALLEL;
    else if (mode == "ff")
        mode_result = FAST_FLOW;
    else
        mode_result = ERROR;

    if(mode_result == ERROR)
    {
        cerr << "The mode argument is wrong!" << endl;
        cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
        cerr << "[N]: linear system dimension" << endl;
        cerr << "[SEED]: seed number for the matrix and array generation" << endl;
        cerr << "[K]: number of iterations of the Jacobi Method" << endl;
        cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
        cerr << "[MODE]: implementation mode types: " << endl; 
        cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
        cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
        cerr << "[FILENAME]: filename without the .csv extension" << endl;
        cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
        cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
        exit(EXIT_FAILURE);
    }


    if(mode_result == SEQUENTIAL)
    {
        if(argc > 9)
        {
            cerr << "Too many arguments for sequential mode!" << endl;
            cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
            cerr << "[N]: linear system dimension" << endl;
            cerr << "[SEED]: seed number for the matrix and array generation" << endl;
            cerr << "[K]: number of iterations of the Jacobi Method" << endl;
            cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
            cerr << "[MODE]: implementation mode types: " << endl; 
            cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
            cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
            cerr << "[FILENAME]: filename without the .csv extension" << endl;
            cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
            cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
            exit(EXIT_FAILURE);
        }
        else if(argc < 9)
        {
            cerr << "Too few arguments for sequential mode!" << endl;
            cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
            cerr << "[N]: linear system dimension" << endl;
            cerr << "[SEED]: seed number for the matrix and array generation" << endl;
            cerr << "[K]: number of iterations of the Jacobi Method" << endl;
            cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
            cerr << "[MODE]: implementation mode types: " << endl; 
            cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
            cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
            cerr << "[FILENAME]: filename without the .csv extension" << endl;
            cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
            cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
            exit(EXIT_FAILURE);
        }

    }

    if(mode_result == STANDARD_PARALLEL)
    {
        if(argc > 10)
        {
            cerr << "Too many arguments for standard parallel mode!" << endl;
            cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
            cerr << "[N]: linear system dimension" << endl;
            cerr << "[SEED]: seed number for the matrix and array generation" << endl;
            cerr << "[K]: number of iterations of the Jacobi Method" << endl;
            cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
            cerr << "[MODE]: implementation mode types: " << endl; 
            cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
            cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
            cerr << "[FILENAME]: filename without the .csv extension" << endl;
            cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
            cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
            exit(EXIT_FAILURE);
        }
        else if (argc < 10)
        {
            cerr << "Too few arguments for standard parallel mode!" << endl;
            cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
            cerr << "[N]: linear system dimension" << endl;
            cerr << "[SEED]: seed number for the matrix and array generation" << endl;
            cerr << "[K]: number of iterations of the Jacobi Method" << endl;
            cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
            cerr << "[MODE]: implementation mode types: " << endl; 
            cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
            cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
            cerr << "[FILENAME]: filename without the .csv extension" << endl;
            cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
            cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
            exit(EXIT_FAILURE);  
        }
    }

    if(mode_result == FAST_FLOW)
    {
        if(argc > 10)
        {
            cerr << "Too many arguments for FastFlow mode!" << endl;
            cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
            cerr << "[N]: linear system dimension" << endl;
            cerr << "[SEED]: seed number for the matrix and array generation" << endl;
            cerr << "[K]: number of iterations of the Jacobi Method" << endl;
            cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
            cerr << "[MODE]: implementation mode types: " << endl; 
            cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
            cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
            cerr << "[FILENAME]: filename without the .csv extension" << endl;
            cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
            cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
            exit(EXIT_FAILURE);
        }
        else if(argc < 10)
        {
            cerr << "Too few arguments for FastFlow mode!" << endl;
            cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
            cerr << "[N]: linear system dimension" << endl;
            cerr << "[SEED]: seed number for the matrix and array generation" << endl;
            cerr << "[K]: number of iterations of the Jacobi Method" << endl;
            cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
            cerr << "[MODE]: implementation mode types: " << endl; 
            cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
            cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
            cerr << "[FILENAME]: filename without the .csv extension" << endl;
            cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
            cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
            exit(EXIT_FAILURE);
        }  
    }

    size_t nw;
    string filename;
    int strong_debug;
    int fixed_dim;

    switch(mode_result)
    {
        case SEQUENTIAL:
            filename = argv[6];
            strong_debug = stoi(argv[7]);
            fixed_dim = stoi(argv[8]);
        break;

        case STANDARD_PARALLEL:
            nw =  static_cast<size_t> (stoi(argv[6]));
            if(nw < static_cast<size_t> (1))
            {
                cerr << "Number of Workers cannot be less than one!" << endl;
                cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
                cerr << "[N]: linear system dimension" << endl;
                cerr << "[SEED]: seed number for the matrix and array generation" << endl;
                cerr << "[K]: number of iterations of the Jacobi Method" << endl;
                cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
                cerr << "[MODE]: implementation mode types: " << endl; 
                cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
                cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
                cerr << "[FILENAME]: filename without the .csv extension" << endl;
                cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
                cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
                exit(EXIT_FAILURE);
            }
            filename = argv[7];
            strong_debug = stoi(argv[8]);
            fixed_dim = stoi(argv[9]);
            break;

        case FAST_FLOW:
            nw =  static_cast<size_t> (stoi(argv[6]));
            if(nw < static_cast<size_t> (1))
            {
                cerr << "Number of Workers cannot be less than one!" << endl;
                cerr << "Usage is: " << argv[0] << " followed by these parameters: "<< endl;
                cerr << "[N]: linear system dimension" << endl;
                cerr << "[SEED]: seed number for the matrix and array generation" << endl;
                cerr << "[K]: number of iterations of the Jacobi Method" << endl;
                cerr << "[TRIALS]: Number of trials of the Jacobi Method for the time analysis"  << endl;
                cerr << "[MODE]: implementation mode types: " << endl; 
                cerr << "\t seq (for sequential) \t std_par (for parallel using standard library) \t ff (for FastFlow) " << endl;
                cerr << "[NW]: number of workers for the parallel code (NOT use it if you want to use seq mode)" << endl;
                cerr << "[FILENAME]: filename without the .csv extension" << endl;
                cerr << "[DEBUG]: insert any number to print the time of the algorithm for each trials, 0 to not print it" << endl;
                cerr << "[FIXED_DIM]: Used to standardize the name of the \emph{csv} file, insert any number to run the method with a fixed dimension of the linear system, 0 to run the method with a fixed number of workers" << endl;
                exit(EXIT_FAILURE);
            }
            filename = argv[7];
            strong_debug = stoi(argv[8]);
            fixed_dim = stoi(argv[9]);
            break;

        default:
            exit(EXIT_FAILURE);
    }

    long time;
    long double avg_time = 0;
    vector<vector<float>> A = generate_diagonal_dominant_matrix(n, seed);
    vector<float> b = generate_vector(n, seed);
    vector<float> x = generate_vector(n, seed);

    switch(mode_result)
    {
        case SEQUENTIAL:
            for (size_t i = 0; i < trials; i++)
            {
                sequential_jacobi(A,b,x,K,time,strong_debug);
                avg_time += time;
            }
            avg_time /= trials;
            cout << "Sequential Average Time: " << avg_time << " microseconds in " << trials << " trials" << endl;
        break;

        case STANDARD_PARALLEL:
            for (size_t i = 0; i < trials; i++)
            {
                standard_parallel_jacobi(A,b,x,K,time,nw,strong_debug);
                avg_time += time;
            }
            avg_time /= trials;
            cout << "Standard Parallel Average Time: " << avg_time << " microseconds in " << trials << " trials" << endl;
            break;

        case FAST_FLOW:
            for (size_t i = 0; i < trials; i++)
            {
                ff_jacobi(A,b,x,K,time,nw,strong_debug);
                avg_time += time;
            }
            avg_time /= trials;
            cout << "FastFlow Average Time: " << avg_time << " microseconds in " << trials << " trials" << endl;
            break;

        default:
            exit(EXIT_FAILURE);
    }

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

    ofstream my_file;
    if(fixed_dim != 0)
    {
        filename =  pathname_string + "/" + filename + '_' + mode + '-' + to_string(n) + ".csv";
        my_file.open(filename, ios::app);
        if(!my_file.is_open())
        {
            cerr << "Cannot open the file " << filename << ".csv" << endl;
            exit(EXIT_FAILURE);
        }
        if(mode_result == SEQUENTIAL)
            my_file << 1 << "\t" << avg_time << endl;
        else
            my_file << nw << "\t" << avg_time << endl;
    }
    else
    {
        if(mode_result != SEQUENTIAL)
            filename =  pathname_string + "/" + filename + '_' + mode + '-' + to_string(nw) + ".csv";
        else 
            filename =  pathname_string + "/" + filename + '_' + mode + ".csv";
        my_file.open(filename, ios::app);
        if(!my_file.is_open())
        {
            cerr << "Cannot open the file " << filename << ".csv" << endl;
            exit(EXIT_FAILURE);
        }
        my_file << n << "\t" << avg_time << endl;
    }
        
    my_file.close();

    exit(EXIT_SUCCESS);
}
