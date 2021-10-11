/* EE205 Project2: QuickSort & ChoosePivot */
/* Written by 20200029 HyunDeok Kong */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define Max 10000000 // maximum length

// Get integer array from standard input
// gets integer type pointer
// returns integer type length of array
// length must be less than Max
int GetNum(int* a){
  // store element
  int num;
  // store length of a
  int len = 0;

  // get elements
  while(len <= Max){
    // if scanf scans EOF, break loop
    if(scanf("%d",&num) == EOF) {
      break;
    }
    // else
    len++; // increase length by one
    // store elements
    a[len-1]=num;
  }
  
  return len;
}

// Swap i & j-th element of array v
// Gets array v, and two integers i, j
void swap(int* v, int i, int j){
  // temporary memory
  int temp=v[i];
  v[i] = v[j];
  v[j] = temp;
  return;
}

// Choose pivot function
// Gets array v, length, and type number
// Type 1: First, 2: Last, 3: Random, 4: Median-of-three
// Swap pivot element and first element
void ChoosePivot(int *v, int begin, int end, int type){
  // save pivot
  int p = 0;

  // for random
  srand(time(NULL));

  // Choose pivot
  if(type == 1) { // First
    return;
  }
  else if(type == 2){ // Last
    swap(v,begin,end); // swap last and first
    return;    
  }
  else if(type == 3){ // Random
    // pick random pivot from begin to end
    p = rand() % (end-begin+1) + begin;
    swap(v,begin,p); // swap random and first
    return;
  }
  else if(type == 4){ // median of three
    if(v[begin] < v[(begin+end)/2]){
      if(v[(begin+end)/2] < v[end]){
	p = (begin+end)/2; // middle element is median
	swap(v,begin,p); // swap pivot and first
	return;
      }
      else if(v[begin] < v[end]){
	swap(v,begin,end); // swap pivot and first
	return;
      }
      else {
	return; // pivot is first
      }
    }
    else{ // v[begin] > v[middle]
      if(v[begin] < v[end]){
	return; // pivot is first
      }
      else if(v[(begin+end)/2] < v[end]){
	swap(v,begin,end); // swap pivot and first
	return;
      }
      else {
	p = (begin+end)/2; // middle element is median
	swap(v,begin,p); // swap pivot and first
	return;
      }

    }
  }
  else{ // default
    return;
  }
}

// Quicksort function
// Gets array v, begin/end location, and type (1,2,3,4)
// Returns how many comparisions is made
// Assume begin elements is pivot
int QuickSort(int* v, int begin, int end, int type){
  // breakout condition
  if (begin >= end) return 0;
  
  // index
  int i = begin + 1; // store left most elements
  int j; // start at begin+1

  // ChoosePivot
  ChoosePivot(v, begin, end, type);

  // QuickSort algorithm
  // make partition
  for (j = begin + 1; j <= end; j++){
    if(v[j] < v[begin]) {
      if(i == j) {
	i++; // if i, j point same elements, no need to swap
      }
      else {
	swap(v,i,j);
	i++;
      }
    }
  }
  // determine pivot location
  swap(v,begin,i-1);

  // recursively sort two partitions
  return (end-begin) + QuickSort(v,begin,i-2,type) + QuickSort(v,i,end,type); // return count
}

// Test QuickSort in the main
int main(){
  // storing array
  int *a = (int*) malloc(Max*sizeof(int));
  // backup pointer
  int *backup;
  // store array length
  int len = 0;
  // comparison counter
  int count = 0;

  // get array
  len = GetNum(a);

  // realloc a
  // backup
  backup = a;
  // reduce memory
  a = (int *) realloc(a, len*sizeof(int));
  if(a == NULL){ // if failed
    free(backup); // free old a
    assert(0); // assert
  }

  // sort array
  count = QuickSort(a,0,len-1,3);
  
  // for debug
  // print sorted array
  for(int i=0; i<len; i++) {
    printf("%d ",a[i]);
  }
  printf("\n");
  // for debug

  // print result
  printf("First: %d\n",count);

  // free all memory
  free(a);
  
  return 0;
}
