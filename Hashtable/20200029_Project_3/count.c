#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/time.h> // for the timing results

#define MAX_LINE 4096
#define MAX_WORD 1024

#define UNIT_ARRAY_SIZE 1024 /* Define unit array size */

#define min(a, b) ((a) < (b) ? (a) : (b))

enum {HASH_MULTIPLIER = 65599};
enum {OUTSIDE, INSIDE};

typedef struct DB* DB_T;

/* Define hash_function.
   Need pcKey(const char *), and iBucketCount(int).
   Return corressponding hash value. */
static int hash_function(const char *pcKey, int iBucketCount) {
  int i;
  unsigned int uiHash = 0U;
  for(i=0; pcKey[i] != '\0'; i++)
    uiHash = uiHash*(unsigned int)HASH_MULTIPLIER+(unsigned int)pcKey[i];
  return (int)(uiHash % (unsigned int)iBucketCount);
}

/* Structure of Word */
struct Word {
  char *word;                // word
  int count;              // count the number of word
  struct Word *next;   // word connection pointer
};

/* Hash table Structure */
struct DB {
  struct Word **Wordarray;   // Word pointer of pointer for dynamically resizeable array.
  int curArrSize;            // current hash table size
  int numItems;              // # of stored words, needed to determine
};

/* Create word data base.
   Needs no parameter to call. 
   Returns DB_T type pointer if it is success.
   If memory allocation is failed, it returns NULL. */
DB_T
CreateDB(void)
{
  /* Uncomment and use the following implementation if you want */
  DB_T d;
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE;

  /* Allocate memory for Word array */
  d->Wordarray = (struct Word **)calloc(d->curArrSize, sizeof(struct Word *));
  if (d->Wordarray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->curArrSize);   
    free(d);
    return NULL;
  }
  
  return d;
}

/* Delete all the memories of customer.
   Needs DB_T type parameter, d.
   If d is NULL, function do nothing. */
void
DestroyDB(DB_T d)
{
  /* fill out this function */
  /* If given pointer is NULL, do nothing */
  if(d == NULL) return;

  /* Define Word type pointers */
  struct Word *p; /* Variable for free the information */
  struct Word *nextp; /* Variable for storing next address */
  int i=0; /* index for array */

  /* Free data at hash table */
  for (i=0; i < d->curArrSize; i++) {
    for (p = d->Wordarray[i];p != NULL; p = nextp) {
      nextp = p->next; /* store next address */
      free(p->word); /* free word */
      free(p); /* free Word */
    }
  }
  free(d->Wordarray); /* free ID hash table array */
  free(d); /* free DB */
  return;
}

/* Add new word.
   Need d(DB_T), word(const char *).
   If there's NULL, function is failed.
   If number of word is full, expand memory by amount of UNIT_ARRAY_SIZE.
   If a word has already exists, add 1 to its count.
   If not, register new word and set its count to one.
   If register successfully, return 0. Else, return -1 */
int RegisterWord(DB_T d, const char *word)
{
  /* fill out this function */
  /* If any of d, word is NULL it is a failure */
  if((d==NULL)||(word==NULL)) return -1;

  /* Below part is for expanding array size of hash table. */
  /*-----------------------------------------------------------------------*/
  /* If number of item is 75% of current array size, expand memory
    until curArrSize <= 2^19(524288). Then, expansion would be stop at 2^20 */
  if ((d->numItems == 0.75*d->curArrSize) && (d->curArrSize <= 524288)) {
    
    /* Set new array for reallocation failed */
    struct Word **d_word_new; /* new hash table array for word */
    int ArrSize_backup = d->curArrSize; /* Array size backup */
    
    /* expand current array size multiply by 2 */
    d->curArrSize *= 2;
    
    /* callocate memory to new Word hash table array */
    d_word_new = (struct Word **)calloc(d->curArrSize,sizeof(struct Word *));
    /* If fail to reallocate memory, free all the memory and return -1 */
    if (d_word_new == NULL) {
      fprintf(stderr, "Can't reallocate a memory for array of size %d\n",
  	      d->curArrSize); /* print error message */
      d->curArrSize = ArrSize_backup; /* assign array size backup */
      return -1; /* Expanding Failure */
    }

    /* move already existing words to new array, and remove old array */
    /* Define Word type pointers */
    struct Word *p; /* Variable for free the information */
    struct Word *nextp; /* Variable for storing next address */
    int wordhash=0; /* variable for word hash value*/
    int i=0; /* index for array */
    
    /* move data from old hash table to new hash table */
    /* Also remove old memory as soon as transporting is completed */
    /* Search already existing items by word hash table */
    for (i=0; i < ArrSize_backup; i++) {
      for (p = d->Wordarray[i];p != NULL; p = nextp) {
	/* store next address */
	nextp = p->next;

	/* Find new hash value */
	wordhash = hash_function(p->word,d->curArrSize);

	/* move old items to new hash table */
	/* allocate memory for copy customer */
	struct Word *copy = (struct Word*)malloc(sizeof(struct Word)); /* pointer for new word */      
	copy->word = strdup(p->word); /* Own word */
	if(copy->word==NULL) return -1; /* In case of owning id or name is failed */
	copy->count = p->count; /* Store purchase value */
	
	/* Connecting old item to the both new ID and name hash table */
	copy->next = d_word_new[wordhash]; /* connect new Word to the hash table */
	d_word_new[wordhash] = copy; /* connect new Word to the hash table */
	
	/* remove old memory */
	free(p->word); /* free word */
	free(p); /* free Word */
      }
    }

    /* free old hash table */
    free(d->Wordarray); /* free ID hash table array */

    /* Assign new hash table to d */
    d->Wordarray = d_word_new; /* assign new hash table to d */
  }
  /*-----------------------------------------------------------------------*/
  
  /* Search whether id or name is already exist */
  struct Word *q; /* Pointer for searching */
  int wordhash = hash_function(word,d->curArrSize); /* hash value for word */

  /* Search whether word is already exist */
  for(q = d->Wordarray[wordhash]; q != NULL; q=q->next)
    /* If same id already exist, increase count by one
    and return 0 */
    if ((strcmp(q->word,word)) == 0) {
        q->count = q->count + 1;
        return 0;
    }

  /* Add customer */
  struct Word *p = (struct Word*)malloc(sizeof(struct Word)); /* pointer for new word */      
  p->word = strdup(word); /* Own word */
  if(p->word==NULL) return -1; /* In case of owning word is failed */
  p->count = 1; /* Store purchase value */

  /* Connecting new item to the hash table */
  p->next = d->Wordarray[wordhash]; /* connect new Word to the hash table */
  d->Wordarray[wordhash] = p; /* connect new Word to the hash table */
  d->numItems++; /* increase number of items */
  
  return 0; /* Register success */
}

int Token(DB_T d) {
    char buf[MAX_LINE], *temp;
	int  i, in_word = OUTSIDE, success;
	char str[MAX_WORD];

	/* read a line from stdin, break it into words,
	   and register each word to the hashtable */
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		for (temp = buf; *temp; temp++) {
			if (isalpha(*temp)) {
				if (in_word == OUTSIDE) {
					in_word = INSIDE;
					i = 0;
				}
				str[i++] = *temp;
			} else {
				if (in_word == INSIDE) {
					str[i] = 0;
					success = RegisterWord(d,str);
                    if(success) {
                        fprintf(stderr, "Register Failure\n"); // print error message
                        return -1; // return failure
                    }
				}
				in_word = OUTSIDE;
			}
		}
	}
	if (in_word == INSIDE) {
		str[i] = 0;
		success = RegisterWord(d,str);
        if(success) {
            fprintf(stderr, "Register Failure\n"); // print error message
            return -1; // return failure
        }
	}

    return 0; // return success
}

// Connect all the elements of d to the word_array
// Needs (DB_T) d, (struct Word**) word_array, (int) len
void ConnectWord(DB_T d, struct Word** word_array){

    /* Define Word type pointers */
    struct Word *p; /* Variable for free the information */
    struct Word *nextp; /* Variable for storing next address */
    int i=0; /* index for hash */
    int j=0; // index for array

    /* Connet all the items in the word hash to the word_array */
    for (i=0; i < d->curArrSize; i++) {
        for (p = d->Wordarray[i];p != NULL; p = nextp) {
            /* store next address */
	        nextp = p->next;
            /* connect Word to the hash table */
	        word_array[j++] = p;
        }
    }
    // for debug
    if(j != d->numItems) printf("Error: ConnecWord failure. Please retry.\n");
    // for debug

    return;
}

// Swap i & jth element
// Needs word_array(struct Word**), i,j(int)
void swap(struct Word** word_array, int i, int j){
    // temporal memory
    struct Word* temp=word_array[i];
    word_array[i] = word_array[j];
    word_array[j] = temp;
    return;
}

// Change string into non-capital string
// needs a(char *)
void StrLwr(char *a){
  int len=strlen(a);
  for(int i = 0; i < len; i++){
    if( *(a+i) <= 'Z' ) {
      *(a+i) = *(a+i) - 'A' + 'a';
    }
  }
  return;
}

// Compare string by dictionary order
// needs a,b(char *)
// returns result if a is upper/same/lower than b
int Dictionary(char *a, char *b) {
  // duplicate string
  char *word1 = strdup(a);
  char *word2 = strdup(b);
  // store result
  int result;

  // change word1, word2 to non-capital-character string.
  StrLwr(word1);
  StrLwr(word2);
  result = strcmp(word1,word2);
  if(result>0) {
    // free memory
    free(word1);
    free(word2);
    return result;
  }
  else if(result<0) {
    // free memory
    free(word1);
    free(word2);
    return result;
  }
  else {
    // free memory
    free(word1);
    free(word2);
    return -strcmp(a,b); // because A is upper than a
  }
}

// Choose pivot function by Median-of-three
// Needs word_array(struct Word**), begin,end(int)
// Swap pivot element and first element
void ChoosePivot(struct Word** v, int begin, int end){
    // save pivot
    int half = (begin + end)/2;
    int p = 0;

    // Choose pivot
    if(v[begin]->count < v[half]->count){
      if(v[half]->count < v[end]->count){
	p = half; // middle element is median
	swap(v,begin,p); // swap pivot and first
	return;
      }
      else if(v[begin]->count < v[end]->count){
	swap(v,begin,end); // swap pivot and first
	return;
      }
      else {
	return; // pivot is first
      }
    }
    else{ // v[begin]->count > v[middle]->count
      if(v[begin]->count < v[end]->count){
	return; // pivot is first
      }
      else if(v[half]->count < v[end]->count){
	swap(v,begin,end); // swap pivot and first
	return;
      }
      else {
	p = half; // middle element is median
	swap(v,begin,p); // swap pivot and first
	return;
      }
    }
}

// Quicksort function: sort in the inverse order
// If the number of the count of each word is same,
// sort the word in the inverse order of the dictionary order.
// Gets word_array(struct Word**), begin,end(int)
// Assume begin elements is pivot
void QuickSort(struct Word** v, int begin, int end){
  // breakout condition
  if (begin >= end) return;
  
  // index
  int i = begin + 1; // store left most elements
  int j; // start at begin+1

  // ChoosePivot
  ChoosePivot(v, begin, end);

  // QuickSort algorithm
  // make partition
  for (j = begin + 1; j <= end; j++){
    if(v[j]->count > v[begin]->count) {
        if(i == j) {
        	i++; // if i, j point same elements, no need to swap
        }
        else {
        	swap(v,i,j);
        	i++;
        }
    }
    // sort the word in the inverse order of the dictionary order.
    // strlwr() function is that change all the characters into the lower characters.
    else if((v[j]->count == v[begin]->count) && (Dictionary(v[j]->word, v[begin]->word)) > 0) {
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
  QuickSort(v,begin,i-2);
  QuickSort(v,i,end);
}

int main(int argc, char* argv[]) {
    /* for the timing results */
    /*-------------------------------------------------------------------------*/
    // for storing start/end time
    struct timeval startTime, endTime;
    /* for storing  (endTime.tv_sec - startTime.tv_sec) and 
     (endTime.tv_usec - starTime.tv_usec) */
    double Time_sec;
    /*-------------------------------------------------------------------------*/

    // print error message and stop program if the form is wrong
    if(atoi(argv[1])<=0){
        printf("Error: type in form of \"./count (natural number) < file\"\n");
        return 0;
    }

    /* for the timing results */
    /*-------------------------------------------------------------------------*/
    gettimeofday(&startTime, NULL); // begin
    /*-------------------------------------------------------------------------*/

    // store n
    int n = atoi(argv[1]); // d->numItems;

    // create hash table
    DB_T d = CreateDB();
    if(d == NULL) { // If it is failed to creating DB,
        fprintf(stderr, "Failed to create DB\n"); // print error message
        assert(0); // assert program
    }

    // variable for verifying whether the program is success or not
    int success;

    // 1. Register words to hash table.
    success = Token(d);
    if(success) {
        printf("Error: Register Failure. Please retry.\n"); // print error message
    }

    // 2. Align all the elements of the hash table to the array.
    struct Word ** word_array = (struct Word **)calloc(d->numItems,sizeof(struct Word *));
    if (word_array == NULL) {
      fprintf(stderr, "Can't reallocate a memory for array of size %d\n",
  	      d->numItems); /* print error message */
      return -1; /* Expanding Failure */
    }
    ConnectWord(d,word_array);

    // 3. QuickSort the array by count.
    QuickSort(word_array,0,d->numItems-1);

    // 4. Print the array.
    for(int i=0; i < min(n,d->numItems) ; i++){
        printf("%7d %s\n",word_array[i]->count,word_array[i]->word);
    }

    // Destroy DB & word_array
    DestroyDB(d);
    free(word_array);

    /* for the timing results */
    /*-------------------------------------------------------------------------*/
    gettimeofday(&endTime, NULL); // end
    // calculate time
    Time_sec = (endTime.tv_sec - startTime.tv_sec) +
      (endTime.tv_usec - startTime.tv_usec)/1000000.0;
    // print time on the standard error
    fprintf(stderr,"It took %.3lf milliseconds\n",1000*Time_sec);
    /*-------------------------------------------------------------------------*/

    return 0;
}