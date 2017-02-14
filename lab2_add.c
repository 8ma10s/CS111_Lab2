#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

///STRUCT DEFINITIONS///

typedef struct pthArg{
  long long *pointer;
  int nIter;
}pthArg;

///GLOBAL VARIABLES
int opt_yield = 0;
int doSync = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int spinLock = 0;
///FUNCTIONS///

void* doNone(void *args);
void add(long long *pointer, long long value);
int setSyncType(char *opt);
void setName(char **name);
void syncLock();
void syncUnlock();
///MAIN///
int main(int argc, char *argv[]){

  //STRUCTS AND VARIABLES

  //getopt related
  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", no_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
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
  while((opt = getopt_long(argc,argv, "t:i:ys:", longopts, &longindex)) != -1){
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
      setName(&name);
      break;

      //SYNC
    case 's':
      if(setSyncType(optarg) != 0){
	fprintf(stderr, "Not a valid sync argument. Skipping this option.\n");
      }
      setName(&name);
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

    //do actual operation (add 1 , subt 1)
    add(pointer, 1);
    add(pointer, -1);

  }
}

void add(long long *pointer, long long value) {

  while(1){
    syncLock();
    long long sum = *pointer + value;
    if(opt_yield){
      sched_yield();
    }
    if(doSync == 3){
      if (__sync_val_compare_and_swap(pointer, sum - value, sum) != (sum - value)){
	continue;
      }
      else{
	break;
      }
    }

    *pointer = sum;
    syncUnlock();
    break;
  }
}

int setSyncType(char *opt){

  if(strcmp(opt, "m") == 0){
    doSync = 1;
  }
  else if(strcmp(opt, "s") == 0){
    doSync = 2;
  }
  else if(strcmp(opt, "c") == 0){
    doSync = 3;
  }
  else{
    return 1;
  }
  return 0;

}

void setName(char **name){
  if(opt_yield == 1){
    if(doSync == 0){
      *name = "add-yield-none";
    }
    else if(doSync == 1){
      *name = "add-yield-m";
    }
    else if(doSync == 2){
      *name = "add-yield-s";
    }
    else if(doSync == 3){
      *name = "add-yield-c";
    }
  }

  else{
    if(doSync == 0){
      *name = "add-none";
    }
    else if(doSync == 1){
      *name = "add-m";
    }
    else if(doSync == 2){
      *name = "add-s";
    }
    else if(doSync == 3){
      *name = "add-c";
    }
  }
}

void syncLock(){

  //if mutex option selected, attempt to lock
  if(doSync == 1){
    if(pthread_mutex_lock(&m)){
      fprintf(stderr, "Error locking the critical section.\n");
      exit(1);
    }
  }
  
  else if(doSync == 2){
    while(__sync_lock_test_and_set(&spinLock, 1)){
      continue;
    }
  }
}

void syncUnlock(){
  //if mutex selected, attempt to unlock
  if(doSync == 1){
    if(pthread_mutex_unlock(&m)){
      fprintf(stderr, "Error unlocking the critical section.\n");
      exit(1);
    }
  }
  
  else if(doSync == 2){
    __sync_lock_release(&spinLock);
  }
  
}
