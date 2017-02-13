#include "SortedList.h"
#include <string.h>
#include <stdio.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
  
if(list == NULL || element == NULL){
    fprintf(stderr, "One of the arguments is null.\n");
    return;
  }

  //if head only
  if(list->key == NULL){
    list->next == element;
    element->prev = list;
    element->next = NULL;
  }
  //if not head only
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
  if(element == NULL){
    fprintf(stderr, "Argument is null.\n");
    return 1;
  }
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

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key){

  //null argument check
  if(list == NULL || key == NULL){
    fprintf(stderr, "One of the arguments is null.\n");
    return NULL;
  }

  //if empty, not found
  if(list->key == NULL){
    return NULL;
  }

  //iterate and attempt to find
  SortedListElement_t *current = list->next;
  while(current != NULL){
    if(strcmp(current->key,key) == 0){
      return current;
    }
    else{
      current=current->next;
    }
  }

  return NULL;

}

int SortedList_length(SortedList_t *list){
  if(list == NULL){
    fprintf(stderr, "Argument is null.\n");
    return -1;
  }

  //if empty
  if(list->key == NULL){
    return 0;
  }

  SortedListElement_t *current = list->next;
  int count = 0;
  while(current != NULL){
    //check for corruption
    if(current->prev->next != current){
      return -1;
    }
    else if(current->next != NULL && current->next->prev != current){
      return -1;
    }
    else{
      count++;
      current=current->next;
    }
  }
  return count;

}
