#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char opcode[3]; // to store opcode after search
int searchOptab(char *opcode1);

int main()
{
    FILE *fptr1 = fopen("input.txt", "r");
    FILE *fptr3 = fopen("intermediate.txt", "w");
    FILE *fptr4 = fopen("symtab.txt", "w");

    int locctr, start, length;
    char symbol[10], opcode1[10], operand[10];
    fscanf(fptr1, "%s %s %s", symbol, opcode1, operand);
    if (strcmp("START", opcode1) == 0)
    {
        locctr = (int)strtol(operand, NULL, 16);
        fprintf(fptr3, "\t%s\t%s\t%s", symbol, opcode1, operand);
    }
    else
    {
        locctr = 0;
    }
    while (feof(fptr1) == 0)
    {
        fscanf(fptr1, "%s %s %s", symbol, opcode1, operand);
        printf("\n%s\t%s\t%s", symbol, opcode1, operand);
        fprintf(fptr3, "\n%X\t%s\t%s\t%s", locctr, symbol, opcode1, operand);
        if (strcmp("****", symbol) == 0)
        {
            // if not symbol definition
            int offset = searchOptab(opcode1);
            if (offset > -1)
            {
                locctr += 3;
            }
            else
            {
                printf("\nSkipped %s ... NOT found in OPTAB\n", opcode1);
            }
        }
        else
        {
            // symbol definition
            if (strcmp("BYTE", opcode1) == 0)
            {
                fprintf(fptr4, "%s\t%X", symbol, locctr);
                locctr++;
            }
            else if (strcmp("WORD", opcode1) == 0)
            {
                fprintf(fptr4, "%s\t%X", symbol, locctr);
                locctr += 3;
            }
            else if (strcmp("RESB", opcode1) == 0)
            {
                fprintf(fptr4, "%s\t%X", symbol, locctr);
                locctr += atoi(operand);
            }
            else if (strcmp("RESW", opcode1) == 0)
            {
                fprintf(fptr4, "%s\t%X", symbol, locctr);
                locctr += 3 * atoi(operand);
            }
            else
            {
                printf("\n%s not defined....\n", symbol);
            }
        }
    }
    fclose(fptr1);
    fclose(fptr3);
    fclose(fptr4);
    return 0;
}

int searchOptab(char *opcode1)
{
    char mnemonic[5];
    FILE *fptr2 = fopen("optab.txt", "r");
    while (fscanf(fptr2, "%s\t%s", mnemonic, opcode) != EOF)
    {
        if (strcmp(mnemonic, opcode1) == 0)
        {
            return (int)ftell(fptr2);
        }
    }
    fclose(fptr2);
    return -1;
}