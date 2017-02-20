#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "SortedList.h"

///STRUCT DEFINITIONS///

typedef struct pthArg{
  SortedList_t *list;
  SortedListElement_t *element;
  int nIter;
  int nThreads;
  int nLists;
  long long *muTime;
}pthArg;

///GLOBAL VARIABLES
int doSync = 0;
pthread_mutex_t *m;
int *spinLock;
int opt_yield = 0;

//FUNCTIONS
int setYieldType(char *opt);
void setName(char **name);
int setSyncType(char *opt);
void syncLock(int num);
void syncUnlock(int num);
void setChar(SortedListElement_t slArr[], char chArr[][5], int arrSize);
void *listOps(void* args);
void getTime(struct timespec *ts);
long long getTimeDiff(struct timespec *start, struct timespec *end);
unsigned long hash(const unsigned char *str);

///MAIN///
int main(int argc, char*argv[]){

  //getopt related
  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", required_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {"lists", required_argument, NULL, 'l'},
    {0,0,0,0},
  };

  //number variables
  int nThreads = 1;
  int nIter = 1;
  int nLists = 1;
  char *name = "list-none-none";
  //time holding structs
  struct timespec start;
  struct timespec end;

  //getopts temporary variables
  int opt;
  int longindex;
  int temp;

  //ACTUAL CODE

  //set the values for these variables using getopt_long
  while((opt = getopt_long(argc,argv, "t:i:y:s:l:", longopts, &longindex)) != -1){
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
      if(setYieldType(optarg) != 0){
	continue;
      }
      setName(&name);
      break;
      //SYNC
    case 's':
      if(setSyncType(optarg) != 0){
	fprintf(stderr, "Not a valid sync argument. Skipping this option.\n");
      }
      setName(&name);
      break;
      //LISTS
    case 'l':
      temp = atoi(optarg);
      if(temp < 1){
	fprintf(stderr, "Number of lists must be at least 1. Skipping this option.\n");
	continue;
      }
      nLists = temp;
      break;
      //UNRECOGNIZED OPTION (DEFAULT)
    default:
      fprintf(stderr, "%s is not an option. Skipping this option.\n", argv[optind - 1]);
      continue;
    }
  }

  //allocate memory for the mutex;
  m = (pthread_mutex_t*) malloc(nLists * sizeof(pthread_mutex_t));
  if(m == NULL){
    fprintf(stderr, "Failed to allocate memory for the mutex lock\n");
    exit(1);
  }

  //allocate memory for the spin lock
  spinLock = (int*) malloc(nLists * sizeof(int));
  if(spinLock == NULL){
    fprintf(stderr, "Failed to allocate memory for the spin lock\n");
    exit(1);
  }

  //initialize empty list
  SortedList_t list[nLists];

  //set initial value for mutex & spin lock
  int l;
  for (l = 0; l < nLists; l++){
    pthread_mutex_init(&(m[l]), NULL);
    spinLock[l] = 0;
    list[l].key = NULL;
    list[l].next = NULL;
    list[l].prev = NULL;
  }

  //Create element container
  SortedListElement_t slArr[nThreads * nIter];

  //Create key container and insert
  char chArr[nThreads * nIter][5];
  setChar(slArr, chArr, nThreads * nIter);

  //Create thread ID container
  pthread_t thArr[nThreads];

  //Create thread time container
  long long muTimeArr[nThreads];

  //Create thread argument container
  pthArg args[nThreads];
  int t;
  for (t = 0; t < nThreads; t++){
    args[t].list = list;
    args[t].nIter = nIter;
    args[t].nThreads = nThreads;
    args[t].nLists = nLists;
    args[t].element = &(slArr[t * nIter]);
    muTimeArr[t] = 0;
    args[t].muTime = &(muTimeArr[t]);
  }

  //get the initial time, check for error
  if(clock_gettime(CLOCK_REALTIME, &start)){
    fprintf(stderr, "Failed to obtain starting time.\n");
    exit(1);
  }

  //start threading
  int i;
  for(i = 0; i < nThreads; i++){
    if(pthread_create(&(thArr[i]), NULL, listOps, (void*) &(args[i]))){
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

  // check that list is zero
  for(t = 0; t < nLists; t++){
    if(SortedList_length(&(list[t])) != 0){
      fprintf(stderr, "list length is not 0 at the end\n");
      exit(1);
    }
  }
  //calculate the numbers to output
  int ops = nThreads * nIter * 3;
  long long sec = (long long) (end.tv_sec - start.tv_sec);
  long long nsec = (long long) (end.tv_nsec - start.tv_nsec);
  long long result = sec * 1000000000 + nsec;
  int tpo = result / ops;

  //calculate mutex time
  long long muTime = 0;
  for(t=0; t < nThreads; t++){
    muTime += muTimeArr[t];
  }
  muTime /= nThreads * (2 * nIter + nLists);

  //output as csv
  printf("%s,%d,%d,%d,%d,%ld,%d", name, nThreads, nIter, nLists, ops, result, tpo);

  if(doSync == 1){
    printf(",%ld\n", muTime);
  }
  else{
    printf("\n");
  }

  free(m);
  free(spinLock);
  return 0;

}

int setYieldType(char *opt){
  if (opt == NULL){
    fprintf(stderr, "Argument is null.\n");
    return 1;
  }

  //iterate each character and add that option to opt_yield
  int isValid = 0;
  int i;
  for (i = 0; opt[i] != '\0'; i++){
    if(opt[i] == 'i'){
      opt_yield |= INSERT_YIELD;
      isValid = 1;
    }
    else if(opt[i] == 'd'){
      opt_yield |= DELETE_YIELD;
      isValid = 1;
    }
    else if(opt[i] == 'l'){
      opt_yield |= LOOKUP_YIELD;
      isValid = 1;
    }
  }

  //if at least one of the options are valid, return 0. Else, return 1 (none is valid)
  if(isValid == 1){
  return 0;
  }
  else{
    return 1;
  }
}

void setName (char **name){
  if(opt_yield == INSERT_YIELD){
    if(doSync == 0){
      *name = "list-i-none";
    }
    else if(doSync == 1){
      *name = "list-i-m";
    }
    else if(doSync == 2){
      *name = "list-i-s";
    }
  }

  else if(opt_yield == DELETE_YIELD){
    if(doSync == 0){
      *name = "list-d-none";
    }
    else if(doSync == 1){
      *name = "list-d-m";
    }
    else if(doSync == 2){
      *name = "list-d-s";
    }
  }

  else if(opt_yield == (INSERT_YIELD | DELETE_YIELD)){
    if(doSync == 0){
      *name = "list-id-none";
    }
    else if(doSync == 1){
      *name = "list-id-m";
    }
    else if(doSync == 2){
      *name = "list-id-s";
    }
  }

  else if(opt_yield == LOOKUP_YIELD){
    if(doSync == 0){
      *name = "list-l-none";
    }
    else if(doSync == 1){
      *name = "list-l-m";
    }
    else if(doSync == 2){
      *name = "list-l-s";
    }
  }

  else if(opt_yield == (INSERT_YIELD | LOOKUP_YIELD)){
    if(doSync == 0){
      *name = "list-il-none";
    }
    else if(doSync == 1){
      *name = "list-il-m";
    }
    else if(doSync == 2){
      *name = "list-il-s";
    }
  }

  else if(opt_yield == (DELETE_YIELD | LOOKUP_YIELD)){
    if(doSync == 0){
      *name = "list-dl-none";
    }
    else if(doSync == 1){
      *name = "list-dl-m";
    }
    else if(doSync == 2){
      *name = "list-dl-s";
    }
  }

  else if(opt_yield == (INSERT_YIELD | DELETE_YIELD | LOOKUP_YIELD)){
    if(doSync == 0){
      *name = "list-idl-none";
    }
    else if(doSync == 1){
      *name = "list-idl-m";
    }
    else if(doSync == 2){
      *name = "list-idl-s";
    }
  }

  else if(opt_yield == 0){
    if(doSync == 0){
      *name = "list-none-none";
    }
    else if(doSync == 1){
      *name = "list-none-m";
    }
    else if(doSync == 2){
      *name = "list-none-s";
    }
  }

}

int setSyncType(char *opt){

  if(strcmp(opt, "m") == 0){
    doSync = 1;
  }
  else if(strcmp(opt, "s") == 0){
    doSync = 2;
  }
  else{
    return 1;
  }
  return 0;

}

void syncLock(int num){

  //if mutex option selected, attempt to lock
  if(doSync == 1){
    if(pthread_mutex_lock(&(m[num]))){
      fprintf(stderr, "Error locking the critical section.\n");
      exit(1);
    }
  }
  
  else if(doSync == 2){
    while(__sync_lock_test_and_set(&(spinLock[num]), 1)){
      continue;
    }
  }
}

void syncUnlock(int num){
  //if mutex selected, attempt to unlock
  if(doSync == 1){
    if(pthread_mutex_unlock(&(m[num]))){
      fprintf(stderr, "Error unlocking the critical section.\n");
      exit(1);
    }
  }
  
  else if(doSync == 2){
    __sync_lock_release(&(spinLock[num]));
  }
  
}

void setChar(SortedListElement_t slArr[], char chArr[][5], int arrSize){
  char chList[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  int i,j;
  for (i = 0; i < arrSize; i++){
    for(j = 0; j < 4; j++){
      chArr[i][j] = chList[rand() % 62];
    }
    chArr[i][4] = '\0';
    slArr[i].key = (chArr[i]);
  }

}

void *listOps(void* args){

  struct timespec start,end;
  long long total = 0;
  int hashNum, listSize;

  pthArg * pArgs = (pthArg*) args;
  
  int i;
  for(i = 0; i < pArgs->nIter; i++){
    hashNum = (int)(hash(pArgs->element[i].key) % pArgs->nLists);
    getTime(&start);
    syncLock(hashNum);
    getTime(&end);
    total += getTimeDiff(&start,&end);

    SortedList_insert(&(pArgs->list[hashNum]), &((pArgs->element)[i]));
    syncUnlock(hashNum);
  }

  listSize = 0; 
  for(i=0; i < pArgs->nLists; i++){
    getTime(&start);
    syncLock(i);
    getTime(&end);
    total += getTimeDiff(&start,&end);
    listSize +=  SortedList_length(&(pArgs->list[i]));
    syncUnlock(i);
  }
  if (listSize < (pArgs->nIter) || listSize > (pArgs->nIter) * (pArgs->nThreads)){
    fprintf(stderr, "Invalid number of elements.\n");
    exit(1);
  }
  
  SortedListElement_t *toDelete;
  for(i = 0; i < pArgs->nIter; i++){
    hashNum = (int)(hash(pArgs->element[i].key) % pArgs->nLists);
    getTime(&start);
    syncLock(hashNum);
    getTime(&end);
    total += getTimeDiff(&start, &end);

    if((toDelete = SortedList_lookup(&(pArgs->list[hashNum]), (pArgs->element)[i].key)) == NULL){
      fprintf(stderr, "Failed to locate the key inserted.\n");
      syncUnlock(hashNum);
      exit(1);
    }   
    if(SortedList_delete(toDelete)){
      fprintf(stderr, "Detected corruption upon deletion.\n");
      syncUnlock(hashNum);
      exit(1);
    }
    syncUnlock(hashNum);
  }

  *(pArgs->muTime) = total;

}

void getTime(struct timespec *ts){
  if(doSync == 1){
    if(clock_gettime(CLOCK_REALTIME, ts)){
      fprintf(stderr, "Failed to obtain thread timecount.\n");
      exit(1);
    }  
  }
}

long long getTimeDiff(struct timespec *start, struct timespec *end){
  long long result;

  result = (long long)(end->tv_sec - start->tv_sec) * 1000000000 + (long long)(end->tv_nsec - start->tv_nsec);

  return result;
}

unsigned long hash(const unsigned char *str){
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;

}
