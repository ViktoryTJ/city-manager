# City Manager – Phase 1 & Phase 2

## 📌 Description
City Manager is a UNIX-based CLI application written in C for managing urban infrastructure reports.  
It allows inspectors to create reports and managers to control and monitor districts.

The system uses low-level system calls, file management, and inter-process communication.

---

## 👥 Roles

### 🔹 Inspector
- Add reports to a district
- View and filter reports

### 🔹 Manager
- Create districts
- Remove reports
- Update configuration (e.g. thresholds)

---

## 🧱 Phase 1 – File System Management

### ✔️ Features
- Create district directories (--add <district>)
- Store reports in binary file (reports.dat)
- Store configuration in district.cfg
- Maintain logs in logged_district
- Enforce permissions using UNIX file modes
- View and filter reports

### 📂 Structure

district/
├── reports.dat
├── district.cfg
└── logged_district

---

## ⚙️ Phase 2 – Monitor Process & Signals

### ✔️ Features
- Separate monitor process
- Monitor writes its PID to .monitor_pid
- Main application sends SIGUSR1 signal when a report is added
- Monitor receives signal and prints:
  New report added

### 🔄 Workflow
1. Start monitor process
2. Add a report from main program
3. Main program sends signal
4. Monitor reacts in real-time

---

## 🛠️ Compilation

mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make

---

## ▶️ Usage

### Start monitor (Terminal 1)
./monitor

### Create a district
./city_manager --role manager --user admin --add downtown

### Add a report
./city_manager --role inspector --user viktor --add_report downtown

### List reports
./city_manager --role inspector --user viktor --list downtown

---

## 📡 Signal Example

When adding a report:

Terminal 2:
Monitor notified

Terminal 1:
New report added

---

## 🧾 Logging

All actions are recorded in:
logged_district

Format:
timestamp | role | user | action

---

## 🚫 Ignored Files

- cmake-build-debug/
- .idea/
- .monitor_pid
- generated districts (e.g. downtown/)

---

## ✅ Status

- Phase 1: ✔️ Completed  
- Phase 2: ✔️ Completed  

---

## 👤 Author

Viktor Janosy
