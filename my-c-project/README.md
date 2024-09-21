# My C Project

## Overview
This project is a task management application written in C. It allows users to manage tasks through a command-line interface, with functionalities to add, update, and remove tasks stored in a CSV file.

## Project Structure
```
my-c-project
├── src
│   ├── main.c        # Entry point of the application
│   ├── task.c        # Implementation of task management functions
│   └── task.h        # Header file for task management
├── Makefile          # Build instructions
└── README.md         # Project documentation
```

## Files Description

- **src/main.c**: 
  - Initializes the application.
  - Handles user input.
  - Calls functions to manage tasks.

- **src/task.c**: 
  - Contains the implementation of task management functions.
  - Includes efficient functions for reading from and writing to a CSV file.
  - Implements a robust find and replace function to update tasks without corrupting the CSV file.

- **src/task.h**: 
  - Declares functions and data structures used in `task.c`.
  - Includes prototypes for adding, updating, and removing tasks.

- **Makefile**: 
  - Contains build instructions for compiling the source files and linking them into an executable.

## Setup Instructions
1. Clone the repository:
   ```
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```
   cd my-c-project
   ```
3. Build the project using the Makefile:
   ```
   make
   ```

## Usage
- Run the application:
  ```
  ./my-c-project
  ```
- Follow the on-screen instructions to manage tasks.

## Dependencies
- C compiler (e.g., GCC)
- Standard C library

## License
This project is licensed under the MIT License. See the LICENSE file for details.