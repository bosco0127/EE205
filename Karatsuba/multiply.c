// 20200029 Kong Hyun Deok Project 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h> // for gettimeofday function

#define Maxlen 1024
#define Totallen 2048

// get number in string type
void GetNumStr(char str[Maxlen+2], int *len) {
  // get string from stdin
  if(fgets(str,Maxlen+2,stdin) != NULL) {
    *len = strlen(str)-1;
    // if digit is bigger than maximum length,
    // and exit program.
    if(str[Maxlen] != '\n' && str[Maxlen] != '\0') {
      printf("The digit is bigger than 1024\n");
      exit(1);
    }
  }
  else {
    // if fgets got EOF exit
    exit(1);
  }

  return;
}

// store string to number array
// needs string, number array, and its length
// *number array must be cleared
void StrToNum(char *str, int *num, int len) {
  // store string to num
  for(int i=0; i<len; i++) {
    if(str[i]>='0' && str[i]<='9') { // number only
      num[i] = str[len-i-1]-'0'; // store inversly
    }
    else
      break;
  }
  return;
}

// Get length of Number
// need array and sizeof array
// return length of number
int GetNumLen(int *num, int size) {
  int len=0; // return value
  // searching inversly until it met first element which is not zero
  for(int i=size; i>0 ; i--) {
    if(num[i-1]==0)
      continue;
    else{
      len=i;
      break;
    }
  }

  return len;
}

// print Number array
// needs name of array, number array, and its length
void PrintNum(char c, int *num, int len) {
  // print array named c
  printf("%c:",c);
  for(int i=len; i>0; i--) {
    printf("%d",num[i-1]);
  }
  printf("\n");
  
  return;
}

// Primitive Add operation(C = C + A + B)
// needs A, B, maximum length of A,B,C, and target C
// *C must be cleared if you want C = A + B
// *else, it would compute C = C + A + B
// *Each operand A and B must be different from target C
void PrimitiveAdd(int* A, int* B, int* C, int len) {
  //carry is going to be stored at the element of C
  for(int i=0;i<len;i++) {
    if( A[i]+B[i]+C[i] >= 10 ){
      C[i+1] = C[i+1] + (A[i]+B[i]+C[i])/10; // store carry
    }
    C[i] = (A[i]+B[i]+C[i])%10;
  }
  
  return;
}

// Primitive Subtraction ( C = A - B > 0)
// needs A, lenA, B, C
// *A must be greater than B
// *C must be cleared
// *Each operand A and B must be different from target C
void PrimitiveSub(int* A, int lenA, int* B, int* C) {
  //carry is going to be stored at the element of C
  for(int i=0; i<lenA; i++) {
    if( A[i]+C[i] < B[i] ) {
      C[i+1] = C[i+1] - 1; // borrow carry
      C[i] = C[i] + 10; // add carry
    }
    C[i] = A[i] + C[i] - B[i]; // store substraction result
  }
  
  return;
}

// Initialize all the elements of array to 0
// need array and size of array
void ClearNum(int *num, int size) {
  for(int i=0; i<size; i++) {
    num[i]=0;
  }
  return;
}

// Copy array
// need source, destination, and length of source
void CopyNum(int *src, int *dst, int lensrc) {
  // copy each element one by one
  for(int i=0; i<lensrc; i++) {
    dst[i] = src[i];
  }

  return;  
}

// Grade-School Multiply
// need two operand array and their length, and target array
// *Target array must be cleared
void GradeSchool(int* A, int lenA, int* B, int lenB, int* C) {
  // length of C
  int lenC=0;
  
  // empty array for PrimitiveAdd
  // since PrimitiveAdd does not allow using destination as an input,
  // we need temp to for that is empty
  int temp[Totallen]={0};
  int lentemp=0;

  // store middle result
  int middle[Totallen]={0};
  int lenmid=0;
  
  // store multiply(1 digit) and carry
  int mul=0;
  int carry=0;
  int len=0;

  // multiply
  for(int j=0; j<lenB; j++) {   
    // update middle
    for(int i=0; i<lenA; i++) {
      mul = (A[i]*B[j]+middle[i+j])%10;
      carry = (A[i]*B[j]+middle[i+j])/10;
      middle[i+j] = mul;
      middle[i+j+1] = carry;
    }
    lenmid=GetNumLen(middle,Totallen);
    lentemp=GetNumLen(temp,Totallen);
    // get bigger length
    len = (lenmid > lentemp) ? lenmid : lentemp;
    // C = middle + temp
    PrimitiveAdd(middle,temp,C,len);
    lenC=GetNumLen(C,Totallen);
    
    // middle = 0
    ClearNum(middle,lenmid);
  }
  
  return;
}

// Karatsuba Algorithm
// C = acx10^n + (ad+bc)x10^(n/2) + bd & Gauss's Trick
// n = (lenmax+1)/2 to handle odd length
// for example A=5 => a=0,b=5
// need two operand arrays, maximum length, and target array
// *Target must be cleared
void Karatsuba(int* A, int* B, int len, int* C) {
  // if len == 0, C = 0 * 0 = 0
  if(len == 0) {
    return;
  }

  // else if len = 1, compute primitive multiply
  if(len == 1) {
    C[0] = (A[0]*B[0])%10; // store remainder
    C[1] = (A[0]*B[0])/10; // store quotient
    return;
  }

  // compute half length
  int half = (len+1)/2;

  // else make temporary memory
  // store ac
  int *ac = (int*)calloc(len+3,sizeof(int));
  int len_ac = 0;
  // store bd
  int *bd = (int*)calloc(len+3,sizeof(int));
  int len_bd = 0;
  // store (a+b)(c+d): temp
  int *temp = (int*)calloc(len+3,sizeof(int));
  int len_temp = 0;
  // temp1
  int *temp1 = (int*)calloc(len+3,sizeof(int));
  int len_temp1 = 0;
  // temp2
  int *temp2 = (int*)calloc(len+3,sizeof(int));
  int len_temp2 = 0;

  // compute ac
  len_temp1 = len-half; // store len-half
  Karatsuba(A+half,B+half,len_temp1,ac); // ac=a*c
  len_ac = GetNumLen(ac,len+3); // length of ac

  // compute bd
  CopyNum(A,temp1,half); // temp1 = b
  CopyNum(B,temp2,half); // temp2 = d
  Karatsuba(temp1,temp2,half,bd); // bd=b*d
  len_bd = GetNumLen(bd,len+3); // length of bd
  memset(temp1,0,(len+3)*sizeof(int)); // clear temp1
  memset(temp2,0,(len+3)*sizeof(int)); // clear temp2

  // compute (a+b)(c+d)
  PrimitiveAdd(A,A+half,temp1,half); // temp1 = a + b
  PrimitiveAdd(B,B+half,temp2,half); // temp2 = c + d
  len_temp1 = GetNumLen(temp1,len+3); // length of a+b
  len_temp2 = GetNumLen(temp2,len+3); // length of c+d
  len_temp = (len_temp2 > len_temp1) ? len_temp2 : len_temp1; // get len.max
  Karatsuba(temp1,temp2,len_temp,temp); // temp = (a+b)(c+d)
  len_temp = GetNumLen(temp,len+3); // length of (a+b)(c+d)
  memset(temp1,0,(len+3)*sizeof(int)); // clear temp1
  memset(temp2,0,(len+3)*sizeof(int)); // clear temp2

  // compute ac+bd
  len_temp1 = (len_bd > len_ac) ? len_bd : len_ac; // get len.max(bd,ac)
  PrimitiveAdd(ac,bd,temp1,len_temp1); // temp1 = ac+bd

  // compute ad+bc = (a+b)(c+d) - (ac+bd)
  PrimitiveSub(temp,len_temp,temp1,temp2); // temp2 = ad+bc
  len_temp2 = GetNumLen(temp2,len+3); // length of ad+bc

  // compute C = acx10^n + (ad+bc)x10^(n/2) + bd
  memset(temp1,0,(len+3)*sizeof(int)); // clear temp1
  PrimitiveAdd(temp1,bd,C,len_bd); // C = bd
  PrimitiveAdd(temp1,temp2,C+half,len_temp2); // C = C + (ad+bc)x10^(n/2)
  PrimitiveAdd(temp1,ac,C+2*half,len_ac); // C = C + acx10^n

  // free all
  free(ac);
  free(bd);
  free(temp);
  free(temp1);
  free(temp2);

  // done

  return;
}

int main()
{
  // for storing start/end time
  struct timeval startTime, endTime;
  /* for storing  (endTime.tv_sec - starTime.tv_sec) and 
     (endTime.tv_usec - starTime.tv_usec) */
  double Time_usec;

  // store input string
  char str[Maxlen+2]={0};
  
  // operand A and B
  int A[Maxlen]={0};
  int B[Maxlen]={0};
  int C[Totallen]={0};
  
  // length of A,B,C and maximum length between A,B
  int lenA=0;
  int lenB=0;
  int lenC=0;
  int len=0;
  
  // get A from stdin
  GetNumStr(str, &lenA);

  // store string into A
  StrToNum(str, A, lenA);

  // get B from stdin
  GetNumStr(str, &lenB);

  // store string into B
  StrToNum(str, B, lenB);

  // store maximum length to len
  len = (lenA > lenB) ? lenA : lenB;

  // Grade-School test
  printf("Grade-School test\n");
  // Measure delay of 1000 cycle
  gettimeofday(&startTime, NULL); // starttime
  for(int i=0; i<1000; i++) {
    // **must clear C initially
    ClearNum(C,lenC);
    GradeSchool(A,lenA,B,lenB,C);
    lenC = GetNumLen(C,Totallen);
  }
  gettimeofday(&endTime, NULL); // endtime
  // calculate average time(usec)
  Time_usec = (endTime.tv_sec - startTime.tv_sec)*1000
    +(endTime.tv_usec - startTime.tv_usec)/1000;
  // print result
  lenC = GetNumLen(C,Totallen);
  PrintNum('C', C, lenC); // print C
  printf("Grade-school:%.3f usec\n",Time_usec); // print average time

  // Karatsuba test
  printf("Karatsuba test\n");
  // Measure delay of 1000 cycle
  gettimeofday(&startTime, NULL); // starttime
  for(int i=0; i< 1000; i++) {
    ClearNum(C,lenC); // clear C
    Karatsuba(A,B,len,C);
    lenC = GetNumLen(C,Totallen);
  } // do 1000 cycle
  gettimeofday(&endTime, NULL); // endtime
  // calculate average time(usec)
  Time_usec = (endTime.tv_sec - startTime.tv_sec)*1000
    +(endTime.tv_usec - startTime.tv_usec)/1000;
  // print result
  lenC = GetNumLen(C,Totallen);
  PrintNum('C', C, lenC); // print C
  printf("Karatsuba:%.3f usec\n",Time_usec); // print average time

  return 0;

}
