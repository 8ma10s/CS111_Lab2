#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

///STRUCT DEFINITIONS///

typedef struct pthArg{
  long long *pointer;
  int nIter;
}pthArg;

///GLOBAL VARIABLES
int opt_yield = 0;

///FUNCTIONS///

void* doNone(void *args);
void add(long long *pointer, long long value);


///MAIN///
int main(int argc, char *argv[]){

  //STRUCTS AND VARIABLES

  //getopt related
  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", no_argument, NULL, 'y'},
    {0,0,0,0},
  };

  //number variables
  int nThreads = 1;
  int nIter = 1;
  long long count = 0;
  char *name = "add-none";
  //time holding structs
  struct timespec start;
  struct timespec end;

  //getopts temporary variables
  int opt;
  int longindex;
  int temp;

  //pthread related variables
  pthread_t *thArr;
  pthArg args;

  //ACTUAL CODE

  //set the values for these variables using getopt_long
  while((opt = getopt_long(argc,argv, "t:i:y", longopts, &longindex)) != -1){
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

      //YIELD
    case 'y':
      opt_yield = 1;
      name = "add-yield-none";
      break;

      //UNRECOGNIZED OPTION (DEFAULT)
    default:
      fprintf(stderr, "%s is not an option. Skipping this option.\n", argv[optind - 1]);
      continue;
    }
  } 

  //Allocate appropriate amount of memory for the thread ID container
  thArr = (pthread_t*) malloc(nThreads * sizeof(pthread_t));
  if(thArr == NULL){
    fprintf(stderr,"Could not allocate memory for thread ID.\n");
    exit(1);
  }
  //set the arguments to pass for each thread
  args.pointer = &count;
  args.nIter = nIter;

  //get the initial time, check for error
  if(clock_gettime(CLOCK_REALTIME, &start)){
    fprintf(stderr, "Failed to obtain starting time.\n");
    exit(1);
  }

  //start threading
  int i;
  for(i = 0; i < nThreads; i++){
    if(pthread_create(&(thArr[i]), NULL, doNone, (void*) &args)){
      fprintf(stderr, "pthread_create() error\n");
      exit(1);
    }
  }

  //join threads
  int j;
  for(j =0; j < nThreads; j++){
    if(pthread_join(thArr[j], NULL)){
      fprintf(stderr, "pthread_join() error\n");
      exit(1);
    }
  }

  //take end time
  if(clock_gettime(CLOCK_REALTIME, &end)){
    fprintf(stderr, "Failed to obtain ending time.\n");
    exit(1);
  }

  //calculate the numbers to output
  int ops = nThreads * nIter * 2;
  long long sec = (long long) (end.tv_sec - start.tv_sec);
  long long nsec = (long long) (end.tv_nsec - start.tv_nsec);
  long long result = sec * 1000000000 + nsec;
  double tpo = (double) (result / ops) + ((result % ops) / (double) ops);

  //output as csv
  printf("%s,%d,%d,%d,%ld,%f,%ld\n", name, nThreads, nIter, ops, result, tpo, count);

  //free memory
  free(thArr);
}

void *doNone(void* args){
  int nIter = ((pthArg*)args)->nIter;
  long long *pointer = ((pthArg*)args)->pointer;

  int i;
  for(i = 0; i < nIter; i++){
    add(pointer, 1);
    add(pointer, -1);
  }

}

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if(opt_yield){
    sched_yield();
  }
  *pointer = sum;
}
