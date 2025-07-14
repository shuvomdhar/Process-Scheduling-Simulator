# Process Scheduling Algorithms Simulator

A comprehensive C++ implementation of various operating system process scheduling algorithms with integrated memory management and visualization capabilities.

## 📋 Table of Contents

- [Features](#features)
- [Algorithms Implemented](#algorithms-implemented)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Program Structure](#program-structure)
- [Sample Output](#sample-output)
- [Contributing](#contributing)
- [License](#license)

## 🚀 Features

- **Multiple Scheduling Algorithms**: FCFS, SJF, Priority, and Round Robin
- **Memory Management**: Integrated memory allocation and deallocation simulation
- **Visual Gantt Charts**: Timeline visualization of process execution
- **Comprehensive Metrics**: Calculation of waiting time, turnaround time, and response time
- **Interactive Mode**: Custom process input and algorithm selection
- **Real-time Feedback**: Process state changes and memory status updates
- **Performance Analysis**: Average performance metrics for comparison

## 🔧 Algorithms Implemented

### 1. First Come First Serve (FCFS)
- **Type**: Non-preemptive
- **Strategy**: Processes are scheduled in order of arrival
- **Pros**: Simple implementation, fair for long processes
- **Cons**: Can cause convoy effect

### 2. Shortest Job First (SJF)
- **Type**: Non-preemptive
- **Strategy**: Selects process with shortest burst time
- **Pros**: Optimal average waiting time
- **Cons**: Starvation of longer processes

### 3. Priority Scheduling
- **Type**: Non-preemptive
- **Strategy**: Schedules based on process priority (lower number = higher priority)
- **Pros**: Important processes get priority
- **Cons**: Starvation of low-priority processes

### 4. Round Robin
- **Type**: Preemptive
- **Strategy**: Time quantum-based fair scheduling
- **Pros**: Fair CPU sharing, good response time
- **Cons**: Context switching overhead

## 📦 Prerequisites

- C++ compiler (GCC, Clang, or MSVC)
- C++11 or later standard support
- Standard C++ libraries (iostream, vector, queue, algorithm, etc.)

## 🛠️ Installation

1. **Clone or download the source code**
   ```bash
   git clone <repository-url>
   cd process-scheduling-simulator
   ```

2. **Compile the program**
   ```bash
   g++ -std=c++11 -o scheduler scheduler.cpp
   ```

   Or with additional optimizations:
   ```bash
   g++ -std=c++11 -O2 -Wall -o scheduler scheduler.cpp
   ```

3. **Run the executable**
   ```bash
   ./scheduler
   ```

## 🎮 Usage

### Automatic Demo Mode
The program starts with a demonstration using predefined sample processes:
- Process A: Arrival=0, Burst=4, Priority=2, Memory=100MB
- Process B: Arrival=1, Burst=3, Priority=1, Memory=200MB
- Process C: Arrival=2, Burst=1, Priority=4, Memory=150MB
- Process D: Arrival=3, Burst=5, Priority=3, Memory=300MB
- Process E: Arrival=4, Burst=2, Priority=1, Memory=100MB

### Interactive Mode
After the demo, you can input your own processes:

1. **Enter number of processes**
2. **Specify total system memory (MB)**
3. **For each process, provide:**
   - Process ID (e.g., "A", "B", "P1", etc.)
   - Arrival time
   - Burst time
   - Priority (1 = highest priority)
   - Memory required (MB)
4. **Select scheduling algorithm**
5. **View results and analysis**

### Example Interactive Session
```
Enter your own processes to schedule:
Number of processes: 3
Total memory (MB): 500

Process 1:
  ID: X
  Arrival Time: 0
  Burst Time: 5
  Priority (1=highest): 1
  Memory Required (MB): 100

Process 2:
  ID: Y
  Arrival Time: 2
  Burst Time: 3
  Priority (1=highest): 2
  Memory Required (MB): 150

Process 3:
  ID: Z
  Arrival Time: 4
  Burst Time: 2
  Priority (1=highest): 1
  Memory Required (MB): 200

Choose scheduling algorithm:
1. First Come First Serve (FCFS)
2. Shortest Job First (SJF)
3. Priority Scheduling
4. Round Robin
Choice: 4
Enter time quantum: 2
```

## 🏗️ Program Structure

### Core Classes

```cpp
struct Process
├── Process information (ID, times, priority, memory)
├── Execution metrics (waiting, turnaround, response times)
└── Memory allocation status

class MemoryManager
├── allocateMemory()
├── deallocateMemory()
├── getAvailableMemory()
└── displayMemoryStatus()

class Scheduler (Abstract Base)
├── addProcess()
├── schedule() [Pure Virtual]
├── displayResults()
├── displayGanttChart()
└── displayMemoryAllocation()

class FCFSScheduler : public Scheduler
class SJFScheduler : public Scheduler
class PriorityScheduler : public Scheduler
class RoundRobinScheduler : public Scheduler

class ProcessSchedulingSimulator
├── run()
├── testSchedulingAlgorithm()
└── interactiveMode()
```

## 📊 Sample Output

### Gantt Chart Example
```
=== Gantt Chart ===
|   A   |   B   |   C   |   D   |   E   |
0       4       7       8      13      15
```

### Results Table Example
```
=== Process Execution Results ===
Process    Arrival     Burst  Priority  Completion   Waiting  Turnaround  Response    Memory
----------------------------------------------------------------------------------------
A                0         4         2          4         0           4         0       100
B                1         3         1          7         3           6         3       200
C                2         1         4          8         5           6         5       150
D                3         5         3         13         5           10        5       300
E                4         2         1         15         9           11        9       100
----------------------------------------------------------------------------------------
Average Waiting Time: 4.40
Average Turnaround Time: 7.40
Average Response Time: 4.40
```

### Memory Status Example
```
=== Memory Status ===
Total Memory: 1000 MB
Available Memory: 150 MB
Allocated Memory:
  Process A: 100 MB
  Process B: 200 MB
  Process D: 300 MB
  Process E: 100 MB
```

## 🔍 Key Metrics Explained

- **Arrival Time**: When the process arrives in the system
- **Burst Time**: CPU time required by the process
- **Priority**: Process priority (1 = highest, higher numbers = lower priority)
- **Completion Time**: When the process finishes execution
- **Waiting Time**: Time spent waiting in the ready queue
- **Turnaround Time**: Total time from arrival to completion
- **Response Time**: Time from arrival to first CPU allocation
- **Memory**: Memory required by the process (MB)

## 🎯 Educational Value

This simulator is perfect for:
- **Operating Systems students** learning scheduling algorithms
- **Computer Science educators** demonstrating scheduling concepts
- **System administrators** understanding process management
- **Developers** interested in OS internals

## 🤝 Contributing

Contributions are welcome! Areas for improvement:
- Additional scheduling algorithms (Multilevel Queue, CFS, etc.)
- GUI implementation
- Performance optimizations
- Extended memory management features
- Process synchronization simulation

## 📄 License

This project is open source and available under the MIT License.

## 📞 Support

For questions, issues, or suggestions:
- Create an issue in the repository
- Contact the maintainers
- Check the documentation

---

**Note**: This simulator is designed for educational purposes to understand process scheduling concepts. It simplifies many aspects of real operating system schedulers for clarity and learning.