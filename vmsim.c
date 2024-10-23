//#include <bits/getopt_core.h>           //for linux compilation 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For getopt()
#include "FIFO.c"
#define BUFFER 256
#define MAXFRAMES 255

int main(int argc, char *argv[]) {
    char *algorithm = NULL;     
    int num_frames = 0;         
    char *filename = NULL;      
    int opt;                    
    int pageHits = 0;           
	char *algUsed;
    char *line;
    unsigned int *binPageNR = malloc(BUFFER * sizeof(unsigned int));  // Allocate memory for page numbers
    int lineCount = 0;  
    int pageFaults = 0; 


    while ((opt = getopt(argc, argv, "a:n:f:")) != -1) {        // Startar en loop som går igenom alla kommandoradsalternativ, när det inte finns fler alternativ returnerar getopt() -1
        switch (opt) {                                          // Beroende på vilket alternativ som hittats av getopt()  
            case 'a':
                algorithm = optarg;                             //optarg pekar på argumentet efter det hittade teckner, i detta fall det som kommer efter -a
                break;
            case 'n':
                num_frames = atoi(optarg);                       // Konvertera argumentet efter -n till en integer och spara i num_frames
                break;
            case 'f':
                filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -a <fifo|optimal|lru> -n <frames> -f <file>\n", argv[0]);    // Om ett ogiltigt alternativ upptäcks, skriv ut användningsmeddelande och avsluta programmet med fel
                exit(EXIT_FAILURE);
        }
    }

	if(strcmp(algorithm, "fifo") == 0){
		algUsed = "FIFO"; 
	}

    if (algorithm == NULL || num_frames <= 0 || filename == NULL) {
        fprintf(stderr, "Alla argument (-a, -n, -f) måste anges och antal ramar måste vara större än 0.\n");
        exit(EXIT_FAILURE);
    }

    printf("Selected algorithm: %s\n", algorithm);

    // Open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    line = (char *) malloc(BUFFER * sizeof(char));  // Allocate memory for reading lines from file

    // Read the file and process each address one by one
    while (fgets(line, BUFFER, file)) {                 // En whileloop som läser från filen till "line" tills fgets returnerar NULL
       
        unsigned int address = (unsigned int) strtol(line, NULL, 16);
        printf("Read address: 0x%04X\n", address);

        // Extract the page number from the first two hex digits (most significant byte)
        unsigned int page_number = (address >> 8) & 0xFF;

        // Check if the algorithm is FIFO
        if (strcmp(algorithm, "fifo") == 0) {
            // Call FIFO function with each page as we read them
            pageFaults += FIFO(page_number, num_frames, &pageHits);
        }
    }

    // Final results
	printf("\nAlgorithm used: %s\n", algUsed);
	printf("Number of frames used: %d\n", num_frames);
    printf("Total page faults: %d\n", pageFaults);
    printf("Total page hits: %d\n", pageHits);

    // Clean up and close
    fclose(file);
    free(line);
    free(binPageNR);

    return 0;
}
