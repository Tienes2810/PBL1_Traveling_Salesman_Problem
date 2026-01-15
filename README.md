# 🗺️ Traveling Salesman Problem (TSP) Solver

[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![SDL2](https://img.shields.io/badge/Graphics-SDL2-green.svg)](https://www.libsdl.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## 📋 About

The **Traveling Salesman Problem (TSP)** is a Project-Based Learning (PBL1) assignment for the Introduction to Programming course at **Da Nang University of Science and Technology**. This application solves the TSP optimization problem with a graphical user interface using the SDL2 library.

### 🎯 Project Objectives
- Solve the TSP optimization problem (finding the shortest route visiting all cities)
- Visualize algorithms and results with a graphical interface
- Implement Branch and Bound and Dynamic Programming algorithms

## 👥 Team Members

**Team Leader:** Nguyen Tien  
**Member:** Tran Minh Truong

## ✨ Features

- ✅ Read cost matrix data from input files
- ✅ Solve TSP using algorithms:
  - Branch and Bound
  - Dynamic Programming with Bitmask
- ✅ SDL2 graphical interface displaying:
  - Cost matrix
  - Optimal route
  - Minimum cost
- ✅ Export results to output file
- ✅ Support up to 20 cities

## 🛠️ Technologies

- **Language:** C
- **Graphics Libraries:** SDL2, SDL2_ttf, SDL2_image
- **Build Tools:** GCC, Make
- **IDE:** Visual Studio Code

## 📦 Installation

### System Requirements
- Windows OS
- GCC compiler (MinGW)
- SDL2 development libraries

### Installation Steps

1. **Clone repository:**
```bash
git clone https://github.com/Tienes2810/PBL1_Traveling_Salesman_Problem.git
cd PBL1_Traveling_Salesman_Problem
```

2. **Install SDL2:**
   - SDL2 libraries are included in the `src/lib` directory
   - Ensure `.dll` files (SDL2.dll, SDL2_ttf.dll, SDL2_image.dll) are in the root directory

3. **Compile the program:**
```bash
make build
```

4. **Run the program:**
```bash
make run
```

or:
```bash
./main.exe
```

## 📖 User Guide

### Input File Format
Create an `in_put.txt` file with the following format:
```
n
c[0][0] c[0][1] ... c[0][n-1]
c[1][0] c[1][1] ... c[1][n-1]
...
c[n-1][0] c[n-1][1] ... c[n-1][n-1]
```

Where:
- `n`: Number of cities (3 ≤ n ≤ 20)
- `c[i][j]`: Cost of traveling from city i to city j
- `c[i][i] = 0`: Cost of traveling from a city to itself = 0
- `c[i][j] = 0`: No direct route between i and j

### Example Input (4 cities):
```
4
0 10 15 20
10 0 35 25
15 35 0 30
20 25 30 0
```

### Running the Program:
1. Place the input file in the root directory
2. Run `./main.exe`
3. Use the GUI to select input file and view results
4. Results will be saved to output file

## 📊 Algorithms

### 1. Branch and Bound
- Depth-first search with pruning strategy
- Uses lower bound to eliminate non-optimal branches
- Time complexity: O(n!)

### 2. Dynamic Programming with Bitmask
- Uses dynamic programming with bitmask to track visited states
- Recurrence relation: `dp[mask][i]` = minimum cost when visited cities in mask and ending at i
- Time complexity: O(n² × 2ⁿ)

## 📁 Project Structure

```
PBL1/
├── main.c                 # Main source code
├── pbl1nowing.c          # Non-GUI version
├── Makefile              # Build script
├── README.md             # Documentation
├── in_put*.txt           # Sample input files
├── out_put*.txt          # Output result files
├── Arial.TTF             # Font for SDL2_ttf
├── logo.jpeg             # Project logo
├── SDL2*.dll             # SDL2 runtime libraries
└── src/
    ├── include/SDL2/     # SDL2 header files
    └── lib/              # SDL2 library files
```

## 🎮 Demo

The program provides a graphical interface to:
- Select input files
- Display cost matrix
- Visualize optimal route
- Show minimum total cost

## 📝 References

- [Traveling Salesman Problem - Wikipedia](https://en.wikipedia.org/wiki/Travelling_salesman_problem)
- [SDL2 Documentation](https://wiki.libsdl.org/)
- Introduction to Programming Course - Da Nang University of Science and Technology

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Instructor: [Instructor Name]
- Faculty of Information Technology - Da Nang University of Science and Technology
- SDL2 Developer Community

## 📧 Contact

- **Email:** nguyentien281006@gmail.com
- **GitHub:** [@Tienes2810](https://github.com/Tienes2810)

---

⭐ If you find this project useful, please give us a star!
