#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
    FILE *inputFile, *optabFile, *symtabFile, *symtabTempFile, *outputFile;
    int locationCounter, startAddress, i = 0, j = 0, modificationRecords[10];
    int programLength, opcodeLength, tempIndex = 0;
    char programName[10], operand[10], label[10], mnemonic[10];
    char dummySymbol[10] = "*", startAddressStr[10], symbol[10], symbolAddress[10];
    char objCode1[10], objCode2[10], tempSymbol[10], tempOperand[10];

    // Open files for reading and writing
    inputFile = fopen("input.txt", "r");
    optabFile = fopen("optab.txt", "r");
    symtabFile = fopen("symtab.txt", "w+");
    symtabTempFile = fopen("symtab1.txt", "w+");
    outputFile = fopen("output.txt", "w+");

    // Read the first line to check if it contains START directive
    fscanf(inputFile, "%s %s %s", label, mnemonic, operand);
    if (strcmp(mnemonic, "START") == 0)
    {
        startAddress = atoi(operand); // Set start address
        strcpy(programName, label);   // Store program name
        locationCounter = startAddress;
    }

    // Move to the next line in the input file
    fscanf(inputFile, "%s %s %s", label, mnemonic, operand);

    // Process each line until the END directive
    while (strcmp(mnemonic, "END") != 0)
    {
        // If there is no label
        if (strcmp(label, "-") == 0)
        {
            // Look up the mnemonic in the operation table
            rewind(optabFile);

            while (fscanf(optabFile, "%s %s", tempSymbol, tempOperand) != EOF)
            {
                if (strcmp(tempSymbol, mnemonic) == 0)
                {
                    modificationRecords[i] = locationCounter + 1;
                    fprintf(symtabFile, "%s\t%s\n", operand, dummySymbol);
                    fprintf(outputFile, "%s\t0000\n", tempOperand);
                    locationCounter += 3; // Increment location counter by 3
                    i++;
                    break;
                }
            }
        }
        else
        {
            // Process the label and update symbol table
            rewind(symtabFile);
            fscanf(symtabFile, "%s %s", symbol, symbolAddress);
            while (!feof(symtabFile))
            {
                if (strcmp(symbol, label) == 0)
                {
                    sprintf(startAddressStr, "%d", locationCounter);
                    fprintf(symtabTempFile, "%s\t%s\n", label, startAddressStr);
                    sprintf(tempSymbol, "%d", modificationRecords[j]);
                    j++;
                    fprintf(outputFile, "%s\t%s\n", tempSymbol, startAddressStr);
                    i++;
                    break;
                }
                else
                {
                    fscanf(symtabFile, "%s %s", symbol, symbolAddress);
                }
            }

            // Process the different mnemonics
            if (strcmp(mnemonic, "RESW") == 0)
            {
                locationCounter += 3 * atoi(operand);
            }
            else if (strcmp(mnemonic, "BYTE") == 0)
            {
                strcpy(tempOperand, "-");
                opcodeLength = strlen(operand);
                locationCounter += opcodeLength - 2;
                for (int k = 2; k < opcodeLength; k++)
                {
                    tempSymbol[tempIndex++] = operand[k];
                }
                fprintf(outputFile, "%s\t%s\n", tempSymbol, tempOperand);
                break;
            }
            else if (strcmp(mnemonic, "RESB") == 0)
            {
                locationCounter += atoi(operand);
            }
            else if (strcmp(mnemonic, "WORD") == 0)
            {
                strcpy(tempOperand, "#");
                locationCounter += 3;
                fprintf(outputFile, "%s\t%s\n", operand, tempOperand);
                break;
            }
        }

        // Reset file pointer to beginning of the operation table
        rewind(optabFile);
        fscanf(inputFile, "%s %s %s", label, mnemonic, operand);
    }

    // Calculate and display program length
    programLength = locationCounter - startAddress;
    printf("H^%s^%d^0%x\n", programName, startAddress, programLength);
    printf("T^00%d^0%x", startAddress, programLength);

    // Print object code from output file
    rewind(outputFile);
    fscanf(outputFile, "%s %s", objCode1, objCode2);
    while (!feof(outputFile))
    {
        if (strcmp(objCode2, "0000") == 0)
        {
            printf("^%s%s", objCode1, objCode2);
        }
        else if (strcmp(objCode2, "-") == 0)
        {
            printf("^");
            opcodeLength = strlen(objCode1);
            for (int k = 0; k < opcodeLength; k++)
            {
                printf("%d", objCode1[k]);
            }
        }
        else if (strcmp(objCode2, "#") == 0)
        {
            printf("^%s", objCode1);
        }
        fscanf(outputFile, "%s %s", objCode1, objCode2);
    }

    // Print ending record
    printf("\nE^00%d\n", startAddress);

    // Close all files
    fclose(inputFile);
    fclose(optabFile);
    fclose(symtabFile);
    fclose(symtabTempFile);
    fclose(outputFile);
}
