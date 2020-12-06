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
	int priority;
};
// Sorting = lowest to highest
bool arrivalSort(process a, process b) // called when sorting using arrival
{
	return a.arrival < b.arrival;
}
bool burstSort(process a, process b) { // sorting using burst
	return a.burst < b.burst;
}
bool processSort(process a, process b) { // sort according to process number (process numbers are incremental by default)
	return a.order < b.order;
}

struct gantt
{
	int process;
	int burst;
	int time;
};

vector<process> sortProc; // This is for have a default/static container that can only be sorted
vector<process> readMe;   // This is the container to be manipulated
vector<gantt> ganttChart; // Gantt Chart values
vector<gantt> gc;		  // For comparison/computations using the gantt chart
int numProc = 0;		  // number of processes
int pos = 0;			  // position in the ready queue
int currentQueue = -1;	  // Only used for multilevel queues
int preempt = 0;		  // Preempt detection
int finProc = 0;		  // For counting the number of process that have been finished
bool processing = true;	  // Building the Gantt Chart

void processValues(vector<int>); // Sorts values fod the ready queue
void GanttChart();	// Gantt Chart Function
void Table();	 // Table Function
int P_P(int);  // Priority - Preemptive
int P_NP(int); // Priority - Non-Preemptive
void rerun();

int main()
{
	// Refreshing when repeating this function
	sortProc.clear();
	readMe.clear();
	ganttChart.clear();
	gc.clear();
	numProc = 0;
	pos = 0;
	finProc = 0;
	currentQueue = -1;
	preempt = 0;
	processing = true;

	system("cls");
	cout << "Programmed by: Randall Joseph Pajaro" << endl;
	cout << "PRIORITY - PREEMPTIVE AND NON-PREEMPTIVE" << endl;
	cout << endl;

	// Inputs will be coming from a file
	ifstream checker("MP02_check.txt");
	vector<int> values;

	while (!checker.eof())
	{
		int vals;
		checker >> vals;
		values.push_back(vals);
	}

	checker.close();
	processValues(values); // <- grabs the arrival time and burst time

	// UI
	cout << "Number of Processes: " << numProc << endl;
	cout << "Arrival Time:" << endl;
	for (int i = 0; i < sortProc.size(); i++)
		cout << " P" << i + 1 << ": " << sortProc[i].arrival << endl;
	cout << "Burst Time:" << endl;
	for (int i = 0; i < sortProc.size(); i++)
		cout << " P" << i + 1 << ": " << sortProc[i].burst << endl;
	cout << "Priority:" << endl;
	for (int i = 0; i < sortProc.size(); i++)
		cout << " P" << i + 1 << ": " << sortProc[i].priority << endl;

	// sort according to arrival time
	sort(sortProc.begin(), sortProc.end(), arrivalSort);

	cout << endl << endl << "PREEMPTIVE" << endl;
	//P_P
	cout << "Gantt Chart" << endl;
	for (int i = sortProc[0].arrival; processing; i++) // i == time units, processing will only break when every burst reaches 0
	{
		for (int j = 0; j < sortProc.size(); j++) {
			if (i == sortProc[j].arrival) { // condition where process meets arrival time
				if (preempt == 0) {
					readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst });
					sort(readMe.begin(), readMe.end(), burstSort);
				}
				else if (preempt == 1) {
					readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst });
				}
			}
		}

		if (currentQueue == -1) // this is to start the queue process if it hasn't started yet (MLQ only)
		{
			currentQueue = readMe[0].order;
			pos = 0;
		}

		if (finProc < numProc) {
			if (readMe.empty()) {
				ganttChart.push_back({ -1, -1, i });
				GanttChart();
			}
			else if (readMe.size() > 0) {
				P_P(i);
				GanttChart();
			}
		}

	}
	Table();

	cout << endl << endl;
	ganttChart.clear();
	gc.clear();
	finProc = 0;
	processing = true;
	currentQueue == -1;
	// sort according to arrival time
	sort(sortProc.begin(), sortProc.end(), arrivalSort);

	cout << "NON-PREEMPTIVE" << endl;
	//P_NP
	cout << "Gantt Chart" << endl;
	for (int i = sortProc[0].arrival; processing; i++) // i == time units, processing will only break when every burst reaches 0
	{
		for (int j = 0; j < sortProc.size(); j++) {
			if (i == sortProc[j].arrival) { // condition where process meets arrival time
				if (preempt == 0) {
					readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst, sortProc[j].priority });
					sort(readMe.begin(), readMe.end(), burstSort);
				}
				else if (preempt == 1) {
					readMe.push_back({ sortProc[j].order, sortProc[j].arrival, sortProc[j].burst, sortProc[j].priority });
				}
			}
		}

		if (currentQueue == -1) // this is to start the queue process if it hasn't started yet
		{
			currentQueue = readMe[0].order;
			pos = 0;
		}

		if (finProc < numProc) {
			if (readMe.empty()) {
				ganttChart.push_back({ -1, -1, i });
				GanttChart();
			}
			else if (readMe.size() > 0) {
				P_NP(i);
				GanttChart();
			}
		}
	}
	Table();

	rerun();
}

void processValues(vector<int> processes)
{
	numProc = processes[0];

	// AT, BT, Priority
	for (int i = 0; i < numProc; i++)
		sortProc.push_back({ (i + 1)
			, (processes[i + 1])
			, (processes[numProc + i + 1])
			, (processes[(numProc * 2) + i + 1]) });

}

void GanttChart() {
	// Box Legend
	char hl = 196; // horizontal line
	char vl = 179; // vertical line
	char ul = 218; // upper left of the box (end)
	char bl = 192; // bottom left of the box (end)
	char ur = 191; // upper right of the box (end)
	char br = 217; // bottom right of the box (end)
	char cu = 194; // connects the upper parts of the box
	char cb = 193; // connects the bottom parts of the box

	if (finProc == numProc) {
		processing = false;

		int gcSize = ganttChart.size();

		// Upper Lines
		cout << ul;
		gc.push_back({ ganttChart[0].process,ganttChart[0].burst,ganttChart[0].time });

		for (int u = 1, check = 0; u < ganttChart.size(); u++) {
			//cout << "P" << ganttChart[u].process << " "; // debug
			if (ganttChart[u].process != ganttChart[u - 1].process) {
				if (ganttChart[u].time - gc[check].time < 4) {
					for (int l = 0; l < 4; l++)
						cout << hl;
					cout << cu;
				}
				else if (ganttChart[u].time - gc[check].time > 3 && ganttChart[u].time - gc[check].time <= 10) {
					for (int l = 0; l < 8; l++)
						cout << hl;
					cout << cu;
				}
				else if (ganttChart[u].time - gc[check].time > 10) {
					for (int l = 0; l < 14; l++)
						cout << hl;
					cout << cu;
				}
				//cout << ganttChart[u].time - gc[check].time << endl;
				check++;
				gc.push_back({ ganttChart[u].process,ganttChart[u].burst,ganttChart[u].time });
			}
		}
		if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time < 4) {
			for (int l = 0; l < 4; l++)
				cout << hl;
			cout << ur;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 3 && ganttChart[gcSize - 1].time - gc[gc.size() - 1].time <= 10) {
			for (int l = 0; l < 8; l++)
				cout << hl;
			cout << ur;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 10) {
			for (int l = 0; l < 14; l++)
				cout << hl;
			cout << ur;
		}
		cout << endl;

		// Processes
		for (int u = 0, check = 0; u < ganttChart.size() - 1; u++) {
			//cout << "P" << ganttChart[u].process << " "; // debug

			if (ganttChart[u].process != ganttChart[u + 1].process) {
				if (ganttChart[u].process == -1) {
					if (ganttChart[u + 1].time - gc[check].time < 4) {
						cout << vl << " NA ";
					}
					else if (ganttChart[u + 1].time - gc[check].time > 3 && ganttChart[u].time - gc[check].time <= 10) {
						cout << vl << "   NA   ";
					}
					else if (ganttChart[u + 1].time - gc[check].time > 10) {
						cout << vl << "      NA      ";
					}
				}
				else {
					if (ganttChart[u + 1].time - gc[check].time < 4) {
						cout << vl << " P" << ganttChart[u].process << " ";
					}
					else if (ganttChart[u + 1].time - gc[check].time > 3 && ganttChart[u].time - gc[check].time <= 10) {
						cout << vl << "   P" << ganttChart[u].process << "   ";
					}
					else if (ganttChart[u + 1].time - gc[check].time > 10) {
						cout << vl << "      P" << ganttChart[u].process << "      ";
					}
				}
				check++;
			}
		}
		if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time < 4) {
			cout << vl << " P" << ganttChart[gcSize - 1].process << " " << vl;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 3 && ganttChart[gcSize - 1].time - gc[gc.size() - 1].time <= 10) {
			cout << vl << "   P" << ganttChart[gcSize - 1].process << "   " << vl;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 10) {
			cout << vl << "      P" << ganttChart[gcSize - 1].process << "      " << vl;
		}
		cout << endl;

		//Bottom Lines
		cout << bl;
		for (int u = 1, check = 0; u < ganttChart.size(); u++) {
			//cout << "P" << ganttChart[u].process << " "; // debug
			if (ganttChart[u].process != ganttChart[u - 1].process) {
				if (ganttChart[u].time - gc[check].time < 4) {
					for (int l = 0; l < 4; l++)
						cout << hl;
					cout << cb;
				}
				else if (ganttChart[u].time - gc[check].time > 3 && ganttChart[u].time - gc[check].time <= 10) {
					for (int l = 0; l < 8; l++)
						cout << hl;
					cout << cb;
				}
				else if (ganttChart[u].time - gc[check].time > 10) {
					for (int l = 0; l < 14; l++)
						cout << hl;
					cout << cb;
				}
				//cout << ganttChart[u].time - gc[check].time << endl;
				check++;
			}
		}
		if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time < 4) {
			for (int l = 0; l < 4; l++)
				cout << hl;
			cout << br;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 3 && ganttChart[gcSize - 1].time - gc[gc.size() - 1].time <= 10) {
			for (int l = 0; l < 8; l++)
				cout << hl;
			cout << br;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 10) {
			for (int l = 0; l < 14; l++)
				cout << hl;
			cout << br;
		}
		cout << endl;

		// num nums (time units)
		for (int u = 0, check = 0; u < ganttChart.size() - 1; u++) {
			//cout << "P" << ganttChart[u].process << " "; // debug
			if (u == 0) {
				cout << ganttChart[u].time;
			}
			if (ganttChart[u].process != ganttChart[u + 1].process) {
				if (ganttChart[u + 1].time - gc[check].time < 4) {
					cout << setw(5) << ganttChart[u + 1].time;
				}
				else if (ganttChart[u + 1].time - gc[check].time > 3 && ganttChart[u].time - gc[check].time <= 10) {
					cout << setw(9) << ganttChart[u + 1].time;
				}
				else if (ganttChart[u + 1].time - gc[check].time > 10) {
					cout << setw(15) << ganttChart[u + 1].time;
				}
				check++;
			}
		}
		if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time < 4) {
			cout << setw(5) << gcSize;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 3 && ganttChart[gcSize - 1].time - gc[gc.size() - 1].time <= 10) {
			cout << setw(9) << gcSize;
		}
		else if (ganttChart[gcSize - 1].time - gc[gc.size() - 1].time > 10) {
			cout << setw(15) << gcSize;
		}
		cout << endl << endl;
	}
}

void Table() {
	// Box Legend
	char hl = 196; // horizontal line
	char vl = 179; // vertical line
	char ul = 218; // upper left of the box (end)
	char bl = 192; // bottom left of the box (end)
	char ur = 191; // upper right of the box (end)
	char br = 217; // bottom right of the box (end)
	char cu = 194; // connects the upper parts of the box
	char cb = 193; // connects the bottom parts of the box
	char cross = 197;
	vector<int> tWaitTbl, ttTbl;
	cout << "Table" << endl;
	sort(sortProc.begin(), sortProc.end(), processSort);

	cout << " Processes " << vl << " Turnaround Time " << vl << "  Waiting Time " << endl;

	// Table Inputs
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

		// Table Output
		cout << "  P" << sortProc[i].order << setw(8) << vl << setw(9) << tTime << setw(9) << vl << setw(10) << wait << endl;
		ttTbl.push_back(tTime);
		tWaitTbl.push_back(wait);
	}

	int totalTT = 0, totalWait = 0;
	for (int t = 0; t < numProc; t++) {
		totalTT += ttTbl[t];
		totalWait += tWaitTbl[t];
	}
	cout << setw(12) << setfill(hl) << cross << setw(18) << cross << setw(18) << "\n" << setfill(' ');
	// Total and Average:
	cout << " Total" << setw(6) << vl << setw(9) << totalTT << setw(9) << vl << setw(10) << totalWait << endl;
	cout << " Average" << setw(4) << vl << setw(9) << (float)totalTT / (float)numProc << setw(9) << vl << setw(10) << (float)totalWait / (float)numProc << endl;
	cout << setfill(' ');
}

/*Priority Preemptive
* The approach is almost the same as SJF, but this code only preempts when the priority of the the process that arrived is higher
*/
int x = 1;
int P_P(int arrived) {
	int next = 0;

	if (readMe.size() > 1) {
		vector<process> prioOne, prioTwo;
		int prioCounter = 0;

		readMe[pos].burst--;
		ganttChart.push_back({ readMe[pos].order, readMe[pos].burst, arrived });

		if (readMe[pos].burst > 0) // if the process still has bursts left
			next = readMe[pos].priority;
		else if (readMe[pos].burst <= 0) { // if the process has no bursts left
			readMe.erase(readMe.begin() + pos);
			finProc++;
			if (pos == readMe.size()) { // when the position reaches greater than what's in the Queue, reset position to 0
				pos = 0;
			}

			if (readMe.size() > 0) { // when there are still processes left
				for (int i = 0; i < readMe.size(); i++) {
					if (readMe[i].priority == 1)
						prioOne.push_back(readMe[i]);
					else if (readMe[i].priority == 2)
						prioTwo.push_back(readMe[i]);
				}

				// sort priority of SJF_NP starting with the lowest burst time
				if (readMe[pos].priority == 1) {
					readMe.clear();
					sort(prioOne.begin(), prioOne.end(), arrivalSort);

					if (prioTwo.size() > 0) {
						for (int i = 0; i < prioOne.size(); i++)
							readMe.push_back(prioOne[i]);
						for (int i = 0; i < prioTwo.size(); i++)
							readMe.push_back(prioTwo[i]);
					}
					else {
						for (int i = 0; i < prioOne.size(); i++)
							readMe.push_back(prioOne[i]);
					}
				}
				else if (readMe[pos].priority == 2) {
					readMe.clear();
					sort(prioTwo.begin(), prioTwo.end(), arrivalSort);

					if (prioOne.size() > 0) {
						for (int i = 0; i < prioOne.size(); i++)
							readMe.push_back(prioOne[i]);
						for (int i = 0; i < prioTwo.size(); i++)
							readMe.push_back(prioTwo[i]);
					}
					else {
						for (int i = 0; i < prioTwo.size(); i++)
							readMe.push_back(prioTwo[i]);
					}
				}
				next = readMe[pos].priority;
			}
		}
	}
	else {
		readMe[pos].burst--;

		ganttChart.push_back({ readMe[pos].order, readMe[pos].burst, arrived });

		if (readMe[pos].burst > 0) // if the process still has bursts left
			next = readMe[pos].priority;
		else if (readMe[pos].burst <= 0) { // if the process has no bursts left
			readMe.erase(readMe.begin() + pos);
			finProc++;

			if (pos == readMe.size()) { // when the position reaches greater than what's in the Queue, reset position to 0
				pos = 0;
			}

			if (readMe.size() > 0) { // when there are still processes left
				next = readMe[pos].priority;
			}
		}
	}

	return next;
}

/*Priority Non-Preemptive*/
int P_NP(int arrived) {
	int next = 0;

	preempt = 1;
	vector<process> prioOne, prioTwo;
	int prioCounter = 0;

	readMe[pos].burst--;
	ganttChart.push_back({ readMe[pos].order, readMe[pos].burst, arrived });

	if (readMe[pos].burst > 0) // if the process still has bursts left
		next = readMe[pos].priority;
	else if (readMe[pos].burst <= 0) { // if the process has no bursts left
		readMe.erase(readMe.begin() + pos);
		finProc++;
		if (pos == readMe.size()) { // when the position reaches greater than what's in the Queue, reset position to 0
			pos = 0;
		}

		preempt = 0;

		if (readMe.size() > 0) { // when there are still processes left
			for (int i = 0; i < readMe.size(); i++) {
				if (readMe[i].priority == 1)
					prioOne.push_back(readMe[i]);
				else if (readMe[i].priority == 2)
					prioTwo.push_back(readMe[i]);
			}

			// sort priority of SJF_NP starting with the lowest burst time
			if (readMe[pos].priority == 1) {
				readMe.clear();
				sort(prioOne.begin(), prioOne.end(), arrivalSort);

				if (prioTwo.size() > 0) {
					for (int i = 0; i < prioOne.size(); i++)
						readMe.push_back(prioOne[i]);
					for (int i = 0; i < prioTwo.size(); i++)
						readMe.push_back(prioTwo[i]);
				}
				else {
					for (int i = 0; i < prioOne.size(); i++)
						readMe.push_back(prioOne[i]);
				}
			}
			else if (readMe[pos].priority == 2) {
				readMe.clear();
				sort(prioTwo.begin(), prioTwo.end(), arrivalSort);

				if (prioOne.size() > 0) {
					for (int i = 0; i < prioOne.size(); i++)
						readMe.push_back(prioOne[i]);
					for (int i = 0; i < prioTwo.size(); i++)
						readMe.push_back(prioTwo[i]);
				}
				else {
					for (int i = 0; i < prioTwo.size(); i++)
						readMe.push_back(prioTwo[i]);
				}
			}
			next = readMe[pos].priority;
		}
	}

	return next;
}

void rerun() {
	char yn;
	cout << endl << "Do you want to run again[y/n]: ";
	cin >> yn;
	yn = tolower(yn);
	if (yn == 'y')
		main();
	else if (yn == 'n')
		exit(1);
	else
		rerun();
}