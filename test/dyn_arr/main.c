#include "stdio.h"
#include "dynamic_array.h"

int main(void) {
	t_dyn_arr tdynarr_array_int = DYN_ARRAY_constructor();
	dyn_arr_add(&tdynarr_array_int, 1);
	dyn_arr_add(&tdynarr_array_int, 2);
	dyn_arr_add(&tdynarr_array_int, 3);
	dyn_arr_add(&tdynarr_array_int, 4);

	printf("%d %d %d %d\n", 
			*(int*)DYN_ARR_get(&tdynarr_array_int, 0),
			*(int*)DYN_ARR_get(&tdynarr_array_int, 1),
			*(int*)DYN_ARR_get(&tdynarr_array_int, 2),
			*(int*)DYN_ARR_get(&tdynarr_array_int, 3)
			);

	int a = 5;
	int b = 6;
	double *c = malloc(sizeof(double));
	double *d = malloc(sizeof(double));
  *c = 5.773;
  *d = 12.568;
	tdynarr_array_int.fp_add(&tdynarr_array_int, &a);
	tdynarr_array_int.fp_add(&tdynarr_array_int, &b);
	tdynarr_array_int.fp_add(&tdynarr_array_int, c);
	tdynarr_array_int.fp_add(&tdynarr_array_int, d);

	printf("%d %d %f %f\n", 
			*(int*)tdynarr_array_int.fp_get(&tdynarr_array_int, 4),
			*(int*)tdynarr_array_int.fp_get(&tdynarr_array_int, 5),
			*(double*)tdynarr_array_int.fp_get(&tdynarr_array_int, 6),
			*(double*)tdynarr_array_int.fp_get(&tdynarr_array_int, 7)
			);

	return 0;
}
