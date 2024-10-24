#include <stdio.h>
#include <string.h>
#include <time.h>

// Deklaration av funktioner
int page_exists_in_frame(int decimal_value, int address, int *frameArray, int num_frames, char *algo, int *usageOrder);
void fifo_replace_page(int decimal_value, int address, int *frameArray, int num_frames);
void lru_replace_page(int decimal_value, int address, int *frameArray, int num_frames, int *usage_order); 
//void opt_replace_page(int decimal_value, int address, int *frameArray, int num_frames, int *usage_order);
int returnValue = 0;
int usageCounter = 0;

// FIFO-funktion som anropar andra funktioner
int pageAlgo(int decimal_value, int address, int *frameArray, int num_frames, char *algo, int *usageOrder) { 
    returnValue += page_exists_in_frame(decimal_value, address, frameArray, num_frames, algo, usageOrder);
    return returnValue;
}

// Funktion för att kontrollera om sidan finns i frameArray
int page_exists_in_frame(int decimal_value, int address, int *frameArray, int num_frames, char *algo, int *usageOrder) {
    // Kontrollera om sidan redan finns i frameArray
    for (int i = 0; i < num_frames; i++) {
        if (frameArray[i] == decimal_value && frameArray[i] != -1) {
            printf("Address %04X is on page %d which is already in physical memory\n", address, decimal_value);
            usageOrder[i] = ++usageCounter;  // Uppdatera användningshistoriken när sidan används
            return 1;
        }
    }
    
    printf("Address %04X not in physical memory\n", address);
    if (strcmp(algo, "fifo") == 0) {
        fifo_replace_page(decimal_value, address, frameArray, num_frames);
    }else if(strcmp(algo, "lru") == 0){
        lru_replace_page(decimal_value, address, frameArray, num_frames, usageOrder);
    }
    return 0;
}

// Funktion för att ersätta en sida med FIFO
void fifo_replace_page(int decimal_value, int address, int *frameArray, int num_frames) {
    static int fifo_index = 0;  // Håller reda på vilken sida som är nästa att ersättas

    // Ersätt den äldsta sidan (FIFO) med den nya sidan
    if(frameArray[fifo_index] != -1){
        printf("Page #%d paged out\n", frameArray[fifo_index]);
    }
    printf("Page #%d paged in\n", decimal_value);

    // Byt ut den äldsta sidan med den nya
    frameArray[fifo_index] = decimal_value;

    // Flytta FIFO-indexet till nästa plats (cirkulär buffert)
    fifo_index = (fifo_index + 1) % num_frames; // Cirkulär kö
}

void lru_replace_page(int decimal_value, int address, int *frameArray, int num_frames, int *usage_order) { 
    int lru_index = 0;  // Håller reda på vilken sida som är minst nyligen använd
    
    // Leta efter den minst nyligen använda sidan
    for (int i = 1; i < num_frames; i++) {
        if (usage_order[i] < usage_order[lru_index]) {
            lru_index = i;  // Uppdatera index för den minst nyligen använda sidan
        }
    }

    // Ersätt den minst nyligen använda sidan med den nya sidan
    if (frameArray[lru_index] != -1) {
        printf("Page #%d paged out from frame %d\n", frameArray[lru_index], lru_index);
    }
    printf("Page #%d paged in into frame %d\n", decimal_value, lru_index);

    // Byt ut den minst nyligen använda sidan med den nya
    frameArray[lru_index] = decimal_value;

    // Uppdatera användningshistoriken
    usage_order[lru_index] = ++usageCounter;
}

/*void opt_replace_page(int decimal_value, int address, int *frameArray, int num_frames, int *usage_order) {
    int replaceIndex = -1;
    int farthestUsage = -1;
    int i, j;

    // Om det finns tomma ramar, använd dem först
    for (i = 0; i < num_frames; i++) {
        if (frameArray[i] == -1) {
            replaceIndex = i;
            break;
        }
    }

    if (replaceIndex == -1) {
        // Alla ramar är fulla, hitta den sida som inte används på längst tid framöver
        int maxDistance = -1;

        for (i = 0; i < num_frames; i++) {
            int page = frameArray[i];
            int found = 0;

            // Sök efter nästa användning av sidan i framtiden
            for (j = currentReferenceIndex + 1; j < totalReferences; j++) {
                if (page == pageReferences[j]) {
                    found = 1;
                    int distance = j - currentReferenceIndex;
                    if (distance > maxDistance) {
                        maxDistance = distance;
                        replaceIndex = i;
                    }
                    break;
                }
            }

            // Om sidan inte kommer att användas igen
            if (!found) {
                replaceIndex = i;
                break;
            }
        }

        // Om alla sidor kommer att användas igen, har replaceIndex redan satts
        if (replaceIndex == -1) {
            replaceIndex = 0; // Som säkerhetsåtgärd
        }
    }

    // Ersätt sidan
    if (frameArray[replaceIndex] != -1) {
        printf("Page #%d paged out from frame %d\n", frameArray[replaceIndex], replaceIndex);
    }
    printf("Page #%d paged in into frame %d\n", decimal_value, replaceIndex);

    frameArray[replaceIndex] = decimal_value;
}*/

