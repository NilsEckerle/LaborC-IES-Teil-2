#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools/dynamic_array.h"

int main() {
    printf("Testing DYN_ARR_get...\n");
    
    t_dyn_arr *arr = DYN_ARRAY_constructor();
    assert(arr != NULL);
    
    // Add some test data
    int *data1 = malloc(sizeof(int));
    *data1 = 42;
    arr->fp_add(arr, data1);
    
    double *data2 = malloc(sizeof(double));
    *data2 = 3.14;
    arr->fp_add(arr, data2);
    
    char *data3 = malloc(sizeof(char));
    *data3 = 'A';
    arr->fp_add(arr, data3);

    char data4 = 'B';
    DYN_ARR_add_by_value(arr, data4);
    
    // Test 1: Get first element
    void *retrieved = arr->fp_get(arr, 0);
    assert(retrieved != NULL);
    assert(*(int*)retrieved == 42);
    printf("✓ First element retrieved correctly\n");
    
    // Test 2: Get second element (different type)
    retrieved = DYN_ARR_get(arr, 1);
    assert(retrieved != NULL);
    assert(*(double*)retrieved == 3.14);
    printf("✓ Second element retrieved correctly\n");
    
    // Test 3: Get third element
    retrieved = DYN_ARR_get_as_ptr(arr, 2, char *);
    assert(retrieved != NULL);
    assert(*(char*)retrieved == 'A');
    printf("✓ Third element retrieved correctly\n");

    // Test 4: Get third element
    char retrieved_char = DYN_ARR_get_as_type(arr, 3, char);
    assert(retrieved_char == 'B');
    printf("✓ Fourth element retrieved correctly\n");
    
    // Test 5: Get element with negative index
    retrieved = arr->fp_get(arr, -1);
    assert(retrieved == NULL); // Should return NULL for invalid index
    printf("✓ Negative index handled\n");
    
    // Test 6: Get element with index >= size
    retrieved = arr->fp_get(arr, 10);
    assert(retrieved == NULL); // Should return NULL for out-of-bounds
    printf("✓ Out-of-bounds index handled\n");
    
    // Test 7: Get from empty array
    t_dyn_arr *empty_arr = DYN_ARRAY_constructor();
    retrieved = empty_arr->fp_get(empty_arr, 0);
    assert(retrieved == NULL);
    printf("✓ Get from empty array handled\n");
    
    // Test 8: Get with NULL array pointer
    retrieved = DYN_ARR_get(NULL, 0);
    assert(retrieved == NULL);
    printf("✓ NULL array pointer handled\n");
    
    // Test 9: Test macro DYN_ARR_get_as_type
    int retrieved_int = DYN_ARR_get_as_type(arr, 0, int);
    assert(retrieved_int == 42);
    printf("✓ Macro DYN_ARR_get_as_type works\n");
    
    // Test 9: Test macro DYN_ARR_get_as_ptr
    int *retrieved_ptr = DYN_ARR_get_as_ptr(arr, 0, int*);
    assert(retrieved_ptr != NULL);
    assert(*retrieved_ptr == 42);
    printf("✓ Macro DYN_ARR_get_as_ptr works\n");
    
    // Clean up
    arr->fp_destructor(&arr);
    empty_arr->fp_destructor(&empty_arr);
    
    printf("All get tests passed!\n");
    return 0;
}
