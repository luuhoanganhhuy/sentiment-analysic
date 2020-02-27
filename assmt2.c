/*Algorithms are fun */
/*Answer code written by Luu Hoang Anh Huy, Sid 1025379, May 2019*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


#define DICTIONARY_LEN 100 /*Maximun length of the sentiment dictionary*/
#define MAX_WORD_LEN 23 /*Maximun length of a word*/
#define MAX_FORMS_LEN 100 /*Maximun length of the variation froms*/

typedef char word_t[MAX_WORD_LEN+1]; /*a word*/

typedef char line_t[MAX_FORMS_LEN+1]; /*a line*/

typedef struct{
  word_t word;
  int score;
  line_t forms;
} dict_t;

typedef struct node node_t;

struct node {
	word_t data;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;

/* function prototypes */
void stage_one(dict_t *one_dictionary);
void read_one_line(line_t one_line, int max_len);
void stage_two(dict_t *dictionaries, int *num_dictionaries);
void stage_three(list_t *list);
void stage_four(list_t *list, int num_dictionaries, dict_t *dictionaries);
int getword(char W[], int limit);
list_t *insert_at_foot(list_t *list, word_t value);
list_t *make_empty_list(void);
void free_list(list_t *list);
int string_compare(void const *v1, void const *v2);
void print_list(list_t *list);
void print_ave_char(dict_t *dictionaries, int num_dictionaries);
void print_ave_score(dict_t *dictionaries, int num_dictionaries);

/*main function*/
int
main( int argc, char*argv[]){
  dict_t dictionaries[DICTIONARY_LEN];
  int num_dictionaries = 0;

	/*stage 1: read one dictionary word*/
  stage_one(dictionaries);
  num_dictionaries++;

	/*stage 2: read the whole dictionary*/
  stage_two(dictionaries,&num_dictionaries);

	/*stage 3: read the sentence*/
	list_t *list = calloc (1, sizeof *list);
	stage_three(list);

	/*Stage 4: label and calculate the sentiment score*/
	stage_four(list, num_dictionaries, dictionaries);

	/*free list*/
  free_list(list);

  /*Stage5: handling the variation from*/
	printf("=========================Stage 5=========================\n");

  return 0;
}

/*read a line of input */
/*contributed to the read_one_tweet function from assignment 1 COMP10002*/
void
read_one_line(line_t one_line, int max_len) {
	int i = 0, c;

	while (((c = getchar()) != EOF) && (c != '\n') && ( c != '\r')) {
		if (i < max_len) {
			one_line[i++] = c;
		} else {
			printf("Invalid input line, toooooooo long.\n");
			exit(EXIT_FAILURE);
		}
	}
	one_line[i] = '\0';
}

/*read one dictionary word*/
void stage_one(dict_t *one_dictionary){
	/* print stage header */
	printf("=========================Stage 1=========================\n");

  /*read the first 3 line of the input*/
  int new_line =0;
  line_t one_line;
  while (new_line <3){
    read_one_line(one_line, MAX_FORMS_LEN);
    if (new_line == 0){
      /*skip the '$' and get the word*/
      memmove(one_line, one_line+1, strlen(one_line));
      strcpy(one_dictionary->word,one_line);
    }
    if (new_line == 1){
      /*get the sentiment score*/
      one_dictionary->score=atoi(one_line);
    }
    if (new_line == 2){
      /*skip the '#' and get the forms*/
      memmove(one_line, one_line+1, strlen(one_line));
      strcpy(one_dictionary->forms,one_line);
    }
    new_line++;
  }

  /*print the one dictionary word*/
  printf("First word: %s\n",one_dictionary->word);
  printf("Sentiment score: %d\n",one_dictionary->score);
  printf("Forms: %s\n\n",one_dictionary->forms);
}

/*calculate the average number of characters*/
void print_ave_char(dict_t *dictionaries, int num_dictionaries){
  int sum_char= 0;
  double ave_char;
  for (int i=0; i< num_dictionaries;i++){
    sum_char += strlen(dictionaries[i].word);
  }
  ave_char =(double)(sum_char)/(double)(num_dictionaries);
  printf("Average number of characters: %.2f\n",ave_char);
}

/*calculate the average sentiment score */
void print_ave_score(dict_t *dictionaries, int num_dictionaries){
  int sum_score=0;
  double ave_score;
  for (int i=0; i< num_dictionaries;i++){
    sum_score += dictionaries[i].score;
  }
  ave_score =(double)(sum_score)/(double)(num_dictionaries);
  printf("Average sentiment score: %.2f\n\n",ave_score);
}

/*read the whole dicionary*/
void stage_two(dict_t *dictionaries, int *num_dictionaries ){
  line_t one_line;
  /*print stage header*/
	printf("=========================Stage 2=========================\n");

  /*terminate when the next value of word begins with '%' */
  while (dictionaries[*num_dictionaries-1].word[0]  != '%'){
    int new_line =0;
    while (new_line < 3){
      read_one_line(one_line, MAX_FORMS_LEN);
      if (new_line == 0){
        /*skip the '$' and get the word*/
        memmove(one_line, one_line+1, strlen(one_line));
        strcpy((dictionaries+*num_dictionaries)->word,one_line);
      }
      if (new_line == 1){
        /*get the sentiment score*/
        (dictionaries+*num_dictionaries)->score=atoi(one_line);
      }
      if (new_line == 2){
        /*skip the '#' and get the forms*/
        memmove(one_line, one_line+1, strlen(one_line));
        strcpy((dictionaries+*num_dictionaries)->forms,one_line);
      }
      if (dictionaries[*num_dictionaries].word[0] == '%'){
        /*terminate when hit the '%'*/
        break;
      }
      new_line++;
    }
    *num_dictionaries = *num_dictionaries +1;
  }
  *num_dictionaries = *num_dictionaries -1;

  /*print number of dictionary words*/
	printf("Number of words: %d\n",*num_dictionaries);

  /*print the average number of characters*/
  print_ave_char(dictionaries,*num_dictionaries);

  /*print the average sentiment score*/
  print_ave_score(dictionaries,*num_dictionaries);
}

/*getword() function contributed to lecturer COMP10002*/
int
getword(char W[], int limit) {
	int c, len=0;
	/* first, skip over any non alphabetics */
	while ((c=getchar())!=EOF && !isalpha(c)) {
		/* do nothing more */
	}
	if (c==EOF) {
		return EOF;
	}
	/* ok, first character of next word has been found */
	W[len++] = c;
	while (len<limit && (c=getchar())!=EOF && isalpha(c)) {
		/* another character to be stored */
		W[len++] = c;
	}
	/* now close off the string */
	W[len] = '\0';
	return 0;
}

/* make_empty_list, free_list and insert_at_foot contributed to listops
in COMP10002 lecture*/
list_t
*make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}

list_t
*insert_at_foot(list_t *list, word_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	strcpy(new->data, value);
	new->next = NULL;
	if (list->foot==NULL) {
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

/*print the value in the list */
void print_list(list_t *list){
	node_t *new;
	new = list->head;
	while(new != NULL) {
		printf("%s\n", new->data);
		new = new->next;
	}
  printf("\n");
}

/*read the sentence*/
void stage_three(list_t *list){

  /*print the stage header*/
	printf("=========================Stage 3=========================\n");
	word_t value;

  /*create a empty list*/
	make_empty_list();

  /*insert word to the list*/
	while(getword(value,MAX_WORD_LEN)!= EOF){
		insert_at_foot(list,value);
	}

  /*print the list*/
	print_list(list);
}

/*compare two string*/
int string_compare(void const *v1, void const *v2) {
	const char **s1= (const char **) v1;
  const char **s2 = (const char **)v2;
	return strcmp(*s1,*s2);
}

/*label and calculate the sentiment score*/
void stage_four(list_t *list, int num_dictionaries, dict_t *dictionaries){
  /*print the stage header*/
  printf("=========================Stage 4=========================\n");

  /*create an array of dictionary words*/
	char *dictionaries_arr[num_dictionaries+1];
	char **ptr = dictionaries_arr;
	int i;
  for (i=0; i < num_dictionaries;i++){
		ptr[i] = dictionaries[i].word;
	}

  /*using binary search function to look up each word in the sentence*/
	int overall_score = 0;
	char *item;
	node_t *new;
	new = list->head;
	while (new != NULL){
		char * p = new -> data;
		item = (char*) bsearch (&p,dictionaries_arr, num_dictionaries,
      sizeof (char*), string_compare);
		if(item != NULL) {
			for (int i=0; i < num_dictionaries; i++){
				if (!strcmp(dictionaries[i].word,p)){
						printf("%-24s %d\n", p, dictionaries[i].score);

            /*calculatethe overall sentiment score*/
						overall_score += dictionaries[i].score;
				}
			}
		} else {
			printf("%-24s %d\n", p,0);
		}
		new = new->next;
	}
	printf("Overall score: %11d\n\n",overall_score);
}
