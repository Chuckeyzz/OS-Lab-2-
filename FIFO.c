int FIFO(unsigned int current_page, int num_frames, int *pageHits) {
    static unsigned int *frames = NULL;  // Static array for frames, shared across calls
    static int next_frame = 0;           // To track which frame to replace next
    static int free_frames = 0;          // To track the number of free frames
    static int initialized = 0;          // Flag to initialize only once
    int page_fault = 0;                  // To track if this page caused a fault

    // Initialize the frames array on the first call
    if (!initialized) {
        frames = malloc(num_frames * sizeof(unsigned int));
        for (int i = 0; i < num_frames; i++) {
            frames[i] = -1;  // Initialize as empty
        }
        free_frames = num_frames;  // Initially, all frames are free
        initialized = 1;  // Mark as initialized
    }

    // Check if the page is already in memory
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == current_page) {
            printf("Page %d is already in physical memory (Frame %d)\n", current_page, i);
            (*pageHits)++;  // Increment page hits
            return 0;  // No page fault occurred
        }
    }

    // If the page is not found in memory, it's a page fault
    printf("Page %d not in physical memory\n", current_page);

    // If there are free frames, use the next free frame
    if (free_frames > 0) {
        for (int i = 0; i < num_frames; i++) {
            if (frames[i] == -1) {
                frames[i] = current_page;
                printf("Page %d paged in (Frame %d)\n", current_page, i);
                free_frames--;  // Reduce the count of free frames
                return 1;  // Page fault occurred, but no page was replaced
            }
        }
    }

    // If no free frames, use FIFO to replace the oldest page
    printf("Page %d paged out\n", frames[next_frame]);

    // Replace the page in the next frame
    frames[next_frame] = current_page;
    printf("Page %d paged in (Frame %d)\n", current_page, next_frame);

    // Update the next frame index (FIFO circular buffer logic)
    next_frame = (next_frame + 1) % num_frames;

    // Increment page fault count
    page_fault = 1;

    return page_fault;
}