/*
    CECS 326
    Homework 3
    CPU Scheduling Simulator

    Name: Phong Ngo
    Student ID: 033898616
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process
{
    string pid;
    int arrivalTime;        //when process moves to ready queue
    int burstTime;
    int priority;

    // Statistics
    int startTime = -1;     //when process is executed in CPU
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

void printGanttChart(const vector<string>& gantt, const vector<int>& ganttTimes);

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

    vector<string> gantt;
    vector<int> ganttTimes;
    // TODO:
    // 1. Schedule the processes.
    // Sort the process with the ascending arrival time
    stable_sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){ // use reference to take the origin var
        return a.arrivalTime < b.arrivalTime;
    });    
    
    // Run the processes 
    int currentTime = 0;
    ganttTimes.push_back(currentTime);
    
    for(Process& p: processes){
        if(currentTime < p.arrivalTime){
            currentTime = p.arrivalTime;
            gantt.push_back("IDLE");
            ganttTimes.push_back(currentTime);
        }
    // 1. Compute start time.
        p.startTime = currentTime;
        currentTime += p.burstTime;
        p.finishTime = currentTime;
    
    // 2. Compute waiting time.
        p.waitingTime = p.startTime - p.arrivalTime;
    // 3. Compute turnaround time.
        p.turnaroundTime = p.finishTime - p.arrivalTime;
    // 4. Compute response time.
        p.responseTime = p.startTime - p.arrivalTime;
        gantt.push_back(p.pid);
        ganttTimes.push_back(currentTime);
    }
    // 5. Generate the Gantt chart.
    printGanttChart(gantt, ganttTimes);

    // 6. Print statistics.
    printStatistics(processes);
}

//-----------------------------------------------------
void runSJF(vector<Process> processes)
{
    cout << "\n========================================\n";
    cout << "SJF (Non-preemptive)\n";
    cout << "========================================\n";

    int currentTime = 0;
    int finished = 0;
    int numberOfProcesses = processes.size();

    vector<bool> isFinished(numberOfProcesses, false);
    vector<string> gantt;
    vector<int> ganttTimes;

    ganttTimes.push_back(currentTime);

    
    while (finished < numberOfProcesses)
    {
        int shortestProcess = -1;

        // select the process having the smallest brust time
        for (int i = 0; i < numberOfProcesses; i++)
        {
            bool processHasArrived = processes[i].arrivalTime <= currentTime;

            if (!isFinished[i] && processHasArrived)
            {
                if (shortestProcess == -1 ||
                    processes[i].burstTime < processes[shortestProcess].burstTime)
                {
                    shortestProcess = i;
                }
            }
        }

        
        if (shortestProcess == -1){
            currentTime++;

            if (!gantt.empty() && gantt.back() == "IDLE"){
                ganttTimes.back() = currentTime;
            } else{
                gantt.push_back("IDLE");
                ganttTimes.push_back(currentTime);
            }

            continue;
        }

        Process& p = processes[shortestProcess];
        p.startTime = currentTime;
        p.responseTime = p.startTime - p.arrivalTime;
        currentTime += p.burstTime;
        p.finishTime = currentTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;

        gantt.push_back(p.pid);
        ganttTimes.push_back(currentTime);


        // mark the process to completed
        isFinished[shortestProcess] = true;
        finished++;
    }

    printGanttChart(gantt, ganttTimes);
    printStatistics(processes);
}
//-----------------------------------------------------

void runPriority(vector<Process> processes)
{
    cout << "\n========================================\n";
    cout << "Priority (Non-preemptive)\n";
    cout << "========================================\n";

    // TODO
    int currentTime = 0;
    int finished = 0;
    int numberOfProcesses = processes.size();
    
    vector<bool> isCompleted(processes.size(), false);
    vector<string> gantt;
    vector<int> ganttTimes;
    ganttTimes.push_back(currentTime);


    while(finished < numberOfProcesses){
        int selected = -1;
        for(int i = 0; i < numberOfProcesses; i++){
            if(!isCompleted[i] && processes[i].arrivalTime <= currentTime){
                if (selected == -1 || processes[i].priority < processes[selected].priority){
                    selected = i;
                }
            }
        }
         if (selected == -1){
            currentTime++;

            if (!gantt.empty() && gantt.back() == "IDLE"){
                ganttTimes.back() = currentTime;
            } else{
                gantt.push_back("IDLE");
                ganttTimes.push_back(currentTime);
            }

            continue;
        }

        Process& p = processes[selected];
        p.startTime = currentTime;
        currentTime += p.burstTime;
        p.finishTime = currentTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.responseTime = p.startTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;

        gantt.push_back(p.pid);
        ganttTimes.push_back(currentTime);

        isCompleted[selected] = true;
        finished++;
    }

    printGanttChart(gantt, ganttTimes);
    printStatistics(processes);

}

//-----------------------------------------------------
void runRoundRobin(vector<Process> processes, int quantum)
{
    cout << "\n========================================\n";
    cout << "Round Robin\n";
    cout << "========================================\n";

    int currentTime = 0;
    int finished = 0;
    int numberOfProcesses =
        static_cast<int>(processes.size());

    vector<int> readyQueue;
    vector<bool> added(processes.size(), false);
    vector<string> gantt;
    vector<int> ganttTimes;
    ganttTimes.push_back(currentTime);


    for (Process& p : processes)
    {
        p.remainingTime = p.burstTime;
        p.startTime = -1;
    }

    while (finished < numberOfProcesses)
    {
        // add first process to the queue
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (!added[i] && processes[i].arrivalTime <= currentTime)
            {
                readyQueue.push_back(i);
                added[i] = true;
            }
        }

        // no processes is in the queue
        if (readyQueue.empty()){
            currentTime++;

            if (!gantt.empty() && gantt.back() == "IDLE"){
                ganttTimes.back() = currentTime;
            } else {
                gantt.push_back("IDLE");
                ganttTimes.push_back(currentTime);
            }

            continue;
        }

        // remove the first process from the queue
        int selected = readyQueue.front();
        readyQueue.erase(readyQueue.begin());

        Process& p = processes[selected];

        // the first time the process gets CPU
        if (p.startTime == -1)
        {
            p.startTime = currentTime;
            p.responseTime = p.startTime - p.arrivalTime;
        }

        // run a quantum
        int runTime = min(quantum, p.remainingTime);
        
        currentTime += runTime;
        p.remainingTime -= runTime;
        if (!gantt.empty() && gantt.back() == p.pid){
            ganttTimes.back() = currentTime;
        } else {
            gantt.push_back(p.pid);
            ganttTimes.push_back(currentTime);
        }



        // add new processes that arrived during this execution
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (!added[i] &&
                processes[i].arrivalTime <= currentTime)
            {
                readyQueue.push_back(i);
                added[i] = true;
            }
        }

        // if process is not finished, it is preempted and moved to the end of queue
        if (p.remainingTime > 0)
        {
            readyQueue.push_back(selected);
        }
        else
        {
            p.finishTime = currentTime;
            p.turnaroundTime = p.finishTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            finished++;
        }
    }

    printGanttChart(gantt, ganttTimes);
    printStatistics(processes);
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
    cout << left
     << setw(10) << "Process"
     << setw(10) << "Arrival"
     << setw(10) << "Burst"
     << setw(10) << "Priority"
     << setw(10) << "Waiting"
     << setw(12) << "Turnaround"
     << setw(10) << "Response"
     << endl;

    for (const Process& p : processes) {
        cout << left
         << setw(10) << p.pid
         << setw(10) << p.arrivalTime
         << setw(10) << p.burstTime
         << setw(10) << p.priority
         << setw(10) << p.waitingTime
         << setw(12) << p.turnaroundTime
         << setw(10) << p.responseTime
         << endl;
    }
    cout << endl;

    double totalWaitingTime = 0.0;
    double totalTurnaroundTime = 0.0;
    double totalResponseTime = 0.0;
    double count =  processes.size();
    for (const Process& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
        totalResponseTime += p.responseTime;
    }
    cout << fixed << setprecision(2);       // 2 digit decimal
    cout << "Average Waiting Time: " << totalWaitingTime / count << endl;
    cout << "Average Turnaround Time: " << totalTurnaroundTime / count << endl;
    cout << "Average Response Time: " << totalResponseTime / count << endl;
    int totalBurstTime = 0;
    int totalExecutionTime = 0;

    for (const Process& p : processes){
        totalBurstTime += p.burstTime;
        totalExecutionTime = max(totalExecutionTime, p.finishTime);
    }

    double cpuUtilization = 0.0;

    if (totalExecutionTime > 0){
        cpuUtilization =
        static_cast<double>(totalBurstTime) /
        totalExecutionTime * 100.0;
    }

    cout << "CPU Utilization: " << cpuUtilization << "%" << endl;

}

//-----------------------------------------------------

void printGanttChart(const vector<string>& gantt, const vector<int>& ganttTimes)
{
    // TODO:
    //
    // Example:
    //| P1 | P2 | P3 | P4 |
    cout << "Gantt Chart:" << endl;
    cout << endl;
    for (size_t i = 0; i < gantt.size(); ++i) {
        cout << "| " << gantt[i] << " ";
    }
    cout << "|" << endl;

    for (size_t i = 0; i < ganttTimes.size(); ++i) {
        cout << left << setw(4) << ganttTimes[i] << " ";
    }
    cout << endl;
}