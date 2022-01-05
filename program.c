#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define NODE_POOL_CAP 4000000
#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

typedef struct Node Node;

struct Node{
  Node *children[256];
};

static Node node_pool[NODE_POOL_CAP] = {0};
size_t node_pool_count = 0;

Node* alloc_node(void){
  if (node_pool_count > NODE_POOL_CAP){
    printf("[ERROR]Node capacity has been maxed out\n");
    exit(0);
  }
  return &node_pool[node_pool_count++];
}

void insert_text(Node* root, const char *text){
  if(text == NULL || *text == '\0' || *text == '\n'){
    return;
  }
  
  if(root == NULL){ 
    printf("[ERROR]Root node is NULL.\n");
    exit(0);
  }

  size_t index = (size_t) *text;

  if(root->children[index] == NULL){
    root->children[index] = alloc_node();
  }

  insert_text(root->children[index], text + 1);
}


FILE* create_output_file(void){
  FILE *f;
  f = fopen("output.dot", "w");
  if (f == NULL){
    printf("[ERROR]Could not create output file!\n");
    exit(1);
  }
  else{
    printf("[INFO]Created output file!\n");
  }
  return f;
}

int list_length(char* list[]){
  int length = 0;
  while(list[length]){
    length++;
  }
  return length;
}

void sort_list(char* list[]){
  int length = list_length(list);
  char temp[30];
  for(int i=0; i < length; i++){
    for(int j=0; j < length-1-i; j++){
      if(strcmp(list[j], list[j+1]) > 0){
        strcpy(temp, list[j]);
        strcpy(list[j], list[j+1]);
        strcpy(list[j+1], temp);
      }
    }  
  }
  printf("[INFO]List was successfully sorted!\n");
  return;
}

void display_list(char* list[]){
  int length = list_length(list);
  for(int i = 0; i < length; i++){
    printf("%i: %s", i, list[i]);
  }
  return;
}
  
FILE* open_file(char file_name[]){
  FILE *f;
  f = fopen(file_name, "r");
  if (f == NULL){
    printf("[ERROR]File failed to Open, file: %s\n", file_name);
    exit(1);
  }
  else{
    printf("[INFO]Opening file...\n");
    return f;
  }
}

char** allocate_memory(FILE *f){
  char** list;

  size_t pos = ftell(f);  
  fseek(f, 0, SEEK_END);
  
  size_t length = ftell(f); 
  fseek(f, pos, SEEK_SET);   
  
  list = malloc(length * sizeof(char) * 15 );
  if (list == NULL ){
    printf("[ERROR]Memmory could not be allocated, size of memory: %li\n", length * sizeof(char) * 15);
    exit(1);
  }
  else{
    printf("[INFO]Memory was allocated successfully, size of memory: %li\n", length * sizeof(char) * 15);
    return list;
  }
}


char** create_list(char file_name[])
{
  FILE *f;
  f = open_file(file_name);

  char** list;
  list = allocate_memory(f);
  
  char line[100];
  int i = 0;
  printf("[INFO]List creation has started...\n");
  while(fgets(line,sizeof(line),f)){
    list[i] = malloc(30);
    strcpy(list[i],line);
    i++;
  }
  printf("[INFO]List creation has finished...\n");
  fclose(f);
  return list;
}

Node* create_nodes(char* list[]){
  Node* root = alloc_node();
  int length = list_length(list);
  printf("[INFO]Creating all nodes based on list...\n");
  for(int i = 0; i < length; i++){
    insert_text(root, list[i]);
  }
  return root;
}

void dump_dot(Node *root, FILE *f){
  size_t index = root - node_pool;
  for(size_t i = 0; i < ARRAY_LEN(root->children); i++){
    if(root->children[i] != NULL) {
      size_t child_index = root->children[i] - node_pool;
      fprintf(f,"Node_%zu -> Node_%zu [label=%c]\n", index, child_index, (char) i);
      dump_dot(root->children[i], f);
    }
  }
}

void fill_dot(char* list[]){
  Node* root = create_nodes(list);
  FILE *f = create_output_file();
  printf("[INFO]Creating the .dot file with the graph...\n");
  fprintf(f, "digraph Tree {\n");
  dump_dot(root, f);
  fprintf(f, "}");
  printf("[INFO]Finished creating the .dot file with the graph...\n");
  printf("[SUCCESS]Exiting the program with code 0...\n");
  fclose(f);
}

int main(int argc, char* argv[]){
  printf("[STARTING]Starting the app and creation of the Graph...\n");
  if(argc != 2){
    printf("[ERROR]More than 1 accepted arguement passed...\n");
    return 1;
  }
  char* file_name = argv[1];
  char** list = create_list(file_name);
  sort_list(list);
  fill_dot(list);
  system("dot -Tsvg output.dot > output.svg");
  system("rm output.dot");
  return 0;
}
