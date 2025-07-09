#include <assert.h>
#include <stdio.h>
#include "tools/dynamic_array.h"

int main() {
	printf("Testing DYN_ARRAY_constructor...\n");

	// Test 1: Constructor returns non-NULL pointer
	t_dyn_arr *arr = DYN_ARRAY_constructor();
	assert(arr != NULL);
	printf("✓ Constructor returns non-NULL pointer\n");

	// Test 2: Initial size is 0
	assert(arr->ui8_size == 0);
	printf("✓ Initial size is 0\n");

	// Test 3: Data array is NULL
	assert(arr->vpp_data_array == NULL);
	printf("✓ Data array is NULL\n");

	// Test 4: Function pointers are set
	assert(arr->fp_add != NULL);
	printf("✓ Add function pointer is set\n");

	assert(arr->fp_get != NULL);
	printf("✓ Get function pointer is set\n");

	assert(arr->fp_destructor != NULL);
	printf("✓ Destructor function pointer is set\n");

	// Test 5: Multiple constructor calls return different instances
	t_dyn_arr *arr2 = DYN_ARRAY_constructor();
	assert(arr2 != NULL);
	assert(arr != arr2);
	printf("✓ Multiple constructor calls return different instances\n");

	// Clean up
	arr->fp_destructor(&arr);
	arr2->fp_destructor(&arr2);

	printf("All constructor tests passed!\n");
	return 0;
}
