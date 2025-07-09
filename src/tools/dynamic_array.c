#include "tools/dynamic_array.h"
#include <stdlib.h>
#include <string.h>

// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"

int8_t _DYN_ARR_add(t_dyn_arr *tp_array, void *vp_data) {
	if (NULL == tp_array) { // invalid parameter
		WARNING("[_DYN_ARR_add] tp_array is NULL!\n");
		return 1;
	}

	// size up tp_array
	uint8_t ui8_old_size = tp_array->ui8_size;
	void **vpp_old_data_array = tp_array->vpp_data_array;

	tp_array->ui8_size++;

	tp_array->vpp_data_array = malloc(sizeof(void *) * tp_array->ui8_size);
	if (tp_array->vpp_data_array == NULL) { // malloc failed
		FATAL("[_DYN_ARR_add] malloc failed!\n");
		tp_array->ui8_size = ui8_old_size;
		tp_array->vpp_data_array = vpp_old_data_array;
		return 2;
	}

	// copy old data
	if (ui8_old_size > 0) { // Copy old data only if there was any
		memcpy(tp_array->vpp_data_array, vpp_old_data_array, sizeof(void *) * ui8_old_size);
		free(vpp_old_data_array);
	}

	// add new data
	tp_array->vpp_data_array[tp_array->ui8_size - 1] = vp_data;

	INFO("[_DYN_ARR_add] added element '%p' to dyn_arr.\n", vp_data);

	return 0;
}

void *DYN_ARR_get(t_dyn_arr *tp_array, uint8_t ui8_index) {
	if (NULL == tp_array) {
		return NULL;
	}
	if (tp_array->ui8_size <= ui8_index) {
		return NULL;
	}
	return tp_array->vpp_data_array[ui8_index];
}

int8_t DYN_ARR_destructor(t_dyn_arr **tpp_array) {
	if (NULL == tpp_array) {
		return 0;
	}

	if (NULL != (*tpp_array)) {
		if (NULL != (*tpp_array)->vpp_data_array) {
			if ((*tpp_array)->ui8_size > 0) {
				for (int i = 0; i < (*tpp_array)->ui8_size; i++) { // for each element
					if (NULL != (*tpp_array)->vpp_data_array[i]) {
						free((*tpp_array)->vpp_data_array[i]); // free element
						(*tpp_array)->vpp_data_array[i] = NULL;
					}
				}
			}
			free((*tpp_array)->vpp_data_array); // free array (void**)
			(*tpp_array)->vpp_data_array = NULL;
		}
		free((*tpp_array)); // free dyn_arr structure
		(*tpp_array) = NULL;
	}

	return 0;
}

t_dyn_arr *DYN_ARRAY_constructor() {
	t_dyn_arr *tp_arr = malloc(sizeof(t_dyn_arr));
	if (NULL == tp_arr) {
		WARNING("[DYN_ARRAY_constructor] malloc failed!\n");
	}

	tp_arr->vpp_data_array = NULL;
	tp_arr->ui8_size = 0;
	tp_arr->fp_add = _DYN_ARR_add;
	tp_arr->fp_get = DYN_ARR_get;
	tp_arr->fp_destructor = DYN_ARR_destructor;

	return tp_arr;
}
