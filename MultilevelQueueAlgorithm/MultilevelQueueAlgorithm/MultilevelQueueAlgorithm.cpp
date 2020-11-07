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
int numProc = 0;
int foreground = 0, background = 0; // foreground and background processes
int qFore = 0;
int qBack = 0;
bool processing = true;

void processValues(vector<int>);
int FCFS(int, int);

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

    // priority checking
    int burstSum = 0;
    for (int i = 0, j = 0, p = 0, rp = 0; processing;)
    {
        //readMe.push_back({sortProc[i].order, sortProc[i].arrival, sortProc[i].burst, sortProc[i].priority});
        // i = time units, check if a process arrives at every time unit. Then check their priority
        if (i == sortProc[j].arrival)
        {
            if (readMe.size() < 1)
            {
                readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst, sortProc[j].priority });
                p = readMe[j].priority;
            }
            else
            {
                readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst, sortProc[j].priority });
                for (int k = 0, r = 0; k < readMe.size(); k++, r++)
                {
                    if (readMe[r].priority == 2) // sorts processes according to priority. priority 1 will always go first
                    {
                        //process move = new process({readMe[k].order, readMe[k].arrival, readMe[k].burst, readMe[k].priority});
                        readMe.push_back({ readMe[r].order, readMe[r].arrival, readMe[r].burst, readMe[r].priority });
                        readMe.erase(readMe.begin() + r);
                        r -= 1;
                    }
                }
                p = readMe[j].priority;
            }

            //process scheduling algo
            switch (p) // switch case for running an algo that is determined by p (foreground/background)
            {
            case 1:
                FCFS(rp, i);
                break;
            case 2:
                break;
            }
            j++;
        }
        else
        {
            switch (p) // switch case for running an algo that is determined by p (foreground/background)
            {
            case 1:
                FCFS(rp, i);
                break;
            case 2:
                break;
            }
        }

        processing = false; // breaker for now (for testing)

        for (int i = 0; i < sortProc.size(); i++)
            burstSum += sortProc[i].burst;
        if (burstSum == 0)
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

int FCFS(int running, int time)
{
    readMe[running].burst -= 1;
    if (readMe[running].burst == 0) {
        ganttChart.push_back({ readMe[running].order, readMe[running].burst });
            readMe.erase(readMe.begin() + running);
    }

    return time;
}