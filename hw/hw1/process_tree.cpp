#include <iostream>
#include <unistd.h>    // declare fork(), getpid(): child's process id, getppid(): parent's process id
#include <sys/wait.h>  // declare wait()
#include <sys/types.h> // declare pid_t: return type of fork()

using namespace std;

void print_process_info(string name)
{
    cout << "Process: " << name << endl;
    cout << "PID: " << getpid() << endl;
    cout << "Parent PID: " << getppid() << endl;
    cout << endl;
}

void print_simulated_work(string name)
{
    for (int i = 1; i <= 3; i++)
    {
        cout << name << " working... step " << i << endl;
        sleep(1);
    }
    cout << name << " finished." << endl;
    cout << endl;
}

int main(){
    print_process_info("Parent");

    pid_t childA = fork();
    if(childA == 0) {
        print_process_info("Child A");
        
        pid_t childA1 = fork();
        if(childA1 == 0) {
            print_process_info("Child A1");
            print_simulated_work("Child A1");
            return 0; // Child A1 exits after finishing its work
        }

        pid_t childA2 = fork();
        if(childA2 == 0) {
            print_process_info("Child A2");
            print_simulated_work("Child A2");
            return 0; // Child A2 exits after finishing its work
        }

        waitpid(childA1, NULL, 0);
        waitpid(childA2, NULL, 0);
        print_simulated_work("Child A");
        return 0; // Child A exits after finishing its work
    }

    pid_t childB = fork();
    if(childB == 0){
        print_process_info("Child B");
        
        pid_t childB1 = fork();
        if(childB1 == 0) {
            print_process_info("Child B1");
            print_simulated_work("Child B1");
            return 0; // Child B1 exits after finishing its work
        }

        waitpid(childB1, NULL, 0);
        print_simulated_work("Child B");
        return 0; // Child B exits after finishing its work
    }
}
