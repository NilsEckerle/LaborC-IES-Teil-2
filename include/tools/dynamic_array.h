/**
 * @file dynamic_array.h
 * @brief dynamic array data structure
 * @note This has a scaling of n -> n+1 so this is inefficient in some usecases.
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdint.h>

/**
 * @brief Dynamic array structure for storing data
 * 
 * This structure implements a resizable array that can store pointers to
 * any type of data. It automatically manages memory allocation and provides
 * type-safe access through macros.
 * 
 * @warning This struct doesn't work with char* strings directly!
 *          Use the provided macros for proper type handling.
 * 
 * @note The array stores void pointers, allowing storage of any data type.
 *       Size is limited to 255 elements due to uint8_t size field.
 */
typedef struct dynamic_array {
  void **vpp_data_array; /**< Array of void pointers to stored data */
  uint8_t ui8_size;      /**< Current number of elements in array */
} t_dyn_arr;

/**
 * @brief Creates and initializes a new dynamic array
 * 
 * Allocates memory for a new dynamic array structure and initializes
 * it with default values (empty array).
 * 
 * @return Pointer to newly created dynamic array on success, NULL on failure
 * 
 * @note The returned array must be freed using DYN_ARR_destructor()
 *       to prevent memory leaks.
 */
t_dyn_arr *DYN_ARRAY_constructor();

/**
 * @brief Adds a data pointer to the dynamic array
 * 
 * Resizes the array and adds the provided data pointer to the end of the array.
 * The array automatically grows as needed.
 * 
 * @param tp_array Pointer to the dynamic array structure
 * @param vp_data Pointer to the data to be added
 * @return 0 on success, positive value on error
 * 
 * @note This function stores the pointer itself, not a copy of the data.
 *       The caller is responsible for ensuring the pointed-to data remains
 *       valid for the lifetime of the array. Use the DYN_ARR_add_by_value
 *       macro to automatically handle copying data to the heap.
 */
int8_t DYN_ARR_add(t_dyn_arr *tp_array, void *vp_data);

/**
 * @brief Retrieves a data pointer from the dynamic array
 * 
 * Returns the void pointer stored at the specified index without
 * removing it from the array.
 * 
 * @param tp_array Pointer to the dynamic array structure
 * @param ui8_index Index of the element to retrieve (0-based)
 * @return Pointer to the data at the specified index, NULL on error or invalid index
 * 
 * @note Use the type-safe macros for proper type casting.
 */
void *DYN_ARR_get(t_dyn_arr *tp_array, uint8_t ui8_index);

/**
 * @brief Destroys the dynamic array and frees all associated memory
 * 
 * Recursively frees all stored data elements, the data array, and the
 * dynamic array structure itself. Sets the pointer to NULL to prevent
 * use-after-free errors.
 * 
 * @param tpp_array Pointer to pointer to the dynamic array structure
 * @return 0 on success (always succeeds)
 * 
 * @note This function assumes all stored data was allocated with malloc()
 *       and will free each element. Do not use with stack-allocated data
 *       or data allocated with other methods.
 */
int8_t DYN_ARR_destructor(t_dyn_arr **tpp_array);

/**
 * @brief Adds data by value to the dynamic array
 * 
 * This macro creates a heap-allocated copy of the provided data and adds
 * it to the dynamic array. The original data can be safely modified or
 * freed after this operation.
 * 
 * @param tp_array Pointer to the dynamic array structure
 * @param t_data The data value to be copied and added (any type)
 * 
 * @note This macro uses typeof() which is a GCC extension. It allocates
 *       memory for a copy of the data, so the destructor will properly
 *       free this memory.
 * 
 * @example
 *       int value = 42;
 *       DYN_ARR_add_by_value(array, value);  // Copies value to heap
 */
#define DYN_ARR_add_by_value(tp_array, t_data)      \
  do {                                              \
    void *vp_data = malloc(sizeof(typeof(t_data))); \
    typeof(t_data) tmp = (t_data);                  \
    memcpy(vp_data, &tmp, sizeof(typeof(t_data)));  \
    DYN_ARR_add(tp_array, vp_data);                 \
  } while (0)

/**
 * @brief Retrieves data from array and casts to specified type
 * 
 * This macro retrieves a data pointer from the array and dereferences it
 * as the specified type. Use this when the stored data was added by value
 * or when you need the actual data value.
 * 
 * @param tp_array Pointer to the dynamic array structure
 * @param ui8_index Index of the element to retrieve (0-based)
 * @param type The type to cast the retrieved data to
 * @return The data value cast to the specified type
 * 
 * @note This macro dereferences the pointer, so it returns the actual
 *       data value, not a pointer to it.
 * 
 * @example
 *       int value = DYN_ARR_get_as_type(array, 0, int);
 */
#define DYN_ARR_get_as_type(tp_array, ui8_index, type) (*(type *)DYN_ARR_get(tp_array, ui8_index))

/**
 * @brief Retrieves data from array and casts to specified type pointer
 * 
 * This macro retrieves a data pointer from the array and casts it to
 * the specified pointer type. Use this when the stored data is itself
 * a pointer or when you need a pointer to the stored data.
 * 
 * @param tp_array Pointer to the dynamic array structure
 * @param ui8_index Index of the element to retrieve (0-based)
 * @param type The pointer type to cast the retrieved pointer to
 * @return The data pointer cast to the specified type
 * 
 * @note This macro does not dereference the pointer, so it returns
 *       a pointer of the specified type.
 * 
 * @example
 *       char *str = DYN_ARR_get_as_ptr(array, 0, char*);
 */
#define DYN_ARR_get_as_ptr(tp_array, ui8_index, type) ((type)DYN_ARR_get(tp_array, ui8_index))

#endif
