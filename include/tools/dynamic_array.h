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

	int8_t (*fp_add)(struct dynamic_array* tdynarrp_array, void *vp_data);
	void *(*fp_get)(struct dynamic_array* tdynarrp_array, int8_t ui8_index);
	int8_t (*fp_dtor)(struct dynamic_array* tdynarrp_array);
} t_dyn_arr;

int8_t _DYN_ARR_add(t_dyn_arr *tdynarrp_array, void *vp_data);
void *DYN_ARR_get(t_dyn_arr *tdynarrp_array, int8_t ui8_index);
int8_t DYN_ARR_deconstructor(t_dyn_arr *tdynarrp_array);

#define dyn_arr_add(tdynarrp_array, t_data) do { \
	void *vp_data = malloc(sizeof(typeof(t_data))); \
	typeof(t_data) tmp = (t_data); \
	memcpy(vp_data, &tmp, sizeof(typeof(t_data))); \
	_DYN_ARR_add(tdynarrp_array, vp_data); \
} while (0)

#define dyn_arr_get_as_type(tdynarrp_array, ui8_index, type) \
	(*(type *)dyn_arr_get(tdynarrp_array, ui8_index))


static inline t_dyn_arr DYN_ARRAY_constructor() {
	return (t_dyn_arr) {
		.vpp_data_array = NULL,
		.ui8_size = 0,
    .fp_add = _DYN_ARR_add,
		.fp_get = DYN_ARR_get,
		.fp_dtor = DYN_ARR_deconstructor
	};
}

#endif
