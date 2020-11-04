/*
* This is my take on different CPU Scheduling Algorithms meshed into 
* multilevel queue. (A failed "laboratory exercise" from our professor in my university)
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

int numProc = 0;
int foreground = 0, background = 0; // foreground and background processes
int qFore = 0;
int qBack = 0;
vector<process> sortProc;

void processValues(vector<int>);
void FCFS(vector<int>, vector<int>, vector<int>);

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
    sort(sortProc.begin(), sortProc.end(), [](const auto &i, const auto &j) {
        return i.arrival < j.arrival;
    });

    
}

void processValues(vector<int> processes)
{
    numProc = processes[0];

    // AT, BT, Priority (1 or 2)
    for (int i = 0; i < numProc; i++)
        sortProc.push_back({(i + 1)
        , (processes[i + 1])
        , (processes[numProc + i + 1])
        , (processes[(numProc * 2) + i + 1])});

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

void FCFS(vector<int> at, vector<int> bt, vector<int> p)
{
}
