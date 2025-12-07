# 🔄 IPC Ring Buffer Synchronization

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6.svg?style=flat&logo=windows)
![Build](https://img.shields.io/badge/Build-CMake-064F8C.svg?style=flat&logo=cmake)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A robust implementation of **Inter-Process Communication (IPC)** using a shared binary file acting as a **Circular Buffer (FIFO)**. The system ensures thread-safe and process-safe data transfer using Windows API synchronization primitives.

---

## 🚀 Key Features

*   **Architecture:** Separation of concerns using a dedicated `Core` static library.
*   **Synchronization:** Usage of **Mutex** (file access) and **Semaphores** (empty/full slots tracking).
*   **Data Structure:** Persistent **Circular Queue** stored in a binary file.
*   **Build System:** Modern **CMake** configuration for easy compilation.
*   **Quality Assurance:** Integrated **Unit & Integration Tests** covering circular logic and data integrity.
*   **Standard Layout:** Professional project structure (`include`, `src`, `tests`).

---

## 📂 Project Structure

```text
Lab4_Sync/
├── CMakeLists.txt       # Build configuration
├── include/             # Public interface (Headers)
│   ├── core.h           # Core logic class definition
│   └── logic.h          # Shared constants and structures
├── src/                 # Implementation (Source)
│   ├── core.cpp         # Low-level Windows API wrapper
│   ├── Receiver.cpp     # Consumer process (Entry point)
│   └── Sender.cpp       # Producer process (Entry point)
└── tests/               # Automated testing
    └── Test.cpp         # Unit tests for queue logic
```

---

## 🛠️ How it Works

The system consists of two main processes communicating through a shared file `data.bin`.

### 1. File Layout
The binary file is structured to persist the state of the queue:

| Offset | Size | Description |
| :--- | :--- | :--- |
| `0 bytes` | `4 bytes` | **Write Index** (Where to write next) |
| `4 bytes` | `4 bytes` | **Read Index** (Where to read next) |
| `8 bytes` | `20 bytes` | Message 1 |
| `28 bytes` | `20 bytes` | Message 2 |
| `...` | `...` | ... |

### 2. Synchronization Logic
*   **Mutex (`Lab4_Mutex`):** Ensures only one process writes to or reads from the file at a time.
*   **Semaphore `Empty`:** Tracks available slots. `Sender` waits on this.
*   **Semaphore `Full`:** Tracks messages ready to read. `Receiver` waits on this.

---

## ⚙️ Building the Project

Ensure you have **CMake** and a C++ Compiler (MinGW or MSVC) installed.

1.  Clone the repository:
    ```bash
    git clone https://github.com/YourUsername/Lab4_Sync.git
    cd Lab4_Sync
    ```

2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

3.  Compile:
    ```bash
    cmake ..
    cmake --build .
    ```

---

## 🎮 Usage

### 1. Start the Receiver
This process initializes the file and the synchronization objects.
```powershell
./Receiver.exe
# Follow prompts:
# Enter filename: data.bin
# Enter records count: 5
# Enter senders count: 2
```

### 2. Start Senders
The Receiver will automatically launch `Sender.exe` instances. However, you can also run them manually:
```powershell
./Sender.exe data.bin
```

### 3. Interaction
*   **Sender:** Select `1` to write a message. If the queue is full, it will wait.
*   **Receiver:** Select `1` to read a message. This frees up space for Senders.

---

## 🧪 Running Tests

The project includes an automated test suite to verify the logic without manual interaction.

Run the test executable from the build folder:
```powershell
./TestQueue.exe
```

**Expected Output:**
```text
=== STARTING UNIT TESTS ===
[RUNNING] testSimpleTransfer... PASSED
[RUNNING] testCircularLogic... PASSED
[RUNNING] testDataIntegrity... PASSED
=== ALL TESTS PASSED SUCCESSFULLY ===
```

---

## 👨‍💻 Author

Student of BSU FAMCS
Project for "Operating Systems" Course (Lab 4).