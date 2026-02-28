# Linux System Calls Suite

A C exercise showcasing Linux process management, including process creation, execution and basic inter-process communication.

## Features
* **Compile & Run**: Automates `gcc` compilation and execution using `fork` and `exec`.
* **Mini-Shell**: A lightweight interactive shell for executing system commands.
* **File IPC**: Demonstrates parent-to-child communication using file descriptors.

## Compilation
Use `gcc` to compile the main source file:
```bash
gcc -o sys_suite main.c
