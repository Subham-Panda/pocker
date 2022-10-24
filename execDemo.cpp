#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

using namespace std;

// demo for exec system call which takes the first cli argument, for which we will use the printPID function to make the exec syscall execute it

int main(int argc, char *argv[]) {

    if(fork() == 0) {
        cout<<"This output is from the CHILD PROCESS running the function called using exec syscall"<<endl;
        execvp(argv[1], &argv[1]);
    }

    // wait(int* status_pointer)
    // if status variable is not being used, we can also pass NULL as argument
    // it returns -1 if the process has no child process
    // ECHILD is the error macro defined for no child process. It corresponds to errno 10
    // this infinite while loop runs until all child processes are exited
    // errno is the macro defined in errno.h header. It is the macro for the function _error() which returns an error code in integer format in case a syscall fails
    while(wait(NULL)!=-1 || errno!=ECHILD);

    cout<<"\n\n\nThis output is from the PARENT PROCESS running the function called using exec syscall"<<endl;
    execvp(argv[1],&argv[1]);

    return 0;

}