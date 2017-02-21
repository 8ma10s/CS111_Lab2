#! /usr/bin/gnuplot
#
# purpose:
#  generate data reduction graphs for the multi-threaded add project
#
#input: lab2_add.csv, lab2_list.csv
#output:
#lab2b_1.png ... throughput (operations per second)

#general plot parameters
set terminal png
set datafile separator ","

set title "lab2b_1: Throughput(operations per second)"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_1.png'
set key left top
plot \
     "< grep add-m lab2_add.csv" using ($2):(1000000000/($6)) \
     title 'mutex add' with linespoints lc rgb 'blue', \
     "< grep add-s lab2_add.csv" using ($2):(1000000000/($6)) \
     title 'spin-lock add' with linespoints lc rgb 'orange', \
     "< grep -E 'list-none-m,[0-9]+,1000,1,' lab_2b_list.csv" using ($2):(1000000000/($7)) \
     title 'mutex list' with linespoints lc rgb 'grey', \
     "< grep -E 'list-none-s,[0-9]+,1000,1,' lab_2b_list.csv" using ($2):(1000000000/($7)) \
     title 'spin-lock list' with linespoints lc rgb 'violet'

set title "lab2b_2: Wait-for-lock time for mutex"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Wait Time (ns)"
set logscale y 10
set output 'lab2b_2.png'
set key left top
plot \
     "< grep -E 'list-none-m,[0-9]+,1000,1,' lab_2b_list.csv" using ($2):($7) \
     title 'Average Time Per Operation' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-m,[0-9]+,1000,1,' lab_2b_list.csv" using ($2):($8) \
     title 'Average Wait Time Per Lock' with linespoints lc rgb 'green'

set title "lab2b_3: Unprotected and Protected threads and iterations that run without failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Successful Iterations"
set logscale y 10
set output 'lab2b_3.png'
# note that unsuccessful runs should have produced no output
plot \
     "< grep list-id-none lab_2b_list.csv" using ($2):($3) \
     title 'yield=id, unprotected' with points lc rgb 'green', \
     "< grep list-id-m lab_2b_list.csv" using ($2):($3) \
     title 'yield=id, mutex' with points lc rgb 'red', \
     "< grep list-id-s lab_2b_list.csv" using ($2):($3) \
     title 'yield=id, sync' with points lc rgb 'violet'

set title "lab2b_4: Throughput(operations per second) for spin-lock"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_4.png'
set key left top
plot \
     "< grep -E 'list-none-s,1?[0-9],1000,1,' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=1' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-s,[0-9]+,1000,4' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -E 'list-none-s,[0-9]+,1000,8' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=8' with linespoints lc rgb 'blue', \
     "< grep -E 'list-none-s,[0-9]+,1000,16' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=16' with linespoints lc rgb 'violet'

set title "lab2b_5: Throughput(operations per second) for mutex"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Operations per second"
set logscale y 10
set output 'lab2b_5.png'
set key left top
plot \
     "< grep -E 'list-none-m,1?[0-9],1000,1,' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=1' with linespoints lc rgb 'red', \
     "< grep -E 'list-none-m,[0-9]+,1000,4' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -E 'list-none-m,[0-9]+,1000,8' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=8' with linespoints lc rgb 'blue', \
     "< grep -E 'list-none-m,[0-9]+,1000,16' lab_2b_list.csv" using ($2):(1000000000/($6)) \
     title 'lists=16' with linespoints lc rgb 'violet'