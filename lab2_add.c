#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//STRUCT DEFINITIONS

typedef struct pthArg{
  long long *pointer;
  int nIter;
}pthArg;

//FUNCTIONS

void* doNone(void *args);
void add(long long *pointer, long long value);

int main(int argc, char *argv[]){

  //STRUCTS AND VARIABLES

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
  int opt;
  int longindex;
  int temp;

  //pthread related variables
  pthread_t *thArr;
  pthArg args;

  //ACTUAL CODE

  //set the values for these variables using getopt_long
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

  //Allocate appropriate amount of memory for the thread ID container
  thArr = (pthread_t*) malloc(nThreads * sizeof(pthread_t));
  if(thArr == NULL){
    fprintf(stderr,"Could not allocate memory for thread ID.\n");
    return 1;
  }
  //set the arguments to pass for each thread
  args.pointer = &count;
  args.nIter = nIter;

  //get the initial time, check for error
  if(clock_gettime(CLOCK_REALTIME, &start)){
    fprintf(stderr, "Failed to obtain starting time.\n");
    return 1;
  }

  //start threading
  int i;
  for(i = 0; i < nThreads; i++){
    if(pthread_create(&(thArr[i]), NULL, doNone, (void*) &args)){
      fprintf(stderr, "pthread_create() error\n");
      return 1;
    }
  }

  //join threads
  int j;
  for(j =0; j < nThreads; j++){
    if(pthread_join(thArr[j], NULL)){
      fprintf(stderr, "pthread_join() error\n");
      return 1;
    }
  }

  printf("Count is: %d\n", count);

  free(thArr);
}

void *doNone(void* args){
  pthArg *pArgs = (pthArg*) args;
  int nIter = pArgs->nIter;
  long long *pointer = pArgs->pointer;

  int i;
  for(i = 0; i < nIter; i++){
    add(pointer, 1);
    add(pointer, -1);
  }

}

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  *pointer = sum;
}
