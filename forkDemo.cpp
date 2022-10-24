#include <sys/types.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    pid_t pidOfParent = getpid();

    // parent process will call the following line to fork a child process
    fork();

    // the part below this will be called by both the parent and child process
    if(getpid() == pidOfParent) {
        cout<<"PID of parent process = "<<getpid()<<endl;  
    } else {
        cout<<"PID of child process = "<<getpid()<<endl;  
    }

    // infinite loop
    // spin up another terminal and use pstree command to check the heirarchy of processes and look for the ids of the parent and the child process
    // once done use ctrl+c to getout of this loop and close the program
    while(true) {
        sleep(1);
    }
}