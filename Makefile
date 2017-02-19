tarfile = SortedList.h SortedList.c lab2b_list.c Makefile lab2_add.csv lab2_list.csv lab2b*.png README.txt

rmfile = lab2a-804608241.tar.gz lab2b*.png SortedList.o lab2_list.o lab2_list lab_2b_list.csv

build: lab2_list.o SortedList.o lab2_add.c
	gcc -g -o lab2_list lab2_list.o SortedList.o -pthread
	gcc -g -o lab2_add lab2_add.c -pthread

SortedList.o: SortedList.h SortedList.c
	gcc -g -o SortedList.o -c SortedList.c

lab2_list.o: lab2_list.c
	gcc -g -o lab2_list.o -c lab2_list.c

profile: lab2_list
	LD_PRELOAD=libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.gperf
	pprof --list=listOps ./lab2_list ./raw.gperf >> prfile.gperf

tests:
	rm -f lab_2b_list.csv
	# range of threads and iterations to see what it takes to cause a failure
	cat lab2_add.csv >> lab_2b_list.csv
	cat lab2_list.csv >> lab_2b_list.csv

graphs: lab2.gp
	/usr/local/cs/bin/gnuplot lab2.gp

tarball: $(tarfile)
	tar -czvf lab2a-804608241.tar.gz $(tarfile)
clean:
	rm $(rmfile)
