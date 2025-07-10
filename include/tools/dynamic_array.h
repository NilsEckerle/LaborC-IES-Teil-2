#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief this struct doesn't work with char*!!!
 */
typedef struct dynamic_array {
	void **vpp_data_array;
	uint8_t ui8_size;
} t_dyn_arr;

t_dyn_arr *DYN_ARRAY_constructor();
int8_t DYN_ARR_add(t_dyn_arr *tp_array, void *vp_data);
void *DYN_ARR_get(t_dyn_arr *tp_array, uint8_t ui8_index);
int8_t DYN_ARR_destructor(t_dyn_arr **tpp_array);

#define DYN_ARR_add_by_value(tp_array, t_data) do { \
	void *vp_data = malloc(sizeof(typeof(t_data))); \
	typeof(t_data) tmp = (t_data); \
	memcpy(vp_data, &tmp, sizeof(typeof(t_data))); \
	DYN_ARR_add(tp_array, vp_data); \
} while (0)

#define DYN_ARR_get_as_type(tp_array, ui8_index, type) \
	(*(type *)DYN_ARR_get(tp_array, ui8_index))

#define DYN_ARR_get_as_ptr(tp_array, ui8_index, type) \
	((type)DYN_ARR_get(tp_array, ui8_index))

#endif
