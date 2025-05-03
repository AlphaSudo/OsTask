#include <stdio.h>
#include <stdlib.h>

int compare_asc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compare_desc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

void fcfs(int arr[], int head, int n) {
    int total = 0;
    printf("\nFCFS Order: %d ", head);
    for (int i = 0; i < n; i++) {
        printf("-> %d ", arr[i]);
        total += abs(head - arr[i]);
        head = arr[i];
    }
    printf("\nTotal Head Movement = %d\n", total);
}

void scan(int arr[], int head, int n, int max_cylinder) {
    int total = 0;
    int *left = malloc(n * sizeof(int));
    int *right = malloc(n * sizeof(int));
    if (!left || !right) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    int l = 0, r = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] < head)
            left[l++] = arr[i];
        else
            right[r++] = arr[i];
    }

    qsort(left, l, sizeof(int), compare_desc);
    qsort(right, r, sizeof(int), compare_asc);

    printf("\nSCAN Order: %d ", head);
    for (int i = 0; i < r; i++) {
        printf("-> %d ", right[i]);
        total += abs(head - right[i]);
        head = right[i];
    }

    if (l > 0) {
        total += abs(head - left[0]);  // turn around
        head = left[0];
        printf("-> %d ", head);
        for (int i = 1; i < l; i++) {
            printf("-> %d ", left[i]);
            total += abs(head - left[i]);
            head = left[i];
        }
    }

    printf("\nTotal Head Movement = %d\n", total);
    free(left);
    free(right);
}

void cscan(int arr[], int head, int n, int max_cylinder) {
    int total = 0;
    int *left = malloc(n * sizeof(int));
    int *right = malloc(n * sizeof(int));
    if (!left || !right) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    int l = 0, r = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] < head)
            left[l++] = arr[i];
        else
            right[r++] = arr[i];
    }

    qsort(left, l, sizeof(int), compare_asc);
    qsort(right, r, sizeof(int), compare_asc);

    printf("\nC-SCAN Order: %d ", head);
    for (int i = 0; i < r; i++) {
        printf("-> %d ", right[i]);
        total += abs(head - right[i]);
        head = right[i];
    }

    if (l > 0) {
        total += abs(head - (max_cylinder - 1));  // go to max
        total += max_cylinder - 1;                // jump to 0
        head = 0;
        printf("-> 0 ");
        for (int i = 0; i < l; i++) {
            printf("-> %d ", left[i]);
            total += abs(head - left[i]);
            head = left[i];
        }
    }

    printf("\nTotal Head Movement = %d\n", total);
    free(left);
    free(right);
}

int main() {
    int n, head, max_cylinder = 5000;

    printf("Enter number of disk requests: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of requests.\n");
        return 1;
    }

    int *queue = malloc(n * sizeof(int));
    if (!queue) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("Enter disk request queue: ");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &queue[i]) != 1 || queue[i] < 0 || queue[i] >= max_cylinder) {
            printf("Invalid disk request at position %d.\n", i);
            free(queue);
            return 1;
        }
    }

    printf("Enter initial head position: ");
    if (scanf("%d", &head) != 1 || head < 0 || head >= max_cylinder) {
        printf("Invalid head position.\n");
        free(queue);
        return 1;
    }

    fcfs(queue, head, n);
    scan(queue, head, n, max_cylinder);
    cscan(queue, head, n, max_cylinder);

    free(queue);
    return 0;
}
