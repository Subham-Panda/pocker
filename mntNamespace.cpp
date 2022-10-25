#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/mount.h>
#include <sys/stat.h>

using namespace std;

bool assertCondtion(bool condition, string errorMessage, int errorLine = __LINE__)
{
    if (!condition)
    {
        cout << errorMessage << " on line " << errorLine << endl;
        cout << "Error code: " << errno << endl;
        cout << "Error message: " << strerror(errno) << endl;
        exit(1);
    }

    return true;
}

void unsharenamespaces()
{
    const int UNSHARE_FLAGS = CLONE_NEWUTS | CLONE_NEWPID;
    const char *HOSTNAME = "pocker";
    assertCondtion(unshare(UNSHARE_FLAGS) != -1, "Unhsaring of UTS namespace has failed");
    assertCondtion(sethostname(HOSTNAME, strlen(HOSTNAME)) != -1, "Setting of host name has failed");
}

void mount_proc() {
    assertCondtion(mkdir("/proc",0755) != -1 || errno == EEXIST, "Failed to create /proc directory");
    assertCondtion(mount("proc", "/proc", "proc", 0, NULL) != -1, "Failed to mount /proc directory");
}

void pocker(char *executionPath, char *argv[])
{
    unsharenamespaces();

    pid_t pidOfChild = fork();
    assertCondtion(pidOfChild != -1, "fork() for PID 1 has failed");

    if (pidOfChild == 0) // init process
    {
        mount_proc();

        pidOfChild = fork();
        assertCondtion(pidOfChild != -1, "fork() for PID 2 has failed");

        if (pidOfChild == 0)
        {
            assertCondtion(execvp(executionPath, argv) != -1, "exec() has failed");
        }

        while(wait(NULL) != -1 || errno == ECHILD);

        cout<<"The child process executing the argument process using exec() has exited, and so init will exit now"<<endl;

    }

    wait(NULL);
}

int main(int argc, char *argv[])
{
    pocker(argv[1], &argv[1]);
    exit(0);
}
