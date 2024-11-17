#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int searchOptab(char *opcode);
int searchSymtab(char *opcode);
char mnemonic[8];
char symequi[8];

int main()
{
    FILE *fptr1 = fopen("intermediate.txt", "r");
    FILE *fptr4 = fopen("output.txt", "w");
    FILE *fptr5 = fopen("listing.txt", "w");

    char symbol[10], opcode[10], operand[10];
    long offset_len;
    long off_rec;
    int reccount = 0;
    int locctr;
    char start[6];
    int str;
    fscanf(fptr1, "%s %s %s", symbol, opcode, operand);
    if (strcmp("START", opcode) == 0)
    {
        fprintf(fptr4, "H %s %s ****\n", symbol, operand);
        printf("H %s %s **\n", symbol, operand);
        offset_len = (int)ftell(fptr4) - 6;
        strcpy(start, operand);
        locctr = atoi(operand);
        str = locctr;
    }
    else
    {
        locctr = 0;
        str = locctr;
    }
    // start new text record
    fprintf(fptr4, "T ** ");
    printf("T ** ");
    off_rec = (int)ftell(fptr4) - 3;
    while (fscanf(fptr1, "%X %s %s %s", &locctr, symbol, opcode, operand) != EOF)
    {
        if (strcmp(symbol, "****") == 0)
        {
            // instructions
            int flag = searchOptab(opcode);
            if (flag > -1)
            {
                // found
                searchSymtab(symbol);
                fprintf(fptr4, "%s%s ", mnemonic, symequi);
                printf("%s%s", mnemonic, symequi);
                reccount++;
            }
            else
            {
                // opcode not found
                printf("\nNot Valid OPCODE: %s\n", opcode);
            }
        }
        else
        {
            // symbol definitions
            printf("\nSYMBOL: %s stored...", symbol);
        }
    }
    fseek(fptr4, off_rec, SEEK_SET);
    fprintf(fptr4, "%X", reccount * 6);
    fseek(fptr4, offset_len, SEEK_SET);
    fprintf(fptr4, "00%X", (locctr - str));
    fseek(fptr4, 0, SEEK_END);
    fprintf(fptr4, "\nE %s", start);
    fclose(fptr1);
    fclose(fptr4);
    fclose(fptr5);
    return 0;
}

int searchOptab(char *opcode)
{
    FILE *fptr2 = fopen("optab.txt", "r");
    char opnew[10];
    while (fscanf(fptr2, "%s %s", opnew, mnemonic) != EOF)
    {
        if (strcmp(opnew, opcode) == 0)
        {
            return atoi(mnemonic);
        }
    }
    fclose(fptr2);
    return -1;
}
int searchSymtab(char *opcode)
{
    FILE *fptr2 = fopen("symtab.txt", "r");
    char symnew[10];
    while (fscanf(fptr2, "%s %s", symnew, symequi) != EOF)
    {
        if (strcmp(symnew, opcode) == 0)
        {
            return atoi(symequi);
        }
    }
    fclose(fptr2);
    return -1;
}