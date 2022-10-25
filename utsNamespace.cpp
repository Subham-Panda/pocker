#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>

using namespace std;

bool assertCondtion(bool condition, string errorMessage, int errorLine = __LINE__) {
    if(!condition) {
        cout<<errorMessage<<" on line "<<errorLine<<endl;
        cout<<"Error code: "<<errno<<endl;
        cout<<"Error message: "<<strerror(errno)<<endl;
        exit(1);
    }

    return true;
}

void unsharenamespaces() {
    const int UNSHARE_FLAGS = CLONE_NEWUTS;
    const char* HOSTNAME = "pocker";
    assertCondtion(unshare(UNSHARE_FLAGS)!=-1, "Unhsaring of UTS namespace has failed");
    assertCondtion(sethostname(HOSTNAME, strlen(HOSTNAME))!=-1, "Setting of host name has failed");
}

void pocker(char* executionPath, char* argv[]) {
    pid_t pidOfChild = fork();
    assertCondtion(pidOfChild != -1, "fork() has failed");
    
    // make the child process execute the command
    if(pidOfChild == 0) {
        unsharenamespaces();

        assertCondtion(execvp(executionPath, argv) != -1, "exec() has failed");
    }
}

int main(int argc, char* argv[]) {
    pocker(argv[1], &argv[1]);

    while(wait(NULL) != -1 || errno != ECHILD);

    exit(0);
}
