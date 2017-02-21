Included Files
lab2_list.c: driver program for linked lists
SortedList.c: Implementation of sorted list
SortedList.h: Definitions of sorted list
lab2b_1.png: Throughput of protected (mutex/spinlock) operations, single list
lab2b_2.png: Wait-for-lock time/average operation time comparison for mutex
lab2b_3.png: Threads/Iterations that run without failure (Protected/Unprotected)
lab2b_4.png: spin-lock throughput for various threads, iterations, and list numbers
lab2b_5.png: mutex throughput for various threads, iterations, and list numbers
Makefile: Information to be used to execute various make commands
lab_2b_list.csv: Data points to be used to generate graphs
lab2_add.csv: Data points to be used to generate graphs
profile.gperf: Profile information containing where cycles are used for spin-lock
README.txt: Information on included files and answers to questions
lab2b.gp: gnuplot file to generate graphs
libprofiler.so: libprofiler for gprof
libprofiler.so.0.4.8: libprofiler executable
pprof: software to create profile

Questions

2.3.1
Most of the cycles are spent in the creation and the joining of the threads. Although threads are not as expensive as processes, the system still needs to create stack pointers, allocate separate memory for the own stack memory, etc. Those operations definitely require many system calls, and since system calls are generally slower, the overhead of threading is significant than the actual operation after threading (at low number of threads).

Most of the cycles are being spent in the polling process in the spin-lock tests. This is obvious from the fact that the overhead keeps increasing as number of thread increases (more than that of mutex). The only action that is unique to spin lock (mutex doesn't have) is the polling, which constantly eats up CPU time.

Most of the cycles are being spent on the time each thread sleeps due to actual operation for mutex locks. Looking at the add and list operations, the throughput of list operation decrease constantly as number of threads increases, but not so much for add operations. That indicates that the large amount of time is used for insertion,deletion of list: as threads increases, so does the number of elements that needs to be inserted increases, and each insertion/deletion locks the entire linked list, causing other threads to sleep until it is done.

2.3.2
According to the performance test, the function syncLock(), which contains all of the codes that does the spinlock implementation (including polling) (about twice the cycle of insertion).

This operations become so expensive because more threads compete against each other to get the lock for every insertion/lookup/delete etc. If one thread is doing any of the actions, the other 11 threads will eat up CPU time just to poll to see if the other threads have given up the lock. The more threads in total, more threads will do the "eat-up CPU" action to prevent the other threads from getting their share processed.

2.3.3
Because average wait time counts the time spent to acquire the lock for EACH thread, and sums that up. For example, if all 12 threads happen to want to do insertion at the same time, while 1 is processing insertion, all 11 other threads will have to wait for acquisition, which multiplies the acquisition time of one thread by 11.

Completion time per operation does not take into account the fact that multiple threads are spending time waiting for the lock. Essentially, it doesn't care whether more than one threads are waiting: it just gets the difference between starting point and end time, and divides the difference by number of operations.

The wait time per operation went up faster as threads increased due to the fact that more and more threads will be waiting at the same time (former takes that into account, latter doesn't).

2.3.4
The throughput as a function of # of lists seems to increase dramatically from 1 list to 2 lists, but less on 4 to 8, and even less on 8 to 16. That is, the function seems to take some sort of log function that approaches a certain value as number of lists approach infinity.

The throughput would not increase at some point because when number of lists exceed total number of elements, some of the lists will have no elements, producing either the same result as having equal number of lists per element, or producing less throughput due to having to check more lists to check for the total length of the list.

Looking at the graph (especially mutex), the statement seems to be true. The results for 16 threads, 4 lists seems approximately equal to the result for 16/4=4 threads, 1 list. Similarly, result for 16 threads, 8 lists seems approximately equal t the result for 16/8=2 threads, 1 list.
