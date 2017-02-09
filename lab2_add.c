#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]){

  //getopt related
  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {0,0,0,0},
  };

  //number variables
  int nThreads = 1;
  int nIter = 1;
  long long count = 0;

  //time holding structs
  struct timespec start;
  struct timespec end;

  //getopts temporary variables
  int temp;

  //set the values for these variables using getopt_long
  int opt;
  int longindex;
  while((opt = getopt_long(argc,argv, "t:i:", longopts, &longindex)) != -1){
    switch(opt){
      //THREAD
    case 't':
      temp = atoi(optarg);
      if(temp < 1){
	fprintf(stderr, "Number of threads must be at least 1. Skipping this option.\n");
	continue;
      }
      nThreads = temp;
      break;
      //ITERATIONS
    case 'i':
      temp = atoi(optarg);
      if(temp < 1){
	fprintf(stderr, "Number of iterations must be at least 1. Skipping this option.\n");
	continue;
      }
      nIter = temp;
      break;
      //UNRECOGNIZED OPTION (DEFAULT)
    default:
      fprintf(stderr, "%s is not an option. Skipping this option.\n", argv[optind - 1]);
      continue;
    }

  } 

  //get the initial time, check for error
  if(clock_gettime(CLOCK_REALTIME, &start)){
    fprintf(stderr, "Failed to obtain starting time.\n");

  }



}
