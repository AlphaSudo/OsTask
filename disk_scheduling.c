#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_requests(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void fcfs_schedule(int disk_requests[], int num_requests, int start_head) {
    int total_seek_time = 0;
    printf("FCFS Order: %d ", start_head);

    for (int i = 0; i < num_requests; i++) {
        total_seek_time += abs(disk_requests[i] - start_head);
        start_head = disk_requests[i];
        printf("-> %d ", start_head);
    }

    printf("\nTotal Seek Time: %d\n", total_seek_time);
}

void scan_schedule(int disk_requests[], int num_requests, int start_head, int cylinder_limit, int direction) {
    int total_seek_time = 0;

    qsort(disk_requests, num_requests, sizeof(int), compare_requests);

    printf("SCAN Order: %d ", start_head);

    int i = 0;
    while (i < num_requests && disk_requests[i] < start_head) i++;

    if (direction == 1) { // Move towards higher numbered cylinders
        // Move towards the end
        for (int j = i; j < num_requests; j++) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }

        // Go to the end
        if (start_head != cylinder_limit - 1) {
            total_seek_time += abs((cylinder_limit - 1) - start_head);
            start_head = cylinder_limit - 1;
            printf("-> %d ", start_head);
        }

        // Move back to the lowest remaining request
        for (int j = i - 1; j >= 0; j--) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }
    } else if (direction == -1) { // Move towards lower numbered cylinders
        // Move towards the beginning
        for (int j = i - 1; j >= 0; j--) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }

        // Go to the beginning
        if (start_head != 0) {
            total_seek_time += abs(0 - start_head);
            start_head = 0;
            printf("-> %d ", start_head);
        }

        // Move back to the highest remaining request
        for (int j = i; j < num_requests; j++) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }
    }

    printf("\nTotal Seek Time: %d\n", total_seek_time);
}

void c_scan_schedule(int disk_requests[], int num_requests, int start_head, int cylinder_limit, int direction) {
    int total_seek_time = 0;

    qsort(disk_requests, num_requests, sizeof(int), compare_requests);

    printf("C-SCAN Order: %d ", start_head);

    int i = 0;
    while (i < num_requests && disk_requests[i] < start_head) i++;

    if (direction == 1) { // Move towards higher numbered cylinders
        // Move right
        for (int j = i; j < num_requests; j++) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }

        // Go to the end
        if (start_head != cylinder_limit - 1) {
            total_seek_time += abs((cylinder_limit - 1) - start_head);
            start_head = cylinder_limit - 1;
            printf("-> %d ", start_head);
        }

        // Jump to beginning
        total_seek_time += (cylinder_limit - 1);
        start_head = 0;
        printf("-> %d ", start_head);

        // Process the rest
        for (int j = 0; j < i; j++) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }
    } else if (direction == -1) { // Move towards lower numbered cylinders
        // Move left
        for (int j = i - 1; j >= 0; j--) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }

        // Go to the beginning
        if (start_head != 0) {
            total_seek_time += abs(0 - start_head);
            start_head = 0;
            printf("-> %d ", start_head);
        }

        // Jump to the end
        total_seek_time += (cylinder_limit - 1);
        start_head = cylinder_limit - 1;
        printf("-> %d ", start_head);

        // Process the rest
        for (int j = num_requests - 1; j >= i; j--) {
            total_seek_time += abs(disk_requests[j] - start_head);
            start_head = disk_requests[j];
            printf("-> %d ", start_head);
        }
    }

    printf("\nTotal Seek Time: %d\n", total_seek_time);
}

int main() {
    int request_count, head_position, total_cylinders, direction;
    char scheduling_algorithm[10];

    printf("Enter number of disk requests: ");
    scanf("%d", &request_count);

    int requests[request_count];

    printf("Enter disk requests:\n");
    for (int i = 0; i < request_count; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head_position);

    printf("Enter total number of cylinders: ");
    scanf("%d", &total_cylinders);

    printf("Enter scheduling algorithm (FCFS/SCAN/CSCAN): ");
    scanf("%s", scheduling_algorithm);

    if (strcmp(scheduling_algorithm, "FCFS") == 0) {
        fcfs_schedule(requests, request_count, head_position);
    } else if (strcmp(scheduling_algorithm, "SCAN") == 0 || strcmp(scheduling_algorithm, "CSCAN") == 0) {
        // User input for track direction (1 for moving towards the higher numbered cylinders, -1 for moving towards the lower numbered cylinders )
        printf("Enter track direction (1 for moving towards the higher numbered cylinders, -1 for moving towards the lower numbered cylinders ): ");
        scanf("%d", &direction);

        if (strcmp(scheduling_algorithm, "SCAN") == 0) {
            scan_schedule(requests, request_count, head_position, total_cylinders, direction);
        } else if (strcmp(scheduling_algorithm, "CSCAN") == 0) {
            c_scan_schedule(requests, request_count, head_position, total_cylinders, direction);
        }
    } else {
        printf("Invalid scheduling algorithm.\n");
    }

    return 0;
}

