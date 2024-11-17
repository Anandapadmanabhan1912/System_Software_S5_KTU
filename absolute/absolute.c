#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main()
{
    FILE *filePointer;
    int i, startAddressInt, lineIndex, programStartAddressInt;
    char programName[10], line[50], objectProgramName[10], tempAddress[10];
    char recordType, startAddressStr[10];

    // Prompt user to enter the program name
    printf("Enter program name: ");
    scanf("%s", programName);

    // Open the input file to read
    filePointer = fopen("input.txt", "r");
    if (filePointer == NULL)
    {
        printf("Error: Unable to open the input file.\n");
        exit(1);
    }

    // Read the first line from the file, which should contain the program name
    fscanf(filePointer, "%s", line);

    // Extract the program name from the object file line (characters 2-7)
    for (i = 2, lineIndex = 0; i < 8; i++, lineIndex++)
    {
        objectProgramName[lineIndex] = line[i];
    }
    objectProgramName[lineIndex] = '\0'; // Null-terminate the extracted name

    printf("Program name from object file: %s\n", objectProgramName);

    // Check if the entered program name matches the name in the object file
    if (strcmp(programName, objectProgramName) == 0)
    {
        do
        {
            fscanf(filePointer, "%s", line); // Read each line in the file
            recordType = line[0];            // Identify the record type (e.g., 'T' or 'E')

            if (recordType == 'T')
            {
                // Extract the starting address from the text record
                for (i = 2, lineIndex = 0; i < 8; i++, lineIndex++)
                {
                    startAddressStr[lineIndex] = line[i];
                }
                startAddressStr[lineIndex] = '\0';       // Null-terminate the address string
                startAddressInt = atoi(startAddressStr); // Convert to integer

                // Parse and display object code data from text record
                i = 12; // Start after the address and length fields
                while (line[i] != '$')
                { // '$' denotes end of record data
                    if (line[i] != '^')
                    { // Skip delimiter
                        printf("Address: %04d \t Code: %c%c\n", startAddressInt, line[i], line[i + 1]);
                        startAddressInt++; // Increment address for next code segment
                        i += 2;            // Move to the next byte of object code
                    }
                    else
                    {
                        i++; // Skip delimiter
                    }
                }
            }
            else if (recordType == 'E')
            { // End of program record
                fclose(filePointer);
                exit(0);
            }
        } while (!feof(filePointer)); // Continue until end of file
    }
    else
    {
        printf("Error: Program name does not match.\n");
    }

    fclose(filePointer); // Close the file
}
