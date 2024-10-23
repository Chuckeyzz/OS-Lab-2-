//#include <bits/getopt_core.h>           //for linux compilation 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For getopt()
#include "FIFO.c"
#define BUFFER 256
#define MAXFRAMES 255

int main(int argc, char *argv[]) {
    char *algorithm = NULL;     // To store the selected algorithm
    int num_frames = 0;         // To store the selected number of frames
    char *filename = NULL;      // To store the selected file
    int opt;                    // To hold the value returned by getopt()
    int pageHits = 0;           // Track page hits
	char *algUsed;
    char *line;
    unsigned int *binPageNR = malloc(BUFFER * sizeof(unsigned int));  // Allocate memory for page numbers
    int lineCount = 0;  // To count the number of pages
    int pageFaults = 0; // Total page fault counter

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "a:n:f:")) != -1) {
        switch (opt) {
            case 'a':
                algorithm = optarg;
                break;
            case 'n':
                num_frames = atoi(optarg);
                break;
            case 'f':
                filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -a <fifo|optimal|lru> -n <frames> -f <file>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

	if(strcmp(algorithm, "fifo") == 0){
		algUsed = "FIFO"; 
	}

    if (algorithm == NULL || num_frames <= 0 || filename == NULL) {
        fprintf(stderr, "All arguments (-a, -n, -f) must be provided and number of frames must be greater than 0.\n");
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
    while (fgets(line, BUFFER, file)) {
        // Get the full address (hexadecimal) from the line
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
