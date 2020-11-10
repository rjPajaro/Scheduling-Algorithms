/*
* This is my take on different CPU Scheduling Algorithms meshed into
* multilevel queue. (A failed "laboratory exercise" from our professor in the university where I study)
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

struct process
{
    int order;
    int arrival;
    int burst;
    int priority;
};
struct gantt
{
    int process;
    int burst;
    int time;
};
bool operator<(const process& a, const process& b) // called when sorting
{
    return a.arrival < b.arrival;
}

vector<process> sortProc;
vector<process> readMe;
vector<gantt> ganttChart;
vector<int> currentProc;
int numProc = 0;
int foreground = 0, background = 0; // foreground and background processes
int qFore = 0;
int qBack = 0;
int currentQueue = -1;
bool processing = true;

void processValues(vector<int>);
int FCFS();

int main()
{
    system("cls");
    cout << "Programmed by: Randall Joseph Pajaro" << endl;
    cout << "MULTILEVEL QUEUE ALGORITHM" << endl;
    cout << endl;

    // Inputs will be coming from a file
    ifstream checker("check.txt");
    vector<int> values;

    while (!checker.eof())
    {
        int vals;
        checker >> vals;
        values.push_back(vals);
    }

    checker.close();
    processValues(values); // <- grabs the arrival time, burst time, priority, foreground and background process

    // UI
    cout << "Number of Processes: " << numProc << endl;
    cout << "Arrival Time:" << endl;
    for (int i = 0; i < sortProc.size(); i++)
        cout << " P" << i + 1 << ": " << sortProc[i].arrival << endl;
    cout << "Burst Time:" << endl;
    for (int i = 0; i < sortProc.size(); i++)
        cout << " P" << i + 1 << ": " << sortProc[i].burst << endl;
    cout << "Priority: (1-FP / 2-BP)" << endl;
    for (int i = 0; i < sortProc.size(); i++)
        cout << " P" << i + 1 << ": " << sortProc[i].priority << endl;

    cout << endl
        << "Algorithms:" << endl;
    cout << " 1) FCFS" << endl
        << " 2) SJF-P" << endl
        << " 3) SJF-NP" << endl
        << " 4) P-P" << endl
        << " 5) P-NP" << endl
        << " 6) RR" << endl
        << endl;

    cout << "Foreground Process: " << foreground << endl;
    if (foreground == 6)
        cout << "Quantum = " << qFore << endl;
    cout << "Background Process: " << background << endl;
    if (background == 6)
        cout << "Quantum = " << qBack << endl;
    cout << endl;

    // sort according to arrival time
    sort(sortProc.begin(), sortProc.end());

    for (int i = 0; i < sortProc.size(); i++) 
        currentProc.push_back(i);
    
    /*
    * Process for multilevel queue:
    * - have an incrementing arrival time and a variable that checks and increments process vector [/]
    * - condition that when the process meets the arrival time, add to the queue [/]
    * - sort according to priority (1 first then 2 would go last) [/]
    * - FCFS
    */

    // priority checking
    int burstSum = 0;
    for (int i = 0, j = 0; processing; i++) // i == time units, processing will only break when every burst reaches 0
    {
        if (i == sortProc[j].arrival){ // condition where process meets arrival time
            if (sortProc[j].priority == 2) { //put all process with priority of 2 at the end of the vector (readMe)
                readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst, sortProc[j].priority });
            }
            else if (sortProc[j].priority == 1) { //put all process with priority of 1 first
                for (int pr = 0; pr < readMe.size(); pr++) {// when the priority is 2, insert the priority
                    if (readMe[pr].priority == 2) {
                        readMe.insert(readMe.begin() + pr, { sortProc[j].order, sortProc[j].arrival, sortProc[j].burst, sortProc[j].priority });
                        break;
                    }
                }
            }
            j++;
        }

        if (currentQueue == -1) // this is to start the queue process if it hasn't started yet
            currentQueue = readMe[0].priority;
        
        //currentQueue will always be reinitialized according to their priority process
        switch (currentQueue) {
        case 1: // foreground
            switch (foreground) {
            case 1: // FCFS
                currentQueue = FCFS();
                break;
            case 2: // SJF-P
                break;
            case 3: // SJF-NP
                break;
            case 4: // P-P
                break;
            case 5: // P-NP
                break;
            case 6: // RR
                break;
            }
            break;
        case 2: // background
            switch(background) {
            case 1: // FCFS
                currentQueue = FCFS();
                break;
            case 2: // SJF-P
                break;
            case 3: // SJF-NP
                break;
            case 4: // P-P
                break;
            case 5: // P-NP
                break;
            case 6: // RR
                break;
            }
            break;
        }
        
        // Debugger
        {
            //if (j == sortProc.size()) {
            //    processing = false; // breaker for now (for testing)
            //    for (int y = 0; y < sortProc.size(); y++) {
            //        cout << "Order: " << readMe[y].order << endl
            //             << "Arrival: " << readMe[y].arrival << endl
            //             << "Burst: " << readMe[y].burst << endl
            //             << "Priority: " << readMe[y].priority << endl << endl;
            //    }
            //}
        }
            
        if (readMe.size() == 0)
            processing = false;
    }
}

void processValues(vector<int> processes)
{
    numProc = processes[0];

    // AT, BT, Priority (1 or 2)
    for (int i = 0; i < numProc; i++)
        sortProc.push_back({ (i + 1), (processes[i + 1]), (processes[numProc + i + 1]), (processes[(numProc * 2) + i + 1]) });

    // FP & BP
    int incrementer = 0;
    foreground = processes[(numProc * 3) + 1];
    if (foreground == 6)
    {
        qFore = processes[(numProc * 3) + 2];
        incrementer++; // only used if foreground is RR
    }
    background = processes[(numProc * 3) + 2 + incrementer];
    incrementer++;
    if (background == 6)
        qBack = processes[(numProc * 3) + 2 + incrementer];
}

int FCFS()
{
    int next = 0;

    return next;
}