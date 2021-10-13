/* EE205 Project2: QuickSort & ChoosePivot */
/* Written by 20200029 HyunDeok Kong */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

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

// Random Choose pivot function
// Gets array v, begin/end
// Swap pivot element and first element
void ChoosePivot(int *v, int begin, int end){
  // save pivot
  int p = 0;

  // for random
  srand(time(NULL));

  // Choose pivot Random
  // pick random pivot from begin to end
  p = rand() % (end-begin+1) + begin;
  
  swap(v,begin,p); // swap random and first
  
  return;
}

// Rselect function for searching (len/2)th-order statistic
// Gets array v, begin/end location, and statistic k
// Returns location of statistic
int RSelect(int* v, int begin, int end, int k){
  // breakout condition
  if (begin >= end) return begin;
  
  // index
  int i = begin + 1; // store left most elements
  int j; // start at begin+1

  // ChoosePivot randomly
  ChoosePivot(v, begin, end); // pivot

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
  swap(v,begin,i-1); // (i-1) is a new index of pivot

  // return
  if((i-1) == k) return (i-1); // return itself
  else if((i-1) > k) return RSelect(v, begin, i-2, k); // k is in 1st partition
  else return RSelect(v, i, end, k-i); // k is in 2nd partition of k-(i-1)-1

}

/*
// Merge Sort for 5 elements
void Merge(int *v, int begin, int end){
  // breakout condition
  if(begin >= end) return;

  // Partition
  int mid = (begin+end)/2;
  int *a=(int*)malloc((mid-begin+1)*sizeof(int));
  int *b=(int*)malloc((end-mid)*sizeof(int));
  int i=0; // a index
  int j=0; // b index
  // copy
  memcpy(a,v,(mid-begin+1)*sizeof(int));
  for(int k=0;i<(end-mid);i++){
    b[k]=v[mid+k];
  }
  // recursive sort
  Merge(a,0,mid-begin);
  Merge(b,0,end-mid-1);
  
  // Merge partion
  for(int k=begin; k<=end; k++){
    if(i==(mid-begin)){
      v[k] = b[j++];
    }
    else if(j==(end-mid-1)){
      v[k] = a[i++];
    }
    else {
      if(a[i]<b[j]){
	v[k] = a[i++];
      }
      else
	v[k] = b[j++];
    }
  }

  // free a,b
  free(a);
  free(b);

  return;
}

// DSelect function
// Gets array v, length, statistic k.
int DSelect(int *v, int begin, int end, int k){
  // breakout condition
  if (begin >= end) return begin;

  // parameter
  int five = (end-begin)/5;
  int *C = (int *)malloc((five+1)*sizeof(int));
  int i=begin+1;
  int j=0;
  int p; // pivot index
  
  // sort each group
  for(int n=0;n<five;n++){
    Merge(v,begin+5*n,begin+5*n+4);
  }
  Merge(v,begin+5*five,end);

  // store median to C
  for(int n=0;n<five;n++){
    C[n]=v[begin+5*n+2];
  }
  C[five]=v[end];

  // reculsively select p
  p = DSelect(C,0,five,five/2);
  // free C
  free(C);

  // Partition v around p
  swap(v,begin,p);
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
  swap(v,begin,i-1); // (i-1) is a new index of pivot

  // return
  if((i-1) == k) return (i-1); // return itself
  else if((i-1) > k) return DSelect(v, begin, i-2, k); // k is in 1st partition
  else return DSelect(v, i, end, k-i); // k is in 2nd partition of k-(i-1)-1
}
*/


// Quicksort function
// Gets array v, begin/end location
void QuickSort(int* v, int begin, int end){
  // breakout condition
  if (begin >= end) return;

  // ChoosePivot
  // pivot must be middle
  int p = RSelect(v, begin, end, (begin+end)/2);

  // Partitions are already made in Rselect

  // Recursively sort
  QuickSort(v,begin,p-1);
  QuickSort(v,p+1,end);
}

// Print array function
// Gets array v, and array length len
// Print each elements in one line to the standard output.
void PrintNum(int* v, int len){
  // print
  for(int i=0; i<len; i++){
    printf("%d\n",v[i]);
  }
  return;
}

// %% for qsort function
int compare(const void *a, const void *b){
  int num1 = *(int *)a;
  int num2 = *(int *)b;

  if(num1 < num2){
    return -1;
  }
  else if(num1 > num2){
    return 1;
  }
  else
    return 0;
}

// Test QuickSort in the main
int main(int argc, char* argv[]){
  // storing array
  int *a = (int*) malloc(Max*sizeof(int));
  // copy array
  int *copy[2]; // 0: mine, 1: crt.
  // backup pointer
  int *backup;
  // store array length
  int len = 0;
  // comparison counter
  long count = 0;

  // for storing start/end time
  struct timeval startTime, endTime;
  /* for storing  (endTime.tv_sec - startTime.tv_sec) and 
     (endTime.tv_usec - starTime.tv_usec) */
  double Time_sec;
  
  // get array
  len = GetNum(a);

  // copy array
  for(int i=0; i<2; i++){
    copy[i] = (int*) malloc(len*sizeof(int));
    memcpy(copy[i],a,len*sizeof(int));
  }

  // for debug
  for(int i=0; i< argc;i++){
    fprintf(stderr,"argv[%d]: \"%s\"\n",i,argv[i]);
  }
  // for debug

  // **sort array**
  // 1. mine
  if(strcmp(argv[1],"mine") == 0) {
    // for debug
    fprintf(stderr,"running mine\n");
    // for debug
    gettimeofday(&startTime, NULL); // begin
    QuickSort(copy[0],0,len-1);
    gettimeofday(&endTime, NULL); // end
    // calculate time
    Time_sec = (endTime.tv_sec - startTime.tv_sec) +
      (endTime.tv_usec - startTime.tv_usec)/1000000.0;
    // print result of sorting
    PrintNum(copy[0],len);
    // print time on the standard error
    fprintf(stderr,"It took %.3lf seconds\n",Time_sec);
  }
  // 2. crt
  else if(strcmp(argv[1],"crt") == 0){
    // for debug
    fprintf(stderr,"running crt\n");
    // for debug
    gettimeofday(&startTime, NULL); // begin
    qsort(copy[1],len,sizeof(int),compare);
    gettimeofday(&endTime, NULL); // end
    // calculate time
    Time_sec = (endTime.tv_sec - startTime.tv_sec) +
      (endTime.tv_usec - startTime.tv_usec)/1000000.0;
    // print result of sorting
    PrintNum(copy[1],len);
    // print time on the standard error
    fprintf(stderr,"It took %.3lf seconds\n",Time_sec);
  }
  // 3. else, run with mine
  else {
    // for debug
    fprintf(stderr,"running else\n");
    // for debug
    gettimeofday(&startTime, NULL); // begin
    QuickSort(copy[0],0,len-1);
    gettimeofday(&endTime, NULL); // end
    // calculate time
    Time_sec = (endTime.tv_sec - startTime.tv_sec) +
      (endTime.tv_usec - startTime.tv_usec)/1000000.0;
    // print result of sorting
    PrintNum(copy[0],len);
    // print time on the standard error
    fprintf(stderr,"It took %.3lf seconds\n",Time_sec);
  }

  // free all memory
  free(a);
  for(int i=0; i<2; i++){
    free(copy[i]);
  }
  
  return 0;
}
