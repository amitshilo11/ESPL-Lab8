#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pointers
FILE *input[2], *output;
int n=0, t=0, diff=0, k=-1;

int main(int argc, char *argv[]) {
    // setting pointers to Default settings
    output = stdout;

    // reads program args and looks for flags
    for(int f=0, i=1; i<argc; i++) {
        if (argv[i][0] != '-') { // if '-' at the beginning go to next arg
        input[f] = fopen(argv[i], "rb");
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

    char a, b;
    while((a = getc(input[0])) != EOF) {
        b = getc(input[1]);
        if(b==EOF)
            printf("input NEW is shorter then ORIG\n");
        if(a!=b){
            if(t==0)
                fprintf(output, "byte: %d %02X %02X\n", n, (unsigned char)a, (unsigned char)b);
            diff++;
        }
        n++;
        if((diff-k)==0)
            break;
    }
    if(t==1)
        fprintf(output, "%d\n", diff);

    fclose(input[0]);
    fclose(input[1]);
    fclose(output);
    return 0;
}