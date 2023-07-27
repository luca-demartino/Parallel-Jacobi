#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

#define START(timename) auto timename = chrono::system_clock::now();
#define STOP(timename,elapsed)  auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - timename).count();



class utimer 
{
  chrono::system_clock::time_point start;
  chrono::system_clock::time_point stop;
  string message; 
  int strong_debug;
  using usecs = chrono::microseconds;
  using msecs = chrono::milliseconds;

private:
  long * us_elapsed;
  
public:

  utimer(const string m, int debug) : message(m),us_elapsed((long *)NULL) 
  {
    start = chrono::system_clock::now();
    strong_debug = debug;
  }
    
  utimer(const string m, long * us, int debug) : message(m),us_elapsed(us) 
  {
    start = chrono::system_clock::now();
    strong_debug = debug;
  }

  ~utimer() 
  {
    stop = chrono::system_clock::now();
    chrono::duration<double> elapsed = stop - start;
    auto musec = chrono::duration_cast<chrono::microseconds>(elapsed).count();

    if(strong_debug != 0)
      cout << message << " computed in " << musec << " usec " << endl;
      
    if(us_elapsed != NULL)
      (*us_elapsed) = musec;
  }
};
