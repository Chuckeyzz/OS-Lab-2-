#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // För getopt()
#include <time.h>
//#include <bits/getopt_core.h>           //for linux compilation 
//#include "FIFO.c"

int main(int argc, char *argv[]){

    char *algorithm = NULL;                              // Sträng för att lagra den valda algoritmen
    int num_frames = 0;                                 // Integer som lagrar det valda antalet frames
    char *filename = NULL;                               // Sträng som lagrar den valda filen
    int opt;                                             // En integer som håller värdet som getopt() returnerar, getopt() returnerar dem angivna tecknen a:n:f: som ett heltal om den stöter på dem (-a -b -f)

	char *line;
	char *pageNR = (char*) malloc(256 * sizeof(char));
	int lineCount = 0;


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
    printf("%s\n",algorithm);


	FILE *file = fopen(filename, "r");                  // Vi använder det inmatade filnamnet för att öppna filen och spara den i "file"
    if (file == NULL) {                                 // Hanterar fall där filen inte hittades
        perror("Kunde inte öppna filen");
        exit(EXIT_FAILURE);
    }
    line = (char*) malloc(256 * sizeof (char));                                 //Allokerar en buffer för att hålla dem inlästa addresserna


	//fread(line, 1, 1000, file);  

    int pair_exists(char *pageNR, int length, char first_char, char second_char) {
        for (int i = 0; i < length; i += 2) { // Gå igenom pageNR i steg om 2 eftersom varje par består av två tecken
            if (pageNR[i] == first_char && pageNR[i + 1] == second_char) {
                return 1; // Paret hittades, returnera 1
            }
        }
        return 0; // Paret hittades inte, returnera 0
    }
    
	while (fgets(line, sizeof(line), file)) {           
        char first_char = line[2];     // Tecken på position 2
        char second_char = line[3];    // Tecken på position 3
    
        // Kontrollera om paret redan finns
        if (!pair_exists(pageNR, lineCount, first_char, second_char)) { //Vi kollar så att teckenparet inte redan finns i pangeNR
            pageNR[lineCount] = first_char;    // Lägg till det första tecknet i pageNR
            pageNR[lineCount + 1] = second_char; // Lägg till det andra tecknet i pageNR
            lineCount += 2;                   // Öka lineCount med 2 eftersom vi lagt till ett par
        }
    
        // Konvertera hexadecimal sträng till ett heltal
        unsigned int address = (unsigned int)strtol(line, NULL, 16);
        printf("Läste adress: 0x%04X\n", address);
    }
    
	/*for(int i = 0; i < (strlen(line)/6); i++){					//for-loop för att kolla vilket pagenummer alla rader tillhör
		if (i == 0){
			pageNR[i] = line [(i+2)*i];
			pageNR[i+1] = line [(i+3)*i];
			printf("Hello");
		}
		for (int j = 0; j < strlen(pageNR); j++){
			if(line [(i+2)*i] != pageNR[j] && line [(i+3)*i] != pageNR[j+1]){
				pageNR[i] = line [(i+2)*i];
				pageNR[i+1] = line [(i+3)*i];
			}
		}
	}*/

	
    printf("\n%s",pageNR);


    fclose(file);    
    free(line);
    free(pageNR);


    return 0;
}