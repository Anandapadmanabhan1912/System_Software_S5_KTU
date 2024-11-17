#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char mask[3];
char bitexp[14]; // Ensure the array size is sufficient

void bit_expansion(char *mask)
{
    int len = strlen(mask);
    bitexp[0] = '\0'; // Initialize bitexp to an empty string

    for (int i = 0; i < len; i++)
    {
        switch (mask[i])
        {
        case '0':
            strcat(bitexp, "0000");
            break;
        case '1':
            strcat(bitexp, "0001");
            break;
        case '2':
            strcat(bitexp, "0010");
            break;
        case '3':
            strcat(bitexp, "0011");
            break;
        case '4':
            strcat(bitexp, "0100");
            break;
        case '5':
            strcat(bitexp, "0101");
            break;
        case '6':
            strcat(bitexp, "0110");
            break;
        case '7':
            strcat(bitexp, "0111");
            break;
        case '8':
            strcat(bitexp, "1000");
            break;
        case '9':
            strcat(bitexp, "1001");
            break;
        case 'A':
            strcat(bitexp, "1010");
            break;
        case 'B':
            strcat(bitexp, "1011");
            break;
        case 'C':
            strcat(bitexp, "1100");
            break;
        case 'D':
            strcat(bitexp, "1101");
            break;
        case 'E':
            strcat(bitexp, "1110");
            break;
        case 'F':
            strcat(bitexp, "1111");
            break;
        default:
            break;
        }
    }

    printf("bitexp: %s\n", bitexp); // Debugging bitexp content
}

int main()
{
    int straddr, len;
    char rec[10];
    char prgname[15];
    int loadaddr, addr;
    int locctr;
    char first[3];
    char second[5];
    int bit_modif = 0;
    FILE *fptr = fopen("program.txt", "r");

    if (!fptr)
    {
        printf("Failed to open program.txt\n");
        return 1;
    }

    fscanf(fptr, "%s %s %X %X", rec, prgname, &straddr, &len);
    printf("\nEnter the load address: ");
    scanf("%X", &loadaddr);
    locctr = loadaddr;

    if (strcmp(rec, "H") == 0)
    {
        printf("\nPROGRAM = %s\n", prgname);
        printf("\nAddress\t  ObjectCode\n");
        printf("_______________________\n");
    }
    else
    {
        printf("\nError in Object Code... No header\n");
    }

    fscanf(fptr, "%s", rec);

    while (strcmp("E", rec) != 0)
    {
        if (strcmp("T", rec) == 0)
        {
            fscanf(fptr, "%X %X %s", &straddr, &len, mask);
            bit_expansion(mask);
            bit_modif = 0;
            fscanf(fptr, "%s", rec);
        }
        else if (strlen(rec) == 6)
        {
            strncpy(first, rec, 2);
            strcat(first, "\0");
            strncpy(second, rec + 2, 4);
            strcat(second, "\0");

            if (bitexp[bit_modif] == '1')
            {
                addr = strtol(second, NULL, 16) + loadaddr;
                printf("\n%X\t\t%s%X", locctr, first, addr);
            }
            else
            {
                strncpy(second, second, 4);
                printf("\n%X\t\t%s%s", locctr, first, second);
            }
            locctr += 3;
            fscanf(fptr, "%s", rec);
            bit_modif++;
        }
    }
    fclose(fptr);
    return 0;
}
