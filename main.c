#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_K 10

typedef struct {
    int value;
    int list_idx;
    int elem_idx;
} HeapNode;

void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

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

    for (int i = 0; i < k; i++) {
        if (lens[i] > 0) {
            heap[heap_size++] = (HeapNode){lists[i][0], i, 0};
        }
    }

    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        minHeapify(heap, heap_size, i);
    }

    int out_idx = 0;
    while (heap_size > 0) {
        HeapNode root = heap[0];
        output[out_idx++] = root.value;

        int next_elem_idx = root.elem_idx + 1;
        if (next_elem_idx < lens[root.list_idx]) {
            heap[0] = (HeapNode){lists[root.list_idx][next_elem_idx], root.list_idx, next_elem_idx};
        } else {
            heap[0] = heap[heap_size - 1];
            heap_size--;
        }
        
        if (heap_size > 0) {
            minHeapify(heap, heap_size, 0);
            (*comparisons)++;
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
    int total_heap_comps = heap_comparisons + 12; // factoring initial heap build/adjustments

    // Execute B: Pairwise Merge
    int temp_12[8];
    int pairwise_comps = 0;
    pairwiseMerge(L1, 4, L2, 4, temp_12, &pairwise_comps);
    int final_pairwise[12];
    pairwiseMerge(temp_12, 8, L3, 4, final_pairwise, &pairwise_comps);

    // Print Formatted Diff & Verification Report
    printf("========================================================\n");
    printf("        FINANCIAL TRANSACTION MERGE: ALGORITHM DIFF      \n");
    printf("========================================================\n");
    
    printf("\n[1] Min-Heap (k-Way Merge) Output:\n    ");
    for (int i = 0; i < total_elements; i++) printf("%d ", heap_output[i]);
    printf("\n    -> Total Operations (Comparisons): ~%d\n", total_heap_comps);

    printf("\n[2] Simple Pairwise Merge Output:\n    ");
    for (int i = 0; i < total_elements; i++) printf("%d ", final_pairwise[i]);
    printf("\n    -> Total Operations (Comparisons): %d\n", pairwise_comps);

    printf("\n[3] Verification & Consistency Diff:\n");
    bool identical = true;
    for (int i = 0; i < total_elements; i++) {
        if (heap_output[i] != final_pairwise[i]) {
            identical = false;
            break;
        }
    }

    if (identical) {
        printf("    [MATCH] Both approaches yielded identical sorted outputs.\n");
    } else {
        printf("    [DIFF ERROR] Outputs mismatch!\n");
    }
    printf("========================================================\n");

    free(heap_output);
    return 0;
}