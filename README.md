# **GazaShell**

A simple implementation of a Unix-like shell in C++.

---

## **Description**  
GazaShell is a lightweight shell with support for essential features:  

1. **Running Commands:** Execute system commands directly.  
2. **Directory Navigation:** Change the current directory using the `cd` command.  
3. **I/O Redirection:** Redirect input and output of commands to/from files.  
4. **Pipelines:** Run multiple commands connected by pipelines (`|`).  
5. **Command History:** Execute the last command using the `!!` command.  
6. **Execution Timing:** Measure the time taken by a command to execute.  
7. **Path Display:** Show the last three directories in the current path.

---

## **Concepts Applied**  

- **System Calls:**  
  - `fork()`, `execvp()`, `wait()`, `pipe()`, `dup2()`, `open()`, `close()`, `chdir()`  

- **File Descriptors:**  
  - `STDIN_FILENO`, `STDOUT_FILENO`  

- **Error Handling:**  
  - Use of `perror()` for error reporting.  

- **Inter-Process Communication:**  
  - Use of `pipe()` for managing command pipelines.  
