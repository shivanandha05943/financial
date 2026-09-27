#include <stdio.h>
#include <stdlib.h>

#define MAX_K 10

// Structure for heap elements
typedef struct {
    int value;
    int list_idx;
    int elem_idx;
} HeapNode;

// Function to swap two heap nodes
void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Min-Heapify function
void minHeapify(HeapNode heap[], int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].value < heap[smallest].value)
        smallest = left;

    if (right < size && heap[right].value < heap[smallest].value)
        smallest = right;

    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        minHeapify(heap, size, smallest);
    }
}

// a) K-Way Merge using Min-Heap
void kWayMerge(int *lists[], int lens[], int k, int output[], int *comparisons) {
    HeapNode heap[MAX_K];
    int heap_size = 0;
    *comparisons = 0;

    // Initialize heap with the first element of each list
    for (int i = 0; i < k; i++) {
        if (lens[i] > 0) {
            heap[heap_size++] = (HeapNode){lists[i][0], i, 0};
        }
    }

    // Build initial heap
    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        minHeapify(heap, heap_size, i);
    }

    int out_idx = 0;
    printf("\n--- Min-Heap Trace States ---\n");
    
    while (heap_size > 0) {
        // Extract minimum
        HeapNode root = heap[0];
        output[out_idx++] = root.value;

        printf("Extracted: %d (from L%d). Remaining Heap size: %d\n", root.value, root.list_idx + 1, heap_size - 1);

        // If there is a next element in the same list, insert it into the heap
        int next_elem_idx = root.elem_idx + 1;
        if (next_elem_idx < lens[root.list_idx]) {
            heap[0] = (HeapNode){lists[root.list_idx][next_elem_idx], root.list_idx, next_elem_idx};
        } else {
            // Replace root with last element and shrink heap
            heap[0] = heap[heap_size - 1];
            heap_size--;
        }
        
        if (heap_size > 0) {
            minHeapify(heap, heap_size, 0);
            (*comparisons)++; // Increment comparison metric approximation
        }
    }
}

// b) Simple Pairwise Merging Approach
void pairwiseMerge(int *L1, int n1, int *L2, int n2, int temp[], int *comp_count) {
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        (*comp_count)++;
        if (L1[i] < L2[j]) {
            temp[k++] = L1[i++];
        } else {
            temp[k++] = L2[j++];
        }
    }
    while (i < n1) temp[k++] = L1[i++];
    while (j < n2) temp[k++] = L2[j++];
}

int main() {
    // Input Lists
    int L1[] = {10, 30, 50, 70};
    int L2[] = {20, 40, 60, 80};
    int L3[] = {15, 35, 55, 75};

    int *lists[] = {L1, L2, L3};
    int lens[] = {4, 4, 4};
    int k = 3;
    int total_elements = 12;

    // Execute A: K-Way Merge
    int *heap_output = (int *)malloc(total_elements * sizeof(int));
    int heap_comparisons = 0;
    
    kWayMerge(lists, lens, k, heap_output, &heap_comparisons);

    printf("\nFinal Merged Output (Min-Heap): ");
    for (int i = 0; i < total_elements; i++) {
        printf("%d ", heap_output[i]);
    }
    printf("\nApproximate Heap Comparisons: ~%d\n", heap_comparisons + 12);

    // Execute B: Pairwise Merge
    int temp_12[8];
    int pairwise_comps = 0;
    pairwiseMerge(L1, 4, L2, 4, temp_12, &pairwise_comps);

    int final_pairwise[12];
    pairwiseMerge(temp_12, 8, L3, 4, final_pairwise, &pairwise_comps);

    printf("\n--- Pairwise Merging Results ---\n");
    printf("Final Merged Output (Pairwise): ");
    for (int i = 0; i < total_elements; i++) {
        printf("%d ", final_pairwise[i]);
    }
    printf("\nTotal Pairwise Comparisons: %d\n", pairwise_comps);

    free(heap_output);
    return 0;
}