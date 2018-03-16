#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ssize_t getline(char **lineptr, size_t *n, FILE *stream);

struct list {
      char* data;
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

      return out;
}

void show(int level,struct list* root)
{
      if (root->data) {
            printf("%d: %s",level,root->data);
      }

      if (root->next) {
            show(level+1,root->next);
      }
      return;
}

void showT(int indent,struct tree* root)
{

      if (!root) {
            printf("%d NULL\n",indent);
            return;
      }
            
      if (indent==0) {
            if (root->data) {
                  printf("%d: %s %d\n",indent,root->data,root->count);
            }
      }

      if (root->left) {
            printf("%d: %s %d\n",indent+1,root->left->data,root->left->count);
      }

      if (root->right) {
            printf("%d: %s %d\n",indent+1,root->right->data,root->right->count);
      }

      showT(indent+1,root->left);
      showT(indent+1,root->right);
      
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


int is_ok(char inp)
{
      if ((inp >='A' && inp <='Z') || (inp >='a' && inp <='z'))
            return 1;
      return 0;
}



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

void add_one_line(FILE* input,struct list* root)
{
      char *buf=NULL;
      int buflen=0;
      int b_read=0;

      b_read=getline(&buf,&buflen,input);
      struct list* item=NULL;

      if (b_read!=-1) {
            fprintf(stderr,"newline %d\n",b_read);
            if (!root->data) {
                  root->data=buf;
                  root->next=NULL;
                  add_one_line(input,root);
            } else {
                  item=get_empty_item();
                  item->data=buf;
                  item->next=NULL;
                  root->next=item;

                  add_one_line(input,item);
            }
      } else {
            return;
      }
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


      lroot=get_empty_item();
      //      add_one_line(input,lroot);

      show(0,lroot);

      dict=get_empty_leaf();

      add_word2tree("ala",dict);
      add_word2tree("ala",dict);
      add_word2tree("ma",dict);
      add_word2tree("kota",dict);
      add_word2tree("a",dict);
      add_word2tree("nawet",dict);
      add_word2tree("dwa",dict);

      extract_words("Tomek kupił dwie lokomotywy",dict);

      eat_one_line(input,dict);
      
      showT(0,dict);


      
}
     
