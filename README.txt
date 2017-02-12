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

