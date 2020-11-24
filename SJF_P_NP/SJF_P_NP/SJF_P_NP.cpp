#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

struct process
{
	int order;
	int arrival;
	int burst;
};

bool arrivalSort(process a, process b) // called when sorting
{
	return a.arrival < b.arrival;
}
bool burstSort(process a, process b) {
	return a.burst < b.burst;
}
bool processSort(process a, process b) {
	return a.order < b.order;
}

struct gantt
{
	int process;
	int burst;
	int time;
};

vector<process> sortProc;
vector<process> readMe;
vector<gantt> ganttChart;
vector<gantt> gc;
int numProc = 0;
int pos = 0;
int currentQueue = -1;
bool processing = true;
int preempt = 0;

void processValues(vector<int>);
void GanttChart();
void Table();
int SJF_P(int);
int SJF_NP(int);

int main()
{
	system("cls");
	cout << "Programmed by: Randall Joseph Pajaro" << endl;
	cout << "SJF-PREEMPTIVE AND NON-PREEMPTIVE" << endl;
	cout << endl;

	// Inputs will be coming from a file
	ifstream checker("MP01_check.txt");
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

	// sort according to arrival time
	sort(sortProc.begin(), sortProc.end(), arrivalSort);

	cout << endl << endl << "PREEMPTIVE" << endl;
	//SJF_P
	cout << "Gantt Chart" << endl;
	for (int i = sortProc[0].arrival, j = 0; processing; i++) // i == time units, processing will only break when every burst reaches 0
	{
		if (i == sortProc[j].arrival) { // condition where process meets arrival time
			if (preempt == 0) {
				readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst });
				sort(readMe.begin(), readMe.end(), burstSort);

				j++;
				if (j == sortProc.size())
					j = 0;
			}
			else if (preempt == 1) {
				readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst });
				j++;
				if (j == sortProc.size())
					j = 0;
			}
		}

		if (currentQueue == -1) // this is to start the queue process if it hasn't started yet
		{
			currentQueue = readMe[0].order;
			pos = 0;
		}

		SJF_P(i);
		GanttChart();
	}
	Table();

	cout << endl << endl;
	ganttChart.clear();
	gc.clear();
	processing = true;
	currentQueue == -1;
	// sort according to arrival time
	sort(sortProc.begin(), sortProc.end(), arrivalSort);

	cout << "NON-PREEMPTIVE" << endl;
	//SJF_NP
	cout << "Gantt Chart" << endl;
	for (int i = sortProc[0].arrival, j = 0; processing; i++) // i == time units, processing will only break when every burst reaches 0
	{
		if (i == sortProc[j].arrival) { // condition where process meets arrival time
			if (preempt == 0) {
				readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst });
				sort(readMe.begin(), readMe.end(), burstSort);

				j++;
				if (j == sortProc.size())
					j = 0;
			}
			else if (preempt == 1) {
				readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst });
				j++;
				if (j == sortProc.size())
					j = 0;
			}
		}

		if (currentQueue == -1) // this is to start the queue process if it hasn't started yet
		{
			currentQueue = readMe[0].order;
			pos = 0;
		}

		SJF_NP(i);
		GanttChart();
	}
	Table();
}

void processValues(vector<int> processes)
{
	numProc = processes[0];

	// AT, BT, Priority (1 or 2)
	for (int i = 0; i < numProc; i++)
		sortProc.push_back({ (i + 1), (processes[i + 1]), (processes[numProc + i + 1]) });

}

void GanttChart() {
	if (readMe.size() == 0) {
		processing = false;

		for (int u = 0; u < ganttChart.size(); u++) {
			//cout << "P" << ganttChart[u].process << " "; // debug
			if (u == 0) {
				cout << "P" << ganttChart[u].process << " ";
				gc.push_back({ ganttChart[u].process,ganttChart[u].burst,ganttChart[u].time });
			}
			else if (ganttChart[u].process != ganttChart[u - 1].process) {
				cout << "P" << ganttChart[u].process << " ";
				gc.push_back({ ganttChart[u].process,ganttChart[u].burst,ganttChart[u].time });
			}
		}
		cout << endl;
		for (int u = 0; u < ganttChart.size(); u++) {
			//cout << ganttChart[u].time << " "; // debug
			if (u == 0) {
				cout << ganttChart[u].time << " ";
			}
			else if (ganttChart[u].process != ganttChart[u - 1].process) {
				cout << ganttChart[u].time << " ";
			}
		}
		cout << ganttChart.size() << endl << endl;
	}
}

void Table() {
	vector<int> tWaitTbl, ttTbl;
	cout << "Table" << endl;
	sort(sortProc.begin(), sortProc.end(), processSort);
	for (int i = 0; i < numProc; i++) {
		vector<int> Tbl;
		// Grabs the time units from the gantt chart
		for (int u = 0; u < gc.size(); u++) {
			if (sortProc[i].order == gc[u].process) {
				Tbl.push_back(gc[u].time);
				if (u + 1 == gc.size()) {
					Tbl.push_back(ganttChart.size());
				}
				else {
					Tbl.push_back(gc[u + 1].time);
				}
			}
		}

		int wait = 0, tTime = 0;
		// computation for the turnaround time
		tTime = Tbl[Tbl.size() - 1] - sortProc[i].arrival;
		// computations for the waiting time
		if (Tbl.size() > 2) { // if the process was queue'd multiple times:
			for (int s = Tbl.size() - 1; s > 1; s--) { // this loop would subtract all the time units of the same process in the queue
				wait = Tbl[s] - Tbl[s - 1];
			}
			wait += Tbl[0]; // adds the first time unit instance
			wait -= sortProc[i].arrival; // subtracts from the arrival time
		}
		else if (Tbl.size() < 3) { // if the process was queue'd only once:
			wait = Tbl[0] - sortProc[i].arrival; // subtracts the first instance from the arrival time
		}
		cout << "P" << sortProc[i].order << " - " << tTime << " - " << wait << endl; // table output
		ttTbl.push_back(tTime);
		tWaitTbl.push_back(wait);
	}

	int totalTT = 0, totalWait = 0;
	for (int t = 0; t < numProc; t++) {
		totalTT += ttTbl[t];
		totalWait += tWaitTbl[t];
	}
	// Total and Average:
	cout << "Total: " << totalTT << " - " << totalWait << endl;
	cout << "Average: " << (float)totalTT / (float)numProc << " - " << (float)totalWait / (float)numProc << endl;
}

/* SJF-P
* Should choose the process with the lowest burst time first [/]
* When SJF_P = 1, sort all process with priority of 1 [/]
* When SJF_P = 2, sort all process with priority of 2 [/]
*/
int SJF_P(int arrived) {
	int next = 0;

	preempt = 0;
	readMe[pos].burst--;

	ganttChart.push_back({ readMe[pos].order, readMe[pos].burst, arrived });

	if (readMe[pos].burst > 0) // if the process still has bursts left
		next = readMe[pos].order;
	else if (readMe[pos].burst <= 0) { // if the process has no bursts left
		readMe.erase(readMe.begin() + pos);
		if (pos == readMe.size()) { // when the position reaches greater than what's in the Queue, reset position to 0
			pos = 0;
		}

		if (readMe.size() > 0) { // when there are still processes left
			next = readMe[pos].order;
		}
	}
	return next;
}

/* SJF-NP
* Finish whichever process arrives first, then compare the burst on which should be finished next [/]
* When SJF_NP = 1, sort all process with priority of 1 [/]
* When SJF_NP = 2, sort all process with priority of 2 [/]
* Should not preempt even if the arrived process is a background process
*/
int SJF_NP(int arrived) {
	int next = 0;

	preempt = 1;
	readMe[pos].burst--;

	ganttChart.push_back({ readMe[pos].order, readMe[pos].burst, arrived });

	if (readMe[pos].burst > 0) // if the process still has bursts left
		next = readMe[pos].order;
	else if (readMe[pos].burst <= 0) { // if the process has no bursts left
		readMe.erase(readMe.begin() + pos);
		sort(readMe.begin(), readMe.end(), burstSort);
		if (pos == readMe.size()) { // when the position reaches greater than what's in the Queue, reset position to 0
			pos = 0;
		}

		if (readMe.size() > 0) { // when there are still processes left
			next = readMe[pos].order;
		}
	}
	return next;
}