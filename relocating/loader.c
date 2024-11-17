#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char bit[30];
char bitmask[20];

// Function to convert hexadecimal bitmask to binary
void bitmask_convert(char mask[])
{
    int len = strlen(mask);
    strcpy(bit, "");

    for (int i = 0; i < len; ++i)
    {
        switch (mask[i])
        {
        case '0':
            strcat(bit, "0000");
            break;
        case '1':
            strcat(bit, "0001");
            break;
        case '2':
            strcat(bit, "0010");
            break;
        case '3':
            strcat(bit, "0011");
            break;
        case '4':
            strcat(bit, "0100");
            break;
        case '5':
            strcat(bit, "0101");
            break;
        case '6':
            strcat(bit, "0110");
            break;
        case '7':
            strcat(bit, "0111");
            break;
        case '8':
            strcat(bit, "1000");
            break;
        case '9':
            strcat(bit, "1001");
            break;
        case 'A':
            strcat(bit, "1010");
            break;
        case 'B':
            strcat(bit, "1011");
            break;
        case 'C':
            strcat(bit, "1100");
            break;
        case 'D':
            strcat(bit, "1101");
            break;
        case 'E':
            strcat(bit, "1110");
            break;
        case 'F':
            strcat(bit, "1111");
            break;
        default:
            break;
        }
    }
}

void main()
{
    FILE *objptr;
    int start, addr, add, len;
    int modif_obj_code;
    int bitmask_index = 0;

    char rec[20], name[20], first[3], second[5];

    printf("ENTER THE STARTING ADDRESS OF THE PROGRAM: ");
    scanf("%X", &start);
    addr = start;

    // Open the object code file
    objptr = fopen("program.txt", "r");

    // Read header record
    fscanf(objptr, "%s", rec);
    if (strcmp(rec, "H") == 0)
    {
        fscanf(objptr, "%s %X %X", name, &add, &len);
        printf("\nPROGRAM NAME = %s\n", name);
        printf("\nADDRESS   OBJECT CODE\n");
        printf("____________________________\n");
    }
    else
    {
        printf("INVALID OBJECT CODE FORMAT\n");
        fclose(objptr);
        exit(1);
    }

    strcpy(rec, "");
    fscanf(objptr, "%s", rec);

    // Process each record until the end record
    while (strcmp(rec, "E") != 0)
    {
        if (strcmp(rec, "T") == 0)
        {
            fscanf(objptr, "%X %X %s", &add, &len, bitmask);
            add += start;
            bitmask_index = 0;
            bitmask_convert(bitmask);
            fscanf(objptr, "%s", rec); // read the first object code in the record
        }

        // Process the object code with relocation if needed
        if (bit[bitmask_index] == '1')
        {
            // Split the record into two parts for modification
            strncpy(first, rec, 2);
            first[2] = '\0';

            strncpy(second, rec + 2, 4);
            second[4] = '\0';

            // Convert second part to integer, apply relocation
            modif_obj_code = strtol(second, NULL, 16) + start;
            printf("%X\t%s%X\n", add, first, modif_obj_code);
                }
        else
        {
            printf("%X\t%s\n", add, rec);
        }

        add += 3;
        bitmask_index++;
        fscanf(objptr, "%s", rec);
    }

    fclose(objptr);
}
