#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct list {
  char* data;
  int count;
  struct list* next;
};

struct tree {
      struct tree* left;
      struct tree* right;
      char* data;
      int count;
};


struct tree* get_empty_leaf()
{
      struct tree* leaf;

      leaf=(struct leaf*) malloc(sizeof(struct tree));
      leaf->left=NULL;
      leaf->right=NULL;
      leaf->count=0;
      leaf->data=NULL;

      return leaf;
}
      
struct list* get_empty_item()
{
      struct list* out;
      out = (struct list*) malloc(sizeof(struct list));
      out->next=NULL;
      out->data=NULL;
      out->count=0;

      return out;
}

void show(int level,struct list* root)
{
      if (root->data) {
	printf("%d: %s %d\n",level,root->data,root->count);
      }

      if (root->next) {
            show(level+1,root->next);
      }
      return;
}

// adds an item at the end of the list.
// The data are taken from the leaf
void add2list(struct list* head,struct tree* leaf)
{
  // first element of a list
  if (!head->data) {
    head->data=leaf->data;
    head->count=leaf->count;
    return;
  }
  
  while (head && head->next)
    head=head->next;

  // case of empty list
  
  head->next=get_empty_item();
  head->next->count=leaf->count;
  head->next->data=leaf->data;

  return;
}

void showW(int indent,struct tree* root,struct list* list_head)
{
  if (!root)
    return;
  
  if (root->left) {
    showW(indent+1,root->left,list_head);
  }

  if (root->data) {
    printf("%d: %s %d\n",indent,root->data,root->count);
    add2list(list_head,root);
  }
  showW(indent+1,root->right,list_head);

  return;
}

void add_word2tree(char* word,struct tree* root)
{
      int test;
      struct tree** tmp;
      
      if (!root) {
            return;
      }


      if (!root->data) {
            root->data=word;
            root->count=1;
            return;
      } 

      test=strcmp(root->data,word);

      if (!test) {
            root->count++;
            return;
      }

      
      if (test>0) {
            tmp=&(root->left);
      } else {
            tmp=&(root->right);
      }


      if(!(*tmp)) {
            *tmp=get_empty_leaf();
            (*tmp)->data=word;
            (*tmp)->count=1;
      } else {
            add_word2tree(word,*tmp);
      }
      
      return;
      
}

// simple chack for alphabet characters
int is_ok(char inp)
{
      if ((inp >='A' && inp <='Z') || (inp >='a' && inp <='z'))
            return 1;
      return 0;
}


// catch regular characters and UTF8 pilish diacritics
int is_ok2(unsigned char* inp)
{
      if ((*inp >='A' && *inp <='Z') || (*inp >='a' && *inp <='z'))
            return 1;


      /* utf-8 special cases */

      if ((*inp) == 0xC3) {
            if (*(inp+1) == 0x93)
                  return 2;
            if (*(inp+1) == 0xb3)
                  return 2;

      }

      if (*inp==0xc4) {
            if (*(inp+1) == 0x84)
                  return 2;
            if (*(inp+1) == 0x86)
                  return 2;
            if (*(inp+1) == 0x98)
                  return 2;
            if (*(inp+1) == 0x99)
                  return 2;
            if (*(inp+1) == 0x85)
                  return 2;
            if (*(inp+1) == 0x87)
                  return 2;
      }


      if (*inp==0xc5) {
            if (*(inp+1) == 0x81)
                  return 2;
            if (*(inp+1) == 0x83)
                  return 2;
            if (*(inp+1) == 0x9a)
                  return 2;
            if (*(inp+1) == 0xb9)
                  return 2;
            if (*(inp+1) == 0xbb)
                  return 2;
            if (*(inp+1) == 0x82)
                  return 2;
            if (*(inp+1) == 0x84)
                  return 2;
            if (*(inp+1) == 0x9b)
                  return 2;
            if (*(inp+1) == 0xba)
                  return 2;
            if (*(inp+1) == 0xbc)
                  return 2;
      }


      
      return 0;
}

// takes the string and fills the tree with
// words from this string
void extract_words(char* string,struct tree* root)
{
      char* word;
      int default_len=8;
      int current_len=0;
      int chtest=0;
      
      if (!string || *string=='\0')
            return;

      word = (char*) malloc(default_len);
      
      while (*string!='\0' && !is_ok(*string))
            string++;

      while ((chtest=is_ok2(string))) {
            word[current_len++]=*string++;
            if (chtest==2) // 2 byte utf
                  word[current_len++]=*string++;

            if (current_len==default_len-2) {
                  default_len*=2;
                  word=realloc(word,default_len);
            }
      }

      if (current_len>0) {
            printf("extracted %s %d\n",word,current_len);
            word[current_len]='\0';
            add_word2tree(word,root);
      }
      
      extract_words(string,root);
      
      return;
}

// recursively parses a line from input
// stream and fills the binary tree
void eat_one_line(FILE* input,struct tree* root)
{
      char *buf=NULL;
      int buflen=0;
      int b_read=0;

      b_read=getline(&buf,&buflen,input);
      struct list* item=NULL;

      if (b_read!=-1) {
            extract_words(buf,root);
            free(buf);
            eat_one_line(input,root);
      }
      return;
}

int main(int argc,char** argv)
{
      FILE* input=NULL;
      int   b_read=0;
      
      struct list* lroot=NULL;
      struct tree* dict=NULL;

      if (!(input=fopen(argv[1],"r"))) {
            exit(1);
      }




      // debug stuff: a good practise is not to
      // throw debug stuff away
      /* add_word2tree("ala",dict); */
      /* add_word2tree("ala",dict); */
      /* add_word2tree("ma",dict); */
      /* add_word2tree("kota",dict); */
      /* add_word2tree("a",dict); */
      /* add_word2tree("nawet",dict); */
      /* add_word2tree("dwa",dict); */
      /* extract_words("Tomek kupił dwie lokomotywy",dict); */


      // create dict (btree) and fill it with file content
      dict=get_empty_leaf();
      eat_one_line(input,dict); 

      // create list and fill the list (and show the tree)
      // BFS (show iplicitely fills lroot tree)
      //
      // showW could be named tree2list, provided the
      // printf there would be commented out.
      //
      // 
      lroot=get_empty_item();
      showW(0,dict,lroot);

      // (show the list)
      printf("================\n");
      show(0,lroot);

      
}
     
