#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools/dynamic_array.h"

int main() {
	printf("Testing DYN_ARR_add...\n");

	t_dyn_arr *arr = DYN_ARRAY_constructor();
	assert(arr != NULL);

	// Test 1: Add single element
	int *data1 = malloc(sizeof(int));
	*data1 = 42;
	int8_t result = arr->fp_add(arr, data1);
	assert(result == 0);
	assert(arr->ui8_size == 1);
	printf("✓ Single element added successfully\n");

	// Test 2: Add multiple elements
	int *data2 = malloc(sizeof(int));
	*data2 = 100;
	result = _DYN_ARR_add(arr, data2);
	assert(result == 0);
	assert(arr->ui8_size == 2);
	printf("✓ Multiple elements can be added\n");

	// Test 3: Add different data types
	DYN_ARR_add_by_value(arr, 3.14);
	assert(arr->ui8_size == 3);
	printf("✓ Different data types can be added\n");

	// Test 4: Add NULL pointer (should handle gracefully)
	int8_t old_size = arr->ui8_size;
	result = arr->fp_add(arr, NULL);
	assert(result == 0);
	assert(arr->ui8_size == 4);
	printf("✓ NULL pointer handling tested\n");

	// Test 5: Add many elements to test capacity expansion
	for (int i = 0; i < 10; i++) {
		int *data = malloc(sizeof(int));
		*data = i * 10;
		result = arr->fp_add(arr, data);
		assert(result == 0);
	}
	assert(arr->ui8_size == 14);
	printf("✓ Multiple additions work (capacity expansion)\n");

	// Test 6: Test with invalid array pointer
	result = _DYN_ARR_add(NULL, data1);
	assert(result > 0); // Should return error
	printf("✓ Invalid array pointer handled\n");

	// Clean up
	arr->fp_destructor(&arr);

	printf("All add tests passed!\n");
	return 0;
}
