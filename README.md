# ⚡ Mini-Redis (C++20)

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-square&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg?style=flat-square&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-lightgrey.svg?style=flat-square&logo=linux)
![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)

A high-performance, multithreaded, in-memory key-value store engineered from scratch in **Modern C++ (C++20)**. Designed as an elite portfolio piece to showcase low-level systems programming, concurrent synchronization primitives, and custom TCP socket network architecture without external framework dependencies.

---

## 🚀 Key Features

*   **Modern C++20 Standards**: Leverages cutting-edge features including `std::jthread` for RAII-compliant thread management, `std::optional` for safe null handling, and explicit memory safety constraints.
*   **Optimized Reader-Writer Concurrency**: Implements `std::shared_mutex` to allow high-throughput concurrent reads while maintaining exclusive, race-free write locking.
*   **Custom TCP Networking Engine**: Built natively from the ground up using POSIX system calls (`socket`, `bind`, `listen`, `accept`) to handle live client streams concurrently.
*   **Redis-Compatible Protocol Simulation**: Parses standard core commands (`SET`, `GET`, `DEL`, `QUIT`) with robust line-buffering and error handling.

---

## 🛠️ Tech Stack & Tools

| Component | Technology | Description |
| :--- | :--- | :--- |
| **Language** | C++20 | Core application logic and type-safe abstractions |
| **Build System** | CMake (3.20+) | Cross-platform build configuration and compiler optimization flags |
| **Concurrency** | `<shared_mutex>`, `<atomic>`, `<thread>` | Multi-threaded synchronization and cooperative thread lifecycle management |
| **Networking** | POSIX Sockets | Low-level TCP/IP stream communication layer |

---

## 🏗️ Project Architecture

```text
mini-redis/
├── CMakeLists.txt         # Root build configuration & compiler strictness flags
├── include/
│   ├── server.hpp         # TCP socket lifecycle and connection dispatcher interface
│   └── store.hpp          # Thread-safe in-memory key-value data container interface
└── src/
    ├── main.cpp           # Server entry point and runtime exception guard
    ├── server.cpp         # POSIX network binding, listening, and jthread handler
    └── store.cpp          # Reader-writer mutex locking implementation

# 1. Clone the repository
git clone [https://github.com/fahadalmaashani/mini-redis.git](https://github.com/fahadalmaashani/mini-redis.git)
cd mini-redis

# 2. Create and enter the build directory
mkdir build && cd build

# 3. Configure the project with CMake in Release mode
cmake -DCMAKE_BUILD_TYPE=Release ..

# 4. Build all targets using available CPU cores
cmake --build . -j$(nproc)

# 5. Run the compiled Mini-Redis server
./mini_redis

# Open a separate terminal and connect to the server
nc localhost 6379

SET session_token auth_xyz987
OK
GET session_token
"auth_xyz987"
DEL session_token
(integer) 1
GET session_token
(nil)
QUIT
BYE
