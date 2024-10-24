#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // För getopt()
#include <time.h>
#define BUFFER 256
//#include <bits/getopt_core.h>           //for linux compilation 
// Funktionsprototyper (deklarationer) för de funktioner som definieras i FIFO.c
int pageAlgo(int decimal_value, int address, int *frameArray, int num_frames, char *algo, int *usageOrder);

int main(int argc, char *argv[]){

    char *algorithm = NULL;                              // Sträng för att lagra den valda algoritmen
    int num_frames = 0;                                 // Integer som lagrar det valda antalet frames
    char *filename = NULL;                               // Sträng som lagrar den valda filen
    int opt;                                             // En integer som håller värdet som getopt() returnerar, getopt() returnerar dem angivna tecknen a:n:f: som ett heltal om den stöter på dem (-a -b -f)

    char *line;
    char *pageNR = (char*) malloc(BUFFER * sizeof(char));
    int lineCount = 0;
    int *binPageNR = malloc(BUFFER * sizeof(int));
    int memAccess = 0;
    int pageHit = 0;
    
    while ((opt = getopt(argc, argv, "a:n:f:")) != -1) { // Startar en loop som går igenom alla kommandoradsalternativ, när det inte finns fler alternativ returnerar getopt() -1
        switch(opt){                                     // Beroende på vilket alternativ som hittats av getopt()
            case 'a':                                    // Hantera '-a' flaggan
                algorithm = optarg;                      // optarg pekar på argumentet efter det hittade teckner, i detta fall det som kommer efter -a
                break;
            case 'n':
                num_frames = atoi(optarg);               // Konvertera argumentet efter -n till en integer och spara i num_frames
                break;
            case 'f':
                filename = optarg;
                break;
            default:
            fprintf(stderr, "Användning: %s -a <fifo|optimal|lru> -n <antal ramar> -f <fil>\n", argv[0]);  // Om ett ogiltigt alternativ upptäcks, skriv ut användningsmeddelande och avsluta programmet med fel
            exit(EXIT_FAILURE);
        }
    }

    int *frameArray = malloc(num_frames * sizeof(int));
    int *usageOrder = malloc(num_frames * sizeof(int));
    // Initialisera alla frameArray-värden till något ogiltigt, t.ex. -1
    for (int i = 0; i < num_frames; i++) {
        frameArray[i] = -1;  // Använd ett värde som inte förekommer i decimal_value
        usageOrder[i] = -1;  // Noll betyder att den inte har använts än
    }
    
    if (algorithm == NULL || num_frames <= 0 || filename == NULL) {          //Vi kontrollerar så att alla argument angivits
        fprintf(stderr, "Alla argument (-a, -n, -f) måste anges och antal ramar måste vara större än 0.\n");
        exit(EXIT_FAILURE);
    }
    printf("%s\n",algorithm);


    FILE *file = fopen(filename, "r");                  // Vi använder det inmatade filnamnet för att öppna filen och spara den i "file"
    if (file == NULL) {                                 // Hanterar fall där filen inte hittades
        perror("Kunde inte öppna filen");
        exit(EXIT_FAILURE);
    }
    line = (char*) malloc(256 * sizeof (char));                                 //Allokerar en buffer för att hålla dem inlästa addresserna

    while (fgets(line, sizeof(line), file)) {           
            char first_char = line[2];     // Tecken på position 2
            char second_char = line[3];    // Tecken på position 3
    
            // Skapa en sträng som innehåller de två tecknen
            char hex_str[3];          // Behöver plats för två tecken + null-terminator
            hex_str[0] = first_char;
            hex_str[1] = second_char;
            hex_str[2] = '\0';        // Lägg till null-terminator
    
            // Använd strtol för att konvertera från hexadecimal sträng till decimalt tal
            int decimal_value = (int)strtol(hex_str, NULL, 16);
            binPageNR[lineCount] = decimal_value;
            lineCount++;
            // Konvertera hexadecimal sträng till ett heltal
            unsigned int address = (unsigned int)strtol(line, NULL, 16);
            pageHit = pageAlgo(decimal_value, address, frameArray, num_frames, algorithm, usageOrder);
            memAccess++;
    }

    printf("\nRunning simulation... Done\n\n");
    printf("Simulation Summary\n\n");
    printf("Algorithm: %s\n", algorithm);
    printf("Frames: %d\n", num_frames);
    printf("Memory accesses: %d\n", memAccess);
    printf("Page hits: %d\n", pageHit);
    printf("Page faults: %d\n", memAccess-pageHit);
    printf("Page replacements: %d\n", (memAccess-pageHit)-num_frames);    
    
    fclose(file);    
    free(line);
    free(pageNR);
    free(binPageNR);
    return 0;
}