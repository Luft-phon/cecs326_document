/*
    CECS 326
    Homework 3
    CPU Scheduling Simulator

    Name:
    Student ID:
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

struct Process
{
    string pid;
    int arrivalTime;
    int burstTime;
    int priority;

    // Statistics
    int startTime = -1;
    int finishTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
    int responseTime = 0;
    int remainingTime = 0;
};

//-----------------------------------------------------
// Function Prototypes
//-----------------------------------------------------

bool loadProcesses(const string& filename, vector<Process>& processes);

void runFCFS(vector<Process> processes);

void runSJF(vector<Process> processes);

void runPriority(vector<Process> processes);

void runRoundRobin(vector<Process> processes, int quantum);

void printStatistics(const vector<Process>& processes);

void printGanttChart(const vector<string>& gantt);

//-----------------------------------------------------

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: ./scheduler processes.txt [timeQuantum]" << endl;
        return 1;
    }

    int quantum = 2;

    if (argc == 3)
        quantum = stoi(argv[2]);

    vector<Process> processes;

    if (!loadProcesses(argv[1], processes))
    {
        cout << "Unable to open input file." << endl;
        return 1;
    }

    cout << "==============================\n";
    cout << " CPU Scheduling Simulator\n";
    cout << "==============================\n\n";

    runFCFS(processes);

    runSJF(processes);

    runPriority(processes);

    runRoundRobin(processes, quantum);

    return 0;
}

//-----------------------------------------------------

bool loadProcesses(const string& filename, vector<Process>& processes)
{
    ifstream fin(filename);

    if (!fin)
        return false;

    Process p;

    while (fin >> p.pid >> p.arrivalTime >> p.burstTime >> p.priority)
    {
        p.remainingTime = p.burstTime;
        processes.push_back(p);
    }

    fin.close();

    return true;
}

//-----------------------------------------------------

void runFCFS(vector<Process> processes)
{
    cout << "\n========== FCFS ==========\n";

    // TODO:
    // 1. Schedule the processes.
    // 2. Compute waiting time.
    // 3. Compute turnaround time.
    // 4. Compute response time.
    // 5. Generate the Gantt chart.
    // 6. Print statistics.
}

//-----------------------------------------------------

void runSJF(vector<Process> processes)
{
    cout << "\n========== SJF ==========\n";

    // TODO
}

//-----------------------------------------------------

void runPriority(vector<Process> processes)
{
    cout << "\n========== Priority ==========\n";

    // TODO
}

//-----------------------------------------------------

void runRoundRobin(vector<Process> processes, int quantum)
{
    cout << "\n========== Round Robin ==========\n";

    // TODO
}

//-----------------------------------------------------

void printStatistics(const vector<Process>& processes)
{
    // TODO:
    //
    // Print:
    // Process
    // Waiting Time
    // Turnaround Time
    // Response Time
    //
    // Then print averages.
}

//-----------------------------------------------------

void printGanttChart(const vector<string>& gantt)
{
    // TODO:
    //
    // Example:
    //
    // | P1 | P2 | P3 | P4 |
}