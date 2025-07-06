#include "unity.h"
#include "unity_config.h"

#include "dynamic_array.h"
#include <cstdint>
#include <string.h>

// Test helper structures
typedef struct person {
    char *name;
    int age;
} t_person;

// Global test variables
t_dyn_arr *test_array;

// Test setup function (called before each test)
void setUp(void) {
    test_array = NULL;
}

// Test teardown function (called after each test)
void tearDown(void) {
    if (test_array != NULL) {
        DYN_ARR_destructor(test_array);
        test_array = NULL;
    }
}

// Test constructor
void test_constructor(void) {
    test_array = DYN_ARRAY_constructor();
    
    TEST_ASSERT_NOT_NULL(test_array);
    TEST_ASSERT_EQUAL(0, test_array->ui8_size);
    TEST_ASSERT_NULL(test_array->vpp_data_array);
    TEST_ASSERT_NOT_NULL(test_array->fp_add);
    TEST_ASSERT_NOT_NULL(test_array->fp_get);
    TEST_ASSERT_NOT_NULL(test_array->fp_destructor);
}

// Test adding by pointer
void test_add_by_pointer(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Create test data
    int *value1 = malloc(sizeof(int));
    int *value2 = malloc(sizeof(int));
    double *value3 = malloc(sizeof(double));
    *value1 = 42;
    *value2 = 84;
    *value3 = 3.14;
    
    // Test adding elements
    int8_t result1 = test_array->fp_add(test_array, value1);
    TEST_ASSERT_EQUAL(0, result1);
    TEST_ASSERT_EQUAL(1, test_array->ui8_size);
    
    int8_t result2 = test_array->fp_add(test_array, value2);
    TEST_ASSERT_EQUAL(0, result2);
    TEST_ASSERT_EQUAL(2, test_array->ui8_size);
    
    int8_t result3 = test_array->fp_add(test_array, value3);
    TEST_ASSERT_EQUAL(0, result3);
    TEST_ASSERT_EQUAL(3, test_array->ui8_size);
}

// Test adding with NULL array
void test_add_by_pointer_null_array(void) {
    int *value = malloc(sizeof(int));
    *value = 42;
    
    int8_t result = test_array->fp_add(NULL, value);
    TEST_ASSERT_EQUAL(1, result);
}

void test_macro_add_by_value(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Test with different data types
    DYN_ARR_add_by_value(test_array, 42);
    TEST_ASSERT_EQUAL(1, test_array->ui8_size);
    
    DYN_ARR_add_by_value(test_array, 3.14);
    TEST_ASSERT_EQUAL(2, test_array->ui8_size);
    
    char test_char = 'A';
    DYN_ARR_add_by_value(test_array, test_char);
    TEST_ASSERT_EQUAL(3, test_array->ui8_size);
    
    // Verify values
    TEST_ASSERT_EQUAL(42, *(int*)DYN_ARR_get(test_array, 0));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.14, *(double*)DYN_ARR_get(test_array, 1));
    TEST_ASSERT_EQUAL('A', *(char*)DYN_ARR_get(test_array, 2));
}

// Test get function
void test_get(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Add some test data
    int *value1 = malloc(sizeof(int));
    int *value2 = malloc(sizeof(int));
    *value1 = 100;
    *value2 = 200;
    
    test_array->fp_add(test_array, value1);
    test_array->fp_add(test_array, value2);
    
    // Test getting valid indices
    void *retrieved1 = DYN_ARR_get(test_array, 0);
    void *retrieved2 = DYN_ARR_get(test_array, 1);
    
    TEST_ASSERT_EQUAL(value1, retrieved1);
    TEST_ASSERT_EQUAL(value2, retrieved2);
    TEST_ASSERT_EQUAL(100, *(int*)retrieved1);
    TEST_ASSERT_EQUAL(200, *(int*)retrieved2);
    
    // Test invalid indices
    TEST_ASSERT_NULL(DYN_ARR_get(test_array, 2));
    TEST_ASSERT_NULL(DYN_ARR_get(test_array, -1));
    
    // Test with function pointer
    void *retrieved3 = test_array->fp_get(test_array, 0);
    TEST_ASSERT_EQUAL(value1, retrieved3);
}

// Test get with NULL array
void test_get_null_array(void) {
    void *result = DYN_ARR_get(NULL, 0);
    TEST_ASSERT_NULL(result);
}

// Test macro get as pointer
void test_macro_get_as_pointer(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Add pointer data
    t_person *person1 = malloc(sizeof(t_person));
    person1->name = "Alice";
    person1->age = 25;
    
    t_person *person2 = malloc(sizeof(t_person));
    person2->name = "Bob";
    person2->age = 30;
    
    test_array->fp_add(test_array, person1);
    test_array->fp_add(test_array, person2);
    
    // Test get as pointer macro
    t_person *retrieved1 = DYN_ARR_get_as_ptr(test_array, 0, t_person *);
    t_person *retrieved2 = DYN_ARR_get_as_ptr(test_array, 1, t_person *);
    
    TEST_ASSERT_EQUAL(person1, retrieved1);
    TEST_ASSERT_EQUAL(person2, retrieved2);
    TEST_ASSERT_EQUAL_STRING("Alice", retrieved1->name);
    TEST_ASSERT_EQUAL(25, retrieved1->age);
    TEST_ASSERT_EQUAL_STRING("Bob", retrieved2->name);
    TEST_ASSERT_EQUAL(30, retrieved2->age);
}

// Test macro get as value
void test_macro_get_as_value(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Add values using macro
    DYN_ARR_add_by_value(test_array, 42);
    DYN_ARR_add_by_value(test_array, 3.14159);
    
    // Test get as value macro
    int int_value = DYN_ARR_get_as_type(test_array, 0, int);
    double double_value = DYN_ARR_get_as_type(test_array, 1, double);
    
    TEST_ASSERT_EQUAL(42, int_value);
    TEST_ASSERT_FLOAT_WITHIN(0.00001, 3.14159, double_value);
}

// Test destructor
void test_destructor(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Add some data
    DYN_ARR_add_by_value(test_array, 42);
    DYN_ARR_add_by_value(test_array, 84);
    
    TEST_ASSERT_EQUAL(2, test_array->ui8_size);
    
    // Test destructor
    int8_t result = DYN_ARR_destructor(test_array);
    TEST_ASSERT_EQUAL(0, result);
    
    // Set to NULL to prevent tearDown from trying to free again
    test_array = NULL;
}

// Test destructor with NULL
void test_destructor_null(void) {
    int8_t result = DYN_ARR_destructor(NULL);
    TEST_ASSERT_EQUAL(0, result);
}

// Test complex scenario with mixed data types
void test_mixed_data_types(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Add different types of data
    DYN_ARR_add_by_value(test_array, 42);           // int
    DYN_ARR_add_by_value(test_array, 3.14);         // double
    DYN_ARR_add_by_value(test_array, 'Z');          // char
    
    // Add pointer data
    t_person *person = malloc(sizeof(t_person));
    person->name = "Charlie";
    person->age = 35;
    DYN_ARR_add(test_array, person);
    
    TEST_ASSERT_EQUAL(4, test_array->ui8_size);
    
    // Verify all data
    TEST_ASSERT_EQUAL(42, DYN_ARR_get_as_type(test_array, 0, int));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.14, DYN_ARR_get_as_type(test_array, 1, double));
    TEST_ASSERT_EQUAL('Z', DYN_ARR_get_as_type(test_array, 2, char));
    
    t_person *retrieved_person = DYN_ARR_get_as_ptr(test_array, 3, t_person *);
    TEST_ASSERT_EQUAL_STRING("Charlie", retrieved_person->name);
    TEST_ASSERT_EQUAL(35, retrieved_person->age);
}

// Test array growth
void test_array_growth(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Add multiple elements to test growth
    for (int i = 0; i < 10; i++) {
        DYN_ARR_add_by_value(test_array, i);
        TEST_ASSERT_EQUAL(i + 1, test_array->ui8_size);
    }
    
    // Verify all values
    for (int i = 0; i < 10; i++) {
        int value = DYN_ARR_get_as_type(test_array, i, int);
        TEST_ASSERT_EQUAL(i, value);
    }
}

// Test edge cases
void test_edge_cases(void) {
    test_array = DYN_ARRAY_constructor();
    TEST_ASSERT_NOT_NULL(test_array);
    
    // Test getting from empty array
    TEST_ASSERT_NULL(DYN_ARR_get(test_array, 0));
    
    // Add one element
    DYN_ARR_add_by_value(test_array, 42);
    
    // Test boundary conditions
    TEST_ASSERT_NOT_NULL(DYN_ARR_get(test_array, 0));  // Valid
    TEST_ASSERT_NULL(DYN_ARR_get(test_array, 1));      // Out of bounds
    TEST_ASSERT_NULL(DYN_ARR_get(test_array, -1));     // Negative index
}

// Main test runner
int main(void) {
    UNITY_BEGIN();
    
    // Basic functionality tests
    RUN_TEST(test_constructor);
    RUN_TEST(test_add_by_pointer);
    RUN_TEST(test_add_by_pointer_null_array);
    RUN_TEST(test_macro_add_by_value);
    RUN_TEST(test_get);
    RUN_TEST(test_get_null_array);
    RUN_TEST(test_macro_get_as_pointer);
    RUN_TEST(test_macro_get_as_value);
    RUN_TEST(test_destructor);
    RUN_TEST(test_destructor_null);
    
    // Advanced tests
    RUN_TEST(test_mixed_data_types);
    RUN_TEST(test_array_growth);
    RUN_TEST(test_edge_cases);
    
    return UNITY_END();
}
