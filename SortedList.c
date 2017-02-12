#include "SortedList.h"
#include <string.h>
#include <stdio.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){

  //if head
  if(list->key == NULL){
    list->next == element;
    element->prev = list;
    element->next = NULL;
  }

  SortedListElement_t *current = list;
  const char *key = element->key;

  while(current->next != NULL && (strcmp(current->next->key, key) < 0)){
    current = current->next;
  }

  element->next = current->next;
  element->prev = current;

  if(current->next != NULL){
    current->next->prev = element;
  }
  current->next = element;
}

int SortedList_delete(SortedListElement_t *element){
  if(element->prev != NULL){
    if(element->prev->next != element){
      fprintf(stderr, "Corrupted pointers.\n");
      return 1;
    }
  }
  if(element->next != NULL){
    if(element->next->prev != element){
      fprintf(stderr, "Corrupted pointers.\n");
      return 1;
    }
  }
  
  element->prev->next = element->next;
  element->next->prev = element->prev;
  
  return 0;
}
