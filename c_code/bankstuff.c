/*
Name:       Ellis Nielsen
ID Num:     1001615470
UTA:        EXN5470
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buff 255


typedef struct account{
    char **user_name;           /*first name, last name*/
    char *bank_name;            /*bank name*/
    int num_accounts;           /*number of accounts*/
    int **account_numbers;      /*all 6 digits, can have mutiple accounts-checking, saving*/
    float **total_in_account;   /*amount in each account*/
    struct account *next;
}Account;

/*This function checks to see if a pointer is null, and then exits if 
the pointer was NULL, as we can't have NULL pointers. It takes in a pointer
of any type.*/
void checkIFNULL(void *pointer){
    if(pointer == NULL){
        printf("pointer failure, returned NULL\n");
        exit(EXIT_FAILURE);
    }
}

/*This function takes all the import info and places them in the node. This 
function also has the task of creating the linked list nodes*/
Account *createAccount(char *first, char *last, char *bankname, int numAcc, int *accountNum, float *total, Account *node){
    Account *new_node;
    new_node = malloc(sizeof(Account));
    if(new_node == NULL){
        printf("Failure to malloc new_node in createAccount\n");
        exit(EXIT_FAILURE);
    }

    new_node->user_name = malloc(1*sizeof(char*));
    checkIFNULL(new_node->user_name);
    new_node->user_name[0]=first;
    new_node->user_name[1]=last;

    new_node->bank_name = bankname;
    new_node->num_accounts = numAcc;


    new_node->total_in_account = calloc(1,sizeof(float*));
    checkIFNULL(new_node->total_in_account);
    new_node->account_numbers = calloc(1,sizeof(int*));
    checkIFNULL(new_node->account_numbers);

    new_node->account_numbers[0] = accountNum;
    new_node->account_numbers[1] = accountNum+1;
    new_node->total_in_account[0] = total;
    new_node->total_in_account[1] = total+1;
    new_node->next = node;

    return new_node;

}

Account *parseCreate(char *line, Account *first){
    char *firstname, *lastname, *bankname, *token;
    int numAcc, *accountNum, tempINT;
    float *total, temp;
    
    token = strtok(line, " ");
    firstname = malloc(sizeof(char) * 20);
    checkIFNULL(firstname);
    memcpy(firstname, token, strlen(token)+1);

    token = strtok(NULL, ",");
    lastname = malloc(sizeof(char)*20);
    checkIFNULL(lastname);
    memcpy(lastname, token, strlen(token)+1);

    token = strtok(NULL, ",");
    bankname = malloc(strlen(token)+1);
    checkIFNULL(bankname);
    memcpy(bankname, token, strlen(token)+1);

    token = strtok(NULL, ",");
    numAcc = atoi(token);

    token = strtok(NULL, ",");
    accountNum = calloc(2,sizeof(int));
    checkIFNULL(accountNum);
    tempINT = atoi(token);
    memcpy(accountNum, &tempINT, sizeof(int));

    if(numAcc == 2){
        token = strtok(NULL, ",");
        tempINT = atoi(token);
        memcpy((accountNum+1), &tempINT, sizeof(int));
    }else{
        tempINT = 0;
        memcpy((accountNum+1), &tempINT, sizeof(int));
    }

    token = strtok(NULL, ",");
    total = calloc(2,sizeof(float));
    checkIFNULL(total);
    temp = atof(token);
    memcpy(total, &temp, sizeof(float));

    if(numAcc == 2){
        token = strtok(NULL, ",");
        temp = atof(token);
        memcpy((total+1), &temp, sizeof(float));
    }else{
        temp = 0.00;
        memcpy((total+1), &temp, sizeof(int));
    }
    first = createAccount(firstname, lastname, bankname, numAcc, accountNum, total , first);
    // free(firstname); 
    // free(lastname); 
    // free(bankname);
    // free(total);
    // free(accountNum);
    return first;
}


/*Gets a filename and opens the file, it then pass on the info to a parse
to pull the info out of the line*/
Account *read_file(int size, char *filename){
    Account *first = NULL;
    FILE *fp;
    fp = fopen(filename, "r+");
    if(fp == NULL){
        printf("Failure to found file in read_file\n");
        exit(EXIT_FAILURE);
    }
    char line[buff];
    int i =0;
    while(!feof(fp)&&i<size){
        fgets(line,buff,fp);
        first = parseCreate(line, first);
        i++;
    }
    fclose(fp);
    return first;
}

/*This function finds the account of the user and returns the struct.
It takes list in and a pointer to find the names. Returns the node if the names
match*/
Account *findAccount(Account *list, char *name){
    char *token;
    token = strtok(name, " ");
    Account *temp;
    for(temp = list; temp!=NULL; temp=temp->next){
        if(strcmp(token , temp->user_name[0])==0){
            return temp;
        }
    }
    if(temp->next == NULL){
        printf("That name doesn't exist");
        return NULL;
    }
    return temp;
}

/*This function finds of if the banks are the same
takes two accounts and compares return 1 if they match, returns 0
if they don't*/
int sameBANK(Account *sender, Account *receiver){
    if(strcmp(sender->bank_name, receiver->bank_name)==0){
        return 1;
    }
    return 0;
}

/*this function checks to see if the bank account has enough money
it takes a account a checking or saving number, and a amt to check*/
int checkMoney(Account *person, int accountn, float amt){
    if(person->total_in_account[accountn][0]-amt>5){
        return 1;
    }
    return 0;
}
/*this function calculates the money and fees for the transaction.
returns 0 if error else returns 1. Takes a list and two names*/
int Transaction(Account *list, char *name, char *money){
    Account *person = findAccount(list, name);
    Account *moneysend = findAccount(list, money);
    if(person == NULL || money == NULL){
        return 0;
    }
    int bankmine=0;
    bankmine = sameBANK(person, moneysend);
    float fee;

    if(bankmine == 0){
        char ans[3];
        printf("Recipient is in a different bank. You will be charged a \%5 transaction fee. Continue? y or n: ");
        scanf("%s", ans);
        if(strcmp(ans, "n")==0){
            printf("okay. Ending session....");
            return 0;
        }
        fee = 1.05;
    }else{
        fee = 1.00;
    }

    float amt;
    printf("How much money would you like to transfer? $");
    scanf("%f", &amt);
    int accountn;
    if(person->num_accounts==2){
        char accountsname[20];
        printf("From which account?");
        scanf("%s", accountsname);
        if(strcmp(accountsname, "savings")==0){
            accountn = 1;
        }
    }else{
        accountn = 0;
    }
    int goodmoney = checkMoney(person, accountn, amt);

    if(goodmoney==0){
        printf("Sorry, you do not have sufficient funds to make this transfer. Ending session");
        return 0;
    }

    //transfer money
    printf("Before transfer: $%.2f\n",person->total_in_account[accountn][0]);
    person->total_in_account[accountn][0]=person->total_in_account[accountn][0]-(amt*fee);
    printf("After transfer: $%.2f\n",person->total_in_account[accountn][0]);
    moneysend->total_in_account[0][0]+=amt*fee;

    printf("Transfer successfully completed.\n");
    return 1;

}
/*This function clears all the memory that has been used
takes a list as a parameter, returns nothing*/
void endProgram(Account *list){
    Account *temp;

    for(temp = list, list = list->next;list!=NULL ; temp = list, list = list->next){
        free(temp->user_name[1]);
        free(temp->user_name[0]);
        free(temp->bank_name);
        free(temp->account_numbers[1]);
        free(temp->account_numbers[0]);
        free(temp->total_in_account[1]);
        free(temp->total_in_account[0]);
        free(temp);
    }
}

/*This function prints out to a file the contents of the list when 
the user want to end the program takes a char pointer and number of users
returns nothing */
void printFile(char *filename, Account *list){
    FILE *fp;
    fp = fopen(filename, "w+");

    while(list!=NULL){
        if(list->num_accounts==1){
            fprintf(fp,"%s %s,%s,%d,%d,%.0f\n",list->user_name[0],list->user_name[1],list->bank_name,list->num_accounts,list->account_numbers[0][0],list->total_in_account[0][0]);
        }else{
            fprintf(fp,"%s %s,%s,%d,%d,%d,%.0f,%.0f\n",list->user_name[0],list->user_name[1],list->bank_name,list->num_accounts,list->account_numbers[0][0],list->account_numbers[1][0],list->total_in_account[0][0],list->total_in_account[1][0]);
        }
        list = list->next;
    }
}

/*This functions does all other tasks such as, getting users inputs and then calling the correct command
this function can end the program if the user tpyes exit*/
int start(Account *list, char *filename){
    printf("******************************************************************\n");
    printf("***Welcome to Money Transfer-Voted the Best 10 Years in a Row!***\n");
    printf("******************************************************************\n");
    char name[50]; 
    char money[50];
    printf("Enter first and last name: ");
    gets(name);

    //end program
    if(strcmp(name, "end program")==0){
        printFile(filename, list);
        endProgram(list);
        printf("exiting...");
        exit(EXIT_SUCCESS);
    }

    printf("who do you want to send money to? ");
    gets(money);                //get name of other
    
    int num;
    num = Transaction(list, name, money);
    if(num==0){
        return 1;
    }
    return 1;
    

}


int main(int argc, char **argv){
    Account *first;
    first = read_file(atoi(argv[1]), argv[2]);
    int run = 1;
    while(run){
        run = start(first,argv[2]);
    }
    return 0;
    
}