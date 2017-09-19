James Ward
Assignment 5, Semaphores
Operating Systems Spring 2016

The objective of this assignment is to understand how semaphores are used to organize the processes of processses that use the same resoureces (ie. printing to the screen, reading from the keyboard). 

This program creates two named semaphores --/screen and /keybrd-- then forks 4 processes that all call runProcess, and finally waits for the children processes to die before unlinking the semaphores and exiting. The runProcess() function runs uses getSems() to lock the required semaphores, and then echos input until a 'q' is read. The function exits at the end (kills child once it has echoed enough times rather than returning to the fork loop). Before it exits, it posts and closes used semaphores. The getSems() funciton locks the two semaphores (screen and keybrd) with deadlock checking and resolution, and returns the number of deadlock incountered.

the program can be automatically created with the 'make' command. This will create the 'prog' executable which can be ran with ./prog

**IMPORTANT** there is sometimes a long wait time for the processes to resolve the deadlock, it's not
frozen.

Below is a sample run

james@SgtSarcasm:~/Documents/OS/JWARD_A5$ make clean
rm *.o prog
james@SgtSarcasm:~/Documents/OS/JWARD_A5$ make
gcc -Wall   -c -o main.o main.c
gcc -Wall -o prog main.o -lpthread
james@SgtSarcasm:~/Documents/OS/JWARD_A5$ ./prog
>> This is process 3352: Had 0 deadlocks
>> Enter a string or q to quit
>> q
>> Echo: q
>> This is process 3353: Had 1 deadlocks
>> Enter a string or q to quit
>> q
>> Echo: q
>> This is process 3352: Had 3 deadlocks
>> Enter a string or q to quit
>> dfasf
>> Echo: dfasf
>> q
>> Echo: q
>> This is process 3353: Had 6 deadlocks
>> Enter a string or q to quit
>> fsadc
>> Echo: fsadc
>> acds
>> Echo: acds
>> as
>> Echo: as
>> q
>> Echo: q
>> This is process 3354: Had 11 deadlocks
>> Enter a string or q to quit
>> q
>> Echo: q
>> This is process 3351: Had 9 deadlocks
>> Enter a string or q to quit
>> q
>> Echo: q
>> This is process 3354: Had 2 deadlocks
>> Enter a string or q to quit
>> q
>> Echo: q
>> This is process 3351: Had 2 deadlocks
>> Enter a string or q to quit
>> q
>> Echo: q
