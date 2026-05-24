> *This project has been created as part of the 42 curriculum by anematol and kmonjard*

# 🐚 Shelld0n

## 📜 Description

The goal of this project is to build a functioning shell from scratch that mimics the core functionalities of GNU Bash. This involves parsing user input, managing system processes, and handling the core mechanics of a Unix command-line interface.

**Key Features:**
* Working pipelines (`|`) to chain commands together.
* Working redirections `<` Infile, `>` Outfile, `<<` Heredocs, `>>` Append.
* Working relative and absolute executions.
* Environment variable expansion and handling quotes (e.g., `$USER`).
* Directory navigation and core built-in commands.
  * `env`,`echo` with the -n option, `unset`, `export`, `cd`, `pwd`.

## Getting Started

### Compilation
Navigate to the root folder of the project and compile the executable using the provided `Makefile`:

```
make
```

*Other rules include:* `re`,`fclean`,`clean`

### Execution
Once compiled, launch the interactive shell environment:
```
./minishell
```

> The shell does not take any arguments.

## Resources

Here are some of the excellent resources we used to understand the underlying mechanics of a shell, including REPL loops, environment variables, and signal handling:

**Video Tutorials & Explanations:**
* [onaecO](https://www.youtube.com/@onaecO)
* [Jacob Sorber](https://www.youtube.com/@JacobSorber)
* [CodeVault](https://www.youtube.com/@CodeVault)

**Technical Documentation:**
* [Linux Man Pages (man7.org)](https://man7.org/linux/man-pages/) - Essential for looking up POSIX system functions and C library behavior.

## AI Usage Statement

Every line of code in this repository is original and has been written entirely by us. AI tools were utilized strictly for two purposes:
1.  Gaining a conceptual understanding of complex operating system topics at the beginning of the project.
2.  Assisting in diagnosing and debugging stubborn runtime errors during the final testing phases.
