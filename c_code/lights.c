/*
Name:       Ellis Nielsen
ID Num:     1001615470
UTA:        EXN5470
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#define buff 20


typedef struct Node{
    char *color;
    int **details;
    struct Node* next;
}Node;

/*This function creates the nodes for the linked and assigns values to them.
It takes a pointer to a list, then it takes the color, and then in takes a pointer
to the values in details*/
Node *createNode(Node *list, char *color, int *values){
    Node *new_node;
    new_node = malloc(sizeof(Node));
    if(new_node == NULL){
        printf("MALLOC FAILED TO ALLOCATE IN CREATENODE\n");
        exit(EXIT_FAILURE);
    }
    new_node->color = color;
    //strcpy(new_node->color,color);
    new_node->details = (int**)calloc(2,sizeof(int));
    (new_node->details)[0] = (int*)values[0];
    (new_node->details)[1] = (int*)values[1];
    new_node->next = list;
    return new_node;
}

/*This function takes a line of info and strtok it to pull the info out
it then assigns the info and then pass the info to createNode*/
Node *getinfo(char *line, Node *list){
    char *color;
    color = calloc(10, sizeof(char));
    int *value;
    value = calloc(2, sizeof(int));
    char *token;
    token = strtok(line,",");
    strcpy(color, token);
    int i = 0;
    while(i<2){
        token = strtok(NULL, ",");
        *value = atoi(token);
        value++;
        i++;
    }
    value-=2;

    return createNode(list, color, value);
}

/*This function takes the filename and creates a linked list of the string of lights.
It returns the head of the linked list*/
Node *light_info(char *filename){
    Node *light = NULL;
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("File failed to open\n");
        exit(EXIT_FAILURE);
    }
    char line[buff];
    while(!feof(fp)&&strcmp(line,"\n")!=0){
        fgets(line, buff, fp);
        light = getinfo(line, light);
    }
    return light;
}

/*This function prints out the lights. It takes a int to either turn off the
lights or turn them on. Takes a list to go through it*/
void printLights(int order, Node *list){
    Node *p;
    for(p = list;p!=NULL;p = p->next){
        char colors;
        if(order == 0){
            colors = 95;
        }else{
            colors = (p->color)[0];
        }
        if((p->details)[1]>0){
            colors = toupper(colors);
        }
        int i = 0;
        int value;
        value = (int)(p->details)[0];
        while(i<value){
            printf("%c", colors);
            i++;
        }
        printf(" ");
        }
    printf("\n");
}

/*This function determines what the light status is then calls the print
function with the correct argument to print lights. The function takes a 
status value, 1 is on, 0 is off. Num is what the user wants the lights to
do*/
int checklights(int status, int num, Node *list){
    if(status == 0 && num == status){
        printf("--Lights are already off.\n");
    }else if(status == 0 && num == 1){
        printf("**Turning on lights:\n");
        printLights(1, list);
        status = 1;
    }else if(status == 1 && num == status){
        printf("--Lights are already on.\n");
    }else if(status == 1 && num == 0){
        printf("***Turning lights off:\n");
        printLights(0,list);
        status = 0;
    }
    return status;
}

/*This function free all the memory for the linked list.
takes a list and free's it*/
void closeprogram(Node *list){
    Node *delete;
    while(list != NULL){
        delete = list;
        list= list->next;
        free(delete);
    }
}

int main(int argc, char **argv){
    Node *first;
    first = light_info((argv[1]));
    int loop = 1;
    char ans[7];
    int light = 0;
    while(loop){
        scanf("%s", ans);
        if(strcmp(ans, "off")==0){ 
            light = checklights(light, 0, first);
        }    
        if(strcmp(ans, "on")==0){
            light = checklights(light, 1, first);
        }    
        if(strcmp(ans, "exit")==0){
            closeprogram(first); 
            printf("Exiting..");
            exit(EXIT_SUCCESS);
        } 
    
    }
}