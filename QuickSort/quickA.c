/* EE205 Project2: QuickSort & ChoosePivot */
/* Written by 20200029 HyunDeok Kong */
#include <stdio.h>

// Swap i & j-th element of array v
// Gets array v, and two integers i, j
void swap(int* v, int i, int j){
  // temporary memory
  int temp=v[i];
  v[i] = v[j];
  v[j] = temp;
  return;
}

// Quicksort function
// Gets array v, begin/end location
// Returns how many comparisions is made
// Assume begin elements is pivot
int QuickSort(int* v, int begin, int end){
  // breakout condition
  if (begin >= end) return 0;
  
  // index
  int i = begin + 1; // store left most elements
  int j; // start at begin+1

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

  // for debug
  // print sorted array
  for(int i=0; i<20; i++) {
    printf("%d ",v[i]);
  }
  printf("\n"); // for debug

  // recursively sort two partitions
  return (end-begin) + QuickSort(v,begin,i-2) + QuickSort(v,i,end); // return count
}

// Test QuickSort in the main
int main(){
  // storing array
  int a[] = {2,4,6,8,10,12,14,16,18,20,1,3,5,7,9,11,13,15,17,19};
  // comparison counter
  int count = 0;

  // sort array
  count = QuickSort(a,0,19);
  
  // print sorted array
  for(int i=0; i<20; i++) {
    printf("%d ",a[i]);
  }
  printf("\n");

  // print result
  printf("First: %d\n",count);
  
  return 0;
}
