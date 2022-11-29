# Enhanced-Access-Control-Logging-System

For this assignment, you will develop an enhanced access control logging system using the C
programming language. The enhanced access control logging system will monitor and keep
track of every file access and modification that occurs in the system by every user. So, each
file access or file modification will generate an entry in a log file. This log file will be
inspected by a separate high privileged process. 

== Usage ==

1) make all(default)
    - This command compiles the files logger.c, acmonitor.c, test_aclog.c
2) make logger
    - This command compiles the file logger.c
3) make acmonitor
    - This command compiles the file acmonitor.c 
4) make test_aclog
    - This command compiles the file test_aclog.c 
5) make run
    - This command executes the test_aclog using the library logger.so
6) make clean
    - This command removes the files produced on runtime

-gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0-