#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools/dynamic_array.h"

int main() {
    printf("Testing DYN_ARR_destructor...\n");
    
    // Test 1: Destructor on empty array
    t_dyn_arr *arr1 = DYN_ARRAY_constructor();
    assert(arr1 != NULL);
    int8_t result = DYN_ARR_destructor(&arr1);
    assert(result == 0);
    assert(NULL == arr1);
    printf("✓ Empty array destructor works\n");
    
    // Test 2: Destructor on array with elements
    t_dyn_arr *arr2 = DYN_ARRAY_constructor();
    assert(arr2 != NULL);
    
    // Add some elements
    int *data1 = malloc(sizeof(int));
    *data1 = 42;
    DYN_ARR_add(arr2, data1);
    
    double *data2 = malloc(sizeof(double));
    *data2 = 3.14;
    DYN_ARR_add(arr2, data2);
    
    result = DYN_ARR_destructor(&arr2);
    assert(result == 0);
    assert(NULL ==  arr2);
    printf("✓ Array with elements destructor works\n");
    
    // Test 3: Destructor on array with many elements
    t_dyn_arr *arr3 = DYN_ARRAY_constructor();
    assert(arr3 != NULL);
    
    for (int i = 0; i < 20; i++) {
        int *data = malloc(sizeof(int));
        *data = i;
        DYN_ARR_add(arr3, data);
    }
    
    result = DYN_ARR_destructor(&arr3);
    assert(result == 0);
    assert(NULL ==  arr3);
    printf("✓ Array with many elements destructor works\n");
    
    // Test 4: Destructor with NULL pointer
    result = DYN_ARR_destructor(NULL);
    assert(result == 0);
    printf("✓ NULL pointer destructor handled\n");
    
    // Test 5: Double destructor call (should handle gracefully)
    t_dyn_arr *arr4 = DYN_ARRAY_constructor();
    assert(arr4 != NULL);
    
    int *data = malloc(sizeof(int));
    *data = 100;
    DYN_ARR_add(arr4, data);
    
    result = DYN_ARR_destructor(&arr4);
    assert(result == 0);
    
    // Note: Calling destructor twice on same pointer is undefined behavior
    // This test is mainly to document expected behavior
    printf("✓ First destructor call successful\n");

    result = DYN_ARR_destructor(&arr4);
    assert(result == 0);

    printf("✓ Second destructor call successful\n");
    
    printf("All destructor tests passed!\n");
    return 0;
}
