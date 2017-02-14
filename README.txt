Included Files

lab2_add.c: adds and subtracts 1, with yield and sync options
SortedList.h: Interface for SortedList
SortedList.c: Implementation of the SortedList
lab2_list.c: driver program that operates on SortedList, with yield and sync options
Makefile: contains all information needed for compilation and execution
lab2_add.csv: data points used for analyzing lab2_add
lab2_lis.csv: data points used for analyzing lab2_list

lab2_add-1.png -- lab2_add-5.png: statistics for lab2_add
lab2_list-1.png -- lab2_list-4.png: statistics for lab2_list
README.txt: this file. contains information about all files

Limitations

For lab2_add, sync options are made to be ignored if anything other than m,s,c are specified. Iterations and thread numbers are ignored if they are less than 1.

For lab2_list, sync options are ignored if none of the letters are i,d,l. If one of them is i,d,l, then that option is set.
This program cannot sort correctly if multi-threaded, unless you enable sync options.

Test Methodology
Tests are done solely using the test script given.

Questions (answers)

2.1.1
It takes many iterations before errors are seen because then the total time to complete all iterations will be longer, increasing the chance of two iterations from the different thread occurring at the same time. 

Smaller number or iterations seldom fail because of the same reason, and also the fact that starting a thread takes some amount of time, so smaller number of iterations are likely to end by the time the next thread is created (so the time to complete iterations for different threads are less likely to overlap).

2.1.2
--yield runs are so much slower because of the context switching time.
It takes some time for a scheduler to switch between threads. Because sched_yield causes more frequent switching between threads, it leads to more time for the entire program to run.

We cannot get the valid per-operation timings because context switching time is too significant to be ignored when simply dividing the total run time by number of operations. That number is just the average of the "sum of one operation and one context switching", but we cannot assume which of the two took what portion of the resulting time.

2.1.3
Decreasing average cost indicates the existence of high fixed cost. That is, the operations that is done only once per execution, such as thread creation, joining, etc. are taking significant portion of the total run time. (Those fixed costs are divided by the total number of operation, so higher the iteratin, lower these costs will show up as per operation).

From the logic above, we can make number of iterations really large, so that the portion of the "fixed cost" added to time per operation is insignificant. That means, the higher the iteration, the closer its value for "time per operation" will be to the true value.

2.1.4
For small number of threads, the chances of two (or more) threads reading/writing at the same time is very low, and thus there are only few occurrences of a thread having to wait for the locks to be unlocked (waiting for it to be unlocked takes time).

All operations slow down as number of threads rises because of the increasing probability of 2 threads trying to operate at the same time. "Any of the combinations of 2 threads out of 12 threads operating at the same time"  has higher probability than "2 out of 2 threads operating at the same time".  Since only 1 thread can operate on the critical section at a time, the other thread trying to operate on the same variable must stop at that location of the code and wait for the critical section to be open. 

Spin locks are so expensive for large numbers of threads because the threads don't sleep, but instead keep looping ("busywait") until the locks are unlocked. So essentially, all threads are constantly working doing its full work, either operating or checking for locks to be open.

2.2.1
Both mutexes at part 1 and 2 are similar in a way that it goes up with number of threads. That is because the more threads they have, the more chance each thread (and thus, each operation) has to wait for the locks to be unlocked (if 12 threads, all 12 are competing vs. only 2 for 2 threads).

They are different in a way that the cost per operation goes up at the lower rate for part1 mutex, whereas the rate of change of cost per operation is almost contant for part 2. That is most likely due to the fact that for linked lists, the actual time per operation is so significant compared to simple addition. For adding program, one operation takes only the small amount of time (thus causing less wait time), whereas the longer operations in the linked lists cause more threads to wait for that mutex to be unlocked (for each thread that is waiting, the cost per operation goes up).

2.2.2
Both are similar in a way that the cost goes up as number of threads increases. That is, for both methods, the cost per operation increases as more threads compete in order to complete their own operations.

The difference is that spin locks tend to have increasing cost compared to mutex (the gap between mutex and spin lock increases as number of threads increase). That most likely happens becaue spin lock does not put the thread to sleep, so some of the CPU time is used for polling (if the lock is unlocked). While polling, that core of the CPU cannot do its other share of work, thus slowing down the entire system to some extent. This affects the execution time of the program, causing spin locks to be slower. It is understandable that the gap between spin and mutex increases because more polling happens as number of threads increase, eating more CPU time.
