
Questions

2.3.1
Most of the cycles are spent in the creation and the joining of the threads. Although threads are not as expensive as processes, the system still needs to create stack pointers, allocate separate memory for the own stack memory, etc. Those operations definitely require many system calls, and since system calls are generally slower, the overhead of threading is significant than the actual operation after threading (at low number of threads).

Most of the cycles are being spent in the polling process in the spin-lock tests. This is obvious from the fact that the overhead keeps increasing as number of thread increases (more than that of mutex). The only action that is unique to spin lock (mutex doesn't have) is the polling, which constantly eats up CPU time.

Most of the cycles are being spent on the time each thread sleeps due to actual operation for mutex locks. Looking at the add and list operations, the throughput of list operation decrease constantly as number of threads increases, but not so much for add operations. That indicates that the large amount of time is used for insertion,deletion of list: as threads increases, so does the number of elements that needs to be inserted increases, and each insertion/deletion locks the entire linked list, causing other threads to sleep until it is done.

