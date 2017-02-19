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
     title 'mutex' with linespoints lc rgb 'blue', \
     "< grep add-s lab2_add.csv" using ($2):(1000000000/($6)) \
     title 'spin' with linespoints lc rgb 'orange', \
     "< grep list-none-m lab2_list.csv" using ($2):(1000000000/($7)) \
     title '(raw) mutex' with linespoints lc rgb 'grey', \
     "< grep list-none-s lab2_list.csv" using ($2):(1000000000/($7)) \
     title '(raw) spin-lock' with linespoints lc rgb 'violet'
