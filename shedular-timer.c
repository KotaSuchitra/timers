#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define INTERVAL_SEC 1 // Task runs every 1 second
#define RUN_COUNT 5    // Run 5 times for demo

int task_count = 0;
struct timespec start_time, end_time;

// Function to calculate elapsed time in seconds
double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + 
           (end.tv_nsec - start.tv_nsec) / 1e9;
}

// Task that runs periodically
void task_handler(int signum) {
    clock_gettime(CLOCK_REALTIME, &end_time);
    double elapsed = get_elapsed_time(start_time, end_time);

    printf("⏱️ Task %d executed at %.3f seconds since start.\n",
           ++task_count, elapsed);

    if (task_count >= RUN_COUNT) {
        printf("\n✅ Finished %d scheduled tasks. Exiting.\n", RUN_COUNT);
        exit(0);
    }
}

int main() {
    struct itimerval timer;
    struct sigaction sa;

    // Record start time
    clock_gettime(CLOCK_REALTIME, &start_time);

    // Setup signal handler for SIGALRM
    sa.sa_handler = &task_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    // Configure timer to expire after INTERVAL_SEC sec
    timer.it_value.tv_sec = INTERVAL_SEC;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = INTERVAL_SEC; // Repeat interval
    timer.it_interval.tv_usec = 0;

    // Start the timer
    setitimer(ITIMER_REAL, &timer, NULL);

    printf("🚀 Periodic scheduler started (interval = %d sec)\n", INTERVAL_SEC);
    printf("Press Ctrl+C to exit early.\n\n");

    // Infinite loop — real scheduling handled by signals
    while (1) {
        pause(); // Wait for next signal
    }

    return 0;
}
