#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // För getopt()

int main(int argc, char argv[]){
    char *algorithm = NULL;                              // Sträng för att lagra den valda algoritmen
    int num_frames = 0;                                 // Integer som lagrar det valda antalet frames
    char *filename = NULL;                               // Sträng som lagrar den valda filen
    int opt;                                             // En integer som håller värdet som getopt() returnerar, getopt() returnerar dem angivna tecknen a:n:f: som ett heltal om den stöter på dem (-a -b -f)
	//Git test 3
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
if (algorithm == NULL || num_frames <= 0 || filename == NULL) {          //Vi kontrollerar så att alla argument angivits
        fprintf(stderr, "Alla argument (-a, -n, -f) måste anges och antal ramar måste vara större än 0.\n");
        exit(EXIT_FAILURE);
    }
    printf(algorithm);

FILE *file = fopen(filename, "r");                  // Vi använder det inmatadde filnamnet för att öppna filen och spara den i "file"
    if (file == NULL) {                                 // Hanterar fall där filen inte hittades
        perror("Kunde inte öppna filen");
        exit(EXIT_FAILURE);
    }

    char line[256];                                     //Allokerar en buffer för att hålla dem inlästa addresserna
    while (fgets(line, sizeof(line), file)) {           // En whileloop som läser från filen till "line" tills fgets returnerar NULL
        line[strcspn(line, "\n")] = '\0';               // Tar bort new line tecken och ersätter dem med \0

        unsigned int address = (unsigned int)strtol(line, NULL, 16); // Konvertera hexadecimal sträng till ett heltal

        printf("Läste adress: 0x%04X\n", address);
    }
    fclose(file);    

    return 0;   
}