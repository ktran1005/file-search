# File Search

## Overview

Searching files for  specific string of text is a common problem. Each file can be easily searching in its own thread. Spanning hundreds of threads to search hundreds of files would not be a good plan. This is a problem where a thread pool can come in handy. We can search some files concurrently, but we don't want to search all files concurrently.

## Concurrent File Search
The program takes two command line inputs:

- `target` - a string of text to search for
- `dir` - A directory to search

The program will find every line of every file in the directory (and all subdirectories) that include the target line. It will search all subdirectories, not just the primary directory. 
**Note**: The program assumes that no one will create a sym-link cycle in the directory.

The program cannot run if a target is not given. If a directory is not given, use the current directory as the starting point.

The executable file will be called `bin/search`. It will be called with the command line arguments

 ```text
 bin/search [target string] [directory]
```

 The second command line argument is optional but the first is required.

Break up your task into two components:

- Producer Thread
- Thread Pool

The **Producer Thread** should search for all files worth opening. There is no point in trying to open and read a file that will not contain the value. Your search will only look in files that contain the following extensions: cc, c, cpp, h, hpp, pl, sh, py, txt. Ignore any files with other extension you find in the directory. The **Producer Thread**'s job is to find all files with these extensions and pass them to the thread pool.

The **Thread Pool** will open up the files it is provided and search them for the target text. These threads should all be looking at files at the same time.

The program ensures the following:

- Files are search concurrently
- All files containing the results are found
- All threads exit gracefully once they run out of files to search.

## Output Style

This section describes the basic output style. An example execution is provided at the end of the instructions.

Start out my printing the target, directory, and number of threads being used.

```text
!---- Search Started ----!
Target Folder: /gcc
Target Text: <thread>
Using a Pool of 4 threads to search.
```

When a result is found, print out the thread that found it, file name, and line containing the target text.

```
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/gcc/testsuite/g++.dg/modules/xtreme-header-2.h"
Line 12: #include <thread>
----------
```

Print a completion message at the end. Remember you may find 0 returns, so the user still has to know their search ran.

```text
!---- Search Complete ----!
```

## Makefile
1) `make`  - Builds the Program
2) `make clean` - Remove compiled code