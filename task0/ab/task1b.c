#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// pointers
//FILE *input[2], *output;
//int n=0, t=0, diff=0, k=-1;

typedef struct diff {
    long offset;                /* offset of the difference in file starting from zero*/
    unsigned char orig_value;   /* value of the byte in ORIG */
    unsigned char new_value;    /* value of the byte in NEW */
} diff;

typedef struct node node;

struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};

/* Print the nodes in diff_list in the following format:
 * byte POSITION ORIG_VALUE NEW_VALUE
 * Each item followed by a newline character. */
void list_print(node *diff_list, FILE* output){
    if(diff_list->diff_data != NULL){
        fprintf(output, "byte %ld %02X %02X\n",
                diff_list->diff_data->offset,
                diff_list->diff_data->orig_value,
                diff_list->diff_data->new_value);
        if(diff_list->next != NULL)
            list_print(diff_list->next, output);
    }
}

void list_print_n(node *diff_list, FILE* output, int i) {
    if(i != 0) {
        if (diff_list->diff_data != NULL) {
            fprintf(output, "byte %ld %02X %02X\n",
                    diff_list->diff_data->offset,
                    diff_list->diff_data->orig_value,
                    diff_list->diff_data->new_value);
            if (diff_list->next != NULL)
                list_print_n(diff_list->next, output, i-1);
        }
    }
}

void restoreByte(node* list, FILE* new) {
    if(list != NULL) {
        if(list->diff_data != NULL) {
            fseek(new, list->diff_data->offset, SEEK_SET);
            fputc(list->diff_data->orig_value, new);
        }
    }

    if(list->next != NULL) {
        restoreByte(list->next, new);
    }
}

/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/
node* list_append(node* diff_list, diff* data){
    node *new = (node*)malloc(sizeof(node));
    if(data!=NULL) {
        new->diff_data = data;
        new->next = diff_list;
        return new;
    }
    return new;
}

int get_list_size(node* list) {
    int i=0;
    node* curr_list = list;
    while (curr_list != NULL) {
        curr_list = list->next;
        i++;
    }
    return i;
}

/* Free the memory allocated by and for the list. */
void list_free(node *diff_list) {
    node* current = diff_list;
    node* temp;
    while(current!=NULL)
    {
        temp = current;
        current = current->next;
        free(temp->diff_data);
        free(temp);
    }
}

// pointers
FILE *input[2], *output;
int n=0, t=0, d=0, k=0, r=0;

int main(int argc, char *argv[]) {
    // setting pointers to Default settings
    output = stdout;

    // reads program args and looks for flags
    for(int f=0, i=1; i<argc; i++) {
        if (argv[i][0] != '-') { // if '-' at the beginning go to next arg
            input[f] = fopen(argv[i], "rb+");
            f++;
            continue;
        }
        switch (argv[i][1]) {
            case 't':   //-t : Print total number of differences
                t = 1;
                break;
            case 'k':   //-k <n> : Print only first n differences
                k = atoi(argv[i+1]);
                i++;
                break;
            case 'r':   //-k <n> : Print only first n differences
//                k = atoi(argv[i+1]);
//                i++;
                r=1;
                break;
            case 'o':   //-o <output file>: Save result to an output file
                output = fopen(argv[i+1], "w+");
                i++;
                break;
        }
    }
    if(input[0]==NULL || input[1]==NULL) {
        printf("you have to give 2 files in args!\n");
        return 0;
    }

    node *list = (node *) malloc(sizeof(node));

    char a, b;
    while((a = getc(input[0])) != EOF) {
        b = getc(input[1]);

        if(b==EOF)
            printf("input NEW is shorter then ORIG\n");
        if(a!=b) {
            diff *newDiff = (diff *) malloc(sizeof(diff));
            newDiff->offset = n;
            newDiff->orig_value = a;
            newDiff->new_value = b;
            list = list_append(list, newDiff);
            d++;
        }
        n++;
    }

    if(k>0) {
        list_print_n(list, output, k);
    } else if(t==1) {
        fprintf(output, "%d\n", d);
    } else if(r==1) {
        restoreByte(list, input[1]);
    } else {
        list_print(list, output);
    }

    list_free(list);
    fclose(input[0]);
    fclose(input[1]);
    fclose(output);
    return 0;
}
