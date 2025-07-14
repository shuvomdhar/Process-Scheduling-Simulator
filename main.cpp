// Either include the necessary headers or use a single header file

// #include <iostream>
// #include <vector>
// #include <queue>
// #include <algorithm>
// #include <iomanip>
// #include <string>
// #include <map>
// #include <climits>


# include <bits/stdc++.h>

using namespace std;

// Process structure
struct Process
{
    string id;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
    int responseTime;
    bool isStarted;
    int memoryRequired;
    bool memoryAllocated;

    Process(string pid, int at, int bt, int pr, int mem)
        : id(pid), arrivalTime(at), burstTime(bt), priority(pr),
          remainingTime(bt), waitingTime(0), turnaroundTime(0),
          completionTime(0), responseTime(-1), isStarted(false),
          memoryRequired(mem), memoryAllocated(false) {}
};

// Memory management system
class MemoryManager
{
private:
    int totalMemory;
    int availableMemory;
    map<string, int> allocatedMemory;

public:
    MemoryManager(int total) : totalMemory(total), availableMemory(total) {}

    bool allocateMemory(const string &processId, int required)
    {
        if (availableMemory >= required)
        {
            allocatedMemory[processId] = required;
            availableMemory -= required;
            return true;
        }
        return false;
    }

    void deallocateMemory(const string &processId)
    {
        if (allocatedMemory.find(processId) != allocatedMemory.end())
        {
            availableMemory += allocatedMemory[processId];
            allocatedMemory.erase(processId);
        }
    }

    int getAvailableMemory() const { return availableMemory; }

    void displayMemoryStatus() const
    {
        cout << "\n=== Memory Status ===\n";
        cout << "Total Memory: " << totalMemory << " MB\n";
        cout << "Available Memory: " << availableMemory << " MB\n";
        cout << "Allocated Memory:\n";
        for (const auto &alloc : allocatedMemory)
        {
            cout << "  Process " << alloc.first << ": " << alloc.second << " MB\n";
        }
    }
};

// Base scheduler class
class Scheduler
{
protected:
    vector<Process> processes;
    MemoryManager memoryManager;
    vector<string> ganttChart;
    vector<int> ganttTimes;

public:
    Scheduler(int totalMemory) : memoryManager(totalMemory) {}

    void addProcess(const Process &p)
    {
        processes.push_back(p);
    }

    virtual void schedule() = 0;

    void displayResults()
    {
        cout << "\n=== Process Execution Results ===\n";
        cout << setw(8) << "Process" << setw(12) << "Arrival" << setw(10) << "Burst"
             << setw(10) << "Priority" << setw(12) << "Completion" << setw(10) << "Waiting"
             << setw(12) << "Turnaround" << setw(10) << "Response" << setw(10) << "Memory\n";
        cout << string(90, '-') << "\n";

        float avgWaiting = 0, avgTurnaround = 0, avgResponse = 0;

        for (const auto &p : processes)
        {
            cout << setw(8) << p.id << setw(12) << p.arrivalTime << setw(10) << p.burstTime
                 << setw(10) << p.priority << setw(12) << p.completionTime
                 << setw(10) << p.waitingTime << setw(12) << p.turnaroundTime
                 << setw(10) << p.responseTime << setw(10) << p.memoryRequired << "\n";

            avgWaiting += p.waitingTime;
            avgTurnaround += p.turnaroundTime;
            avgResponse += p.responseTime;
        }

        int n = processes.size();
        cout << string(90, '-') << "\n";
        cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWaiting / n << "\n";
        cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTurnaround / n << "\n";
        cout << "Average Response Time: " << fixed << setprecision(2) << avgResponse / n << "\n";
    }

    void displayGanttChart()
    {
        cout << "\n=== Gantt Chart ===\n";

        // Display process names
        cout << "|";
        for (size_t i = 0; i < ganttChart.size(); i++)
        {
            cout << setw(4) << ganttChart[i] << setw(4) << "|";
        }
        cout << "\n";

        // Display time line
        cout << ganttTimes[0];
        for (size_t i = 1; i < ganttTimes.size(); i++)
        {
            cout << setw(8) << ganttTimes[i];
        }
        cout << "\n\n";
    }

    void displayMemoryAllocation()
    {
        memoryManager.displayMemoryStatus();
    }

    virtual ~Scheduler() = default;
};

// First Come First Serve (FCFS) Scheduler
class FCFSScheduler : public Scheduler
{
public:
    FCFSScheduler(int totalMemory) : Scheduler(totalMemory) {}

    void schedule() override
    {
        cout << "\n=== First Come First Serve (FCFS) Scheduling ===\n";

        // Sort by arrival time
        sort(processes.begin(), processes.end(),
             [](const Process &a, const Process &b)
             {
                 return a.arrivalTime < b.arrivalTime;
             });

        int currentTime = 0;
        queue<Process *> readyQueue;
        queue<Process *> waitingQueue; // For memory allocation

        for (auto &p : processes)
        {
            currentTime = max(currentTime, p.arrivalTime);

            // Try to allocate memory
            if (memoryManager.allocateMemory(p.id, p.memoryRequired))
            {
                p.memoryAllocated = true;
                cout << "Time " << currentTime << ": Process " << p.id << " allocated "
                     << p.memoryRequired << " MB memory\n";

                p.responseTime = currentTime - p.arrivalTime;
                p.completionTime = currentTime + p.burstTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;

                ganttChart.push_back(p.id);
                ganttTimes.push_back(currentTime);

                currentTime += p.burstTime;
                ganttTimes.push_back(currentTime);

                cout << "Time " << currentTime << ": Process " << p.id << " completed\n";

                // Deallocate memory
                memoryManager.deallocateMemory(p.id);
                cout << "Time " << currentTime << ": Process " << p.id << " memory deallocated\n";
            }
            else
            {
                cout << "Time " << currentTime << ": Process " << p.id
                     << " waiting for memory allocation\n";
                p.waitingTime += 1;
                currentTime += 1;
            }
        }
    }
};

// Shortest Job First (SJF) Scheduler
class SJFScheduler : public Scheduler
{
public:
    SJFScheduler(int totalMemory) : Scheduler(totalMemory) {}

    void schedule() override
    {
        cout << "\n=== Shortest Job First (SJF) Scheduling ===\n";

        int currentTime = 0;
        vector<bool> completed(processes.size(), false);
        int completedCount = 0;

        while (completedCount < processes.size())
        {
            int shortest = -1;
            int shortestBurst = INT_MAX;

            // Find shortest job that has arrived and can fit in memory
            for (int i = 0; i < processes.size(); i++)
            {
                if (!completed[i] && processes[i].arrivalTime <= currentTime)
                {
                    if (processes[i].burstTime < shortestBurst)
                    {
                        if (memoryManager.getAvailableMemory() >= processes[i].memoryRequired)
                        {
                            shortest = i;
                            shortestBurst = processes[i].burstTime;
                        }
                    }
                }
            }

            if (shortest == -1)
            {
                currentTime++;
                continue;
            }

            Process &p = processes[shortest];

            // Allocate memory
            memoryManager.allocateMemory(p.id, p.memoryRequired);
            p.memoryAllocated = true;

            cout << "Time " << currentTime << ": Process " << p.id << " started\n";

            p.responseTime = currentTime - p.arrivalTime;
            ganttChart.push_back(p.id);
            ganttTimes.push_back(currentTime);

            currentTime += p.burstTime;

            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;

            ganttTimes.push_back(currentTime);
            completed[shortest] = true;
            completedCount++;

            cout << "Time " << currentTime << ": Process " << p.id << " completed\n";

            // Deallocate memory
            memoryManager.deallocateMemory(p.id);
        }
    }
};

// Priority Scheduler
class PriorityScheduler : public Scheduler
{
public:
    PriorityScheduler(int totalMemory) : Scheduler(totalMemory) {}

    void schedule() override
    {
        cout << "\n=== Priority Scheduling ===\n";

        int currentTime = 0;
        vector<bool> completed(processes.size(), false);
        int completedCount = 0;

        while (completedCount < processes.size())
        {
            int highestPriority = -1;
            int highestPriorityValue = INT_MAX;

            // Find highest priority job (lower number = higher priority)
            for (int i = 0; i < processes.size(); i++)
            {
                if (!completed[i] && processes[i].arrivalTime <= currentTime)
                {
                    if (processes[i].priority < highestPriorityValue)
                    {
                        if (memoryManager.getAvailableMemory() >= processes[i].memoryRequired)
                        {
                            highestPriority = i;
                            highestPriorityValue = processes[i].priority;
                        }
                    }
                }
            }

            if (highestPriority == -1)
            {
                currentTime++;
                continue;
            }

            Process &p = processes[highestPriority];

            // Allocate memory
            memoryManager.allocateMemory(p.id, p.memoryRequired);
            p.memoryAllocated = true;

            cout << "Time " << currentTime << ": Process " << p.id
                 << " (Priority: " << p.priority << ") started\n";

            p.responseTime = currentTime - p.arrivalTime;
            ganttChart.push_back(p.id);
            ganttTimes.push_back(currentTime);

            currentTime += p.burstTime;

            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;

            ganttTimes.push_back(currentTime);
            completed[highestPriority] = true;
            completedCount++;

            cout << "Time " << currentTime << ": Process " << p.id << " completed\n";

            // Deallocate memory
            memoryManager.deallocateMemory(p.id);
        }
    }
};

// Round Robin Scheduler
class RoundRobinScheduler : public Scheduler
{
private:
    int timeQuantum;

public:
    RoundRobinScheduler(int totalMemory, int quantum)
        : Scheduler(totalMemory), timeQuantum(quantum) {}

    void schedule() override
    {
        cout << "\n=== Round Robin Scheduling (Quantum: " << timeQuantum << ") ===\n";

        queue<int> readyQueue;
        int currentTime = 0;
        vector<bool> inQueue(processes.size(), false);
        int completedCount = 0;

        // Add processes that arrive at time 0
        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].arrivalTime <= currentTime)
            {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        while (completedCount < processes.size())
        {
            if (readyQueue.empty())
            {
                currentTime++;
                // Add newly arrived processes
                for (int i = 0; i < processes.size(); i++)
                {
                    if (!inQueue[i] && processes[i].arrivalTime <= currentTime)
                    {
                        readyQueue.push(i);
                        inQueue[i] = true;
                    }
                }
                continue;
            }

            int currentProcess = readyQueue.front();
            readyQueue.pop();
            Process &p = processes[currentProcess];

            // Allocate memory if not already allocated
            if (!p.memoryAllocated)
            {
                if (memoryManager.allocateMemory(p.id, p.memoryRequired))
                {
                    p.memoryAllocated = true;
                    cout << "Time " << currentTime << ": Process " << p.id
                         << " allocated " << p.memoryRequired << " MB memory\n";
                }
                else
                {
                    cout << "Time " << currentTime << ": Process " << p.id
                         << " waiting for memory\n";
                    readyQueue.push(currentProcess);
                    currentTime++;
                    continue;
                }
            }

            // Set response time if first time running
            if (p.responseTime == -1)
            {
                p.responseTime = currentTime - p.arrivalTime;
            }

            int executionTime = min(timeQuantum, p.remainingTime);

            cout << "Time " << currentTime << ": Process " << p.id
                 << " executing for " << executionTime << " units\n";

            ganttChart.push_back(p.id);
            ganttTimes.push_back(currentTime);

            currentTime += executionTime;
            p.remainingTime -= executionTime;

            ganttTimes.push_back(currentTime);

            // Add newly arrived processes
            for (int i = 0; i < processes.size(); i++)
            {
                if (!inQueue[i] && processes[i].arrivalTime <= currentTime &&
                    processes[i].remainingTime > 0)
                {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }

            // Check if process completed
            if (p.remainingTime == 0)
            {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                completedCount++;

                cout << "Time " << currentTime << ": Process " << p.id << " completed\n";

                // Deallocate memory
                memoryManager.deallocateMemory(p.id);
                inQueue[currentProcess] = false;
            }
            else
            {
                // Add back to ready queue
                readyQueue.push(currentProcess);
            }
        }
    }
};

// Main application
class ProcessSchedulingSimulator
{
public:
    void run()
    {
        cout << "=== Process Scheduling Algorithms Simulator ===\n\n";

        // Sample processes
        vector<Process> sampleProcesses = {
            Process("A", 0, 4, 2, 100),
            Process("B", 1, 3, 1, 200),
            Process("C", 2, 1, 4, 150),
            Process("D", 3, 5, 3, 300),
            Process("E", 4, 2, 1, 100)};

        int totalMemory = 1000; // MB

        // Test different scheduling algorithms
        testSchedulingAlgorithm(sampleProcesses, totalMemory, 1); // FCFS
        testSchedulingAlgorithm(sampleProcesses, totalMemory, 2); // SJF
        testSchedulingAlgorithm(sampleProcesses, totalMemory, 3); // Priority
        testSchedulingAlgorithm(sampleProcesses, totalMemory, 4); // Round Robin

        cout << "\n=== Interactive Mode ===\n";
        interactiveMode();
    }

private:
    void testSchedulingAlgorithm(const vector<Process> &processes, int totalMemory, int algorithm)
    {
        unique_ptr<Scheduler> scheduler;

        switch (algorithm)
        {
        case 1:
            scheduler = make_unique<FCFSScheduler>(totalMemory);
            break;
        case 2:
            scheduler = make_unique<SJFScheduler>(totalMemory);
            break;
        case 3:
            scheduler = make_unique<PriorityScheduler>(totalMemory);
            break;
        case 4:
            scheduler = make_unique<RoundRobinScheduler>(totalMemory, 2);
            break;
        }

        // Add processes to scheduler
        for (const auto &p : processes)
        {
            scheduler->addProcess(p);
        }

        // Run scheduling
        scheduler->schedule();
        scheduler->displayGanttChart();
        scheduler->displayResults();
        scheduler->displayMemoryAllocation();

        cout << "\n"
             << string(80, '=') << "\n";
    }

    void interactiveMode()
    {
        cout << "Enter your own processes to schedule:\n";

        int numProcesses;
        cout << "Number of processes: ";
        cin >> numProcesses;

        int totalMemory;
        cout << "Total memory (MB): ";
        cin >> totalMemory;

        vector<Process> userProcesses;

        for (int i = 0; i < numProcesses; i++)
        {
            string id;
            int arrival, burst, priority, memory;

            cout << "Process " << (i + 1) << ":\n";
            cout << "  ID: ";
            cin >> id;
            cout << "  Arrival Time: ";
            cin >> arrival;
            cout << "  Burst Time: ";
            cin >> burst;
            cout << "  Priority (1=highest): ";
            cin >> priority;
            cout << "  Memory Required (MB): ";
            cin >> memory;

            userProcesses.emplace_back(id, arrival, burst, priority, memory);
        }

        int choice;
        cout << "\nChoose scheduling algorithm:\n";
        cout << "1. First Come First Serve (FCFS)\n";
        cout << "2. Shortest Job First (SJF)\n";
        cout << "3. Priority Scheduling\n";
        cout << "4. Round Robin\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 4)
        {
            int quantum;
            cout << "Enter time quantum: ";
            cin >> quantum;
            auto scheduler = make_unique<RoundRobinScheduler>(totalMemory, quantum);

            for (const auto &p : userProcesses)
            {
                scheduler->addProcess(p);
            }

            scheduler->schedule();
            scheduler->displayGanttChart();
            scheduler->displayResults();
            scheduler->displayMemoryAllocation();
        }
        else
        {
            testSchedulingAlgorithm(userProcesses, totalMemory, choice);
        }
    }
};

int main()
{
    ProcessSchedulingSimulator simulator;
    simulator.run();
    return 0;
}