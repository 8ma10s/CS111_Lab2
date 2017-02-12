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

