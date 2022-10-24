#include <sys/types.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// this is a fucntion that just prints the PID of the process
int main() {
    cout<<"The PID of the Process is "<<getpid()<<endl;
    return 0;
}