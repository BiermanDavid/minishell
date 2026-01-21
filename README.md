# 🐚 Minishell — Summary
A project focused on building a small, functional shell that behaves similarly to Bash.  
**Final Grade:** ⭐ *100/100*

---

## **Project Objectives**
The goal of this project was to create a fully working shell capable of parsing commands, handling redirections and pipes, managing environment variables, and executing both built‑ins and external programs. The project required understanding how processes, file descriptors, and signals work together to form a command‑line interface. It also involved building a reliable parser and execution engine that behaves closely to Bash while staying within the project’s constraints.

---

## **Learning Outcome**
Working on Minishell taught me how shells actually work under the hood. I learned how to parse user input, manage tokens, and build execution trees. I also learned how to fork processes, redirect input and output, and chain commands together using pipes. Handling signals like ctrl‑C, ctrl‑D, and ctrl‑\ helped me understand how interactive programs manage terminal behavior. Overall, this project strengthened my understanding of process control, file descriptors, and building a stable, interactive command‑line program.

---

## **Skills Learned / Improved**
- Process creation and management  
- File descriptors and redirections  
- Pipes and inter‑process communication  
- Command parsing and tokenization  
- Environment variable handling  
- Signal handling  
- Built‑in command implementation  

---

## **Project Exercises**
All required features and behaviors implemented for the project:

- ✔️ `minishell` program  
- ✔️ Interactive prompt  
- ✔️ Command history via `readline`  
- ✔️ Execution of external programs via PATH or absolute/relative paths  
- ✔️ Single global variable only for signal handling  
- ✔️ Proper handling of single and double quotes  
- ✔️ Environment variable expansion (`$VAR` and `$?`)  
- ✔️ Redirections:  
  - `<` input redirection  
  - `>` output redirection  
  - `<<` heredoc  
  - `>>` append redirection  
- ✔️ Pipes (`|`) connecting commands  
- ✔️ Correct behavior for ctrl‑C, ctrl‑D, ctrl‑\  
- ✔️ Built‑ins implemented:  
  - `echo -n`  
  - `cd`  
  - `pwd`  
  - `export`  
  - `unset`  
  - `env`  
  - `exit`  
- ✔️ No memory leaks in user‑written code  
- ✔️ Uses only allowed system calls and libft functions  

