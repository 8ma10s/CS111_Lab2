tarfile = SortedList.h SortedList.c lab2b_list.c Makefile lab2_add.csv lab2b*.png README.txt

rmfile = lab2a-804608241.tar.gz lab2b*.png SortedList.o lab2_list.o lab2_list lab_2b_list.csv

build: lab2_list.o SortedList.o
	gcc -g -o lab2_list lab2_list.o SortedList.o -pthread

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
	cat lab2_add.csv >> lab_2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab_2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab_2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab_2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab_2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab_2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab_2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab_2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab_2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab_2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab_2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab_2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab_2b_list.csv

graphs: lab2b.gp
	/usr/local/cs/bin/gnuplot lab2b.gp

tarball: $(tarfile)
	tar -czvf lab2a-804608241.tar.gz $(tarfile)
clean:
	rm $(rmfile)
