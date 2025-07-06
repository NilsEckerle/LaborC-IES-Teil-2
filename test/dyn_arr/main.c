#include "stdio.h"
#include "dynamic_array.h"
#include <stdint.h>
#include <stdlib.h>

int main(void) {
	t_dyn_arr *tdynarr_array_int = DYN_ARRAY_constructor();
	DYN_ARR_add_by_value(tdynarr_array_int, 1);
	DYN_ARR_add_by_value(tdynarr_array_int, 2);
	DYN_ARR_add_by_value(tdynarr_array_int, 3);
	DYN_ARR_add_by_value(tdynarr_array_int, 4);

	printf("%d %d %d %d\n", 
			*(int*)DYN_ARR_get(tdynarr_array_int, 0),
			*(int*)DYN_ARR_get(tdynarr_array_int, 1),
			DYN_ARR_get_as_type(tdynarr_array_int, 2, int),
			DYN_ARR_get_as_type(tdynarr_array_int, 3, int)
			);

	int a = 5;
	int b = 6;
	double *c = malloc(sizeof(double));
	double *d = malloc(sizeof(double));
	*c = 5.773;
	*d = 12.568;
	tdynarr_array_int->fp_add(tdynarr_array_int, &a);
	tdynarr_array_int->fp_add(tdynarr_array_int, &b);
	tdynarr_array_int->fp_add(tdynarr_array_int, c);
	tdynarr_array_int->fp_add(tdynarr_array_int, d);

	printf("%d %d %f %f\n", 
			*(int*)tdynarr_array_int->fp_get(tdynarr_array_int, 4),
			*(int*)tdynarr_array_int->fp_get(tdynarr_array_int, 5),
			*(double*)tdynarr_array_int->fp_get(tdynarr_array_int, 6),
			*(double*)tdynarr_array_int->fp_get(tdynarr_array_int, 7)
			);

	typedef struct person {
		char *name;
		uint8_t age;
	} t_person;

	t_person *p1 = malloc(sizeof(t_person));
	p1->name = "name1";
	p1->age = 18;
	t_person *p2 = malloc(sizeof(t_person));
	p2->name = "name2";
	p2->age = 19;
	t_person *p3 = malloc(sizeof(t_person));
	p3->name = "name3";
	p3->age = 20;
	t_person *p4 = malloc(sizeof(t_person));
	p4->name = "name4";
	p4->age = 21;

	t_dyn_arr *tdynarr_array_t_person = DYN_ARRAY_constructor();
	tdynarr_array_t_person->fp_add(tdynarr_array_t_person, &p1);
	// tdynarr_array_t_person->fp_add(tdynarr_array_t_person, p2);
	// tdynarr_array_t_person->fp_add(tdynarr_array_t_person, p3);
	// tdynarr_array_t_person->fp_add(tdynarr_array_t_person, p4);
	// DYN_ARR_add_by_value(tdynarr_array_t_person, p1);
	DYN_ARR_add_by_value(tdynarr_array_t_person, p2);
	DYN_ARR_add_by_value(tdynarr_array_t_person, p3);
	DYN_ARR_add_by_value(tdynarr_array_t_person, p4);

	printf("%s %d\n%s %d\n%s %d\n%s %d\n", 
			(*DYN_ARR_get_as_ptr(tdynarr_array_t_person, 0, t_person **))->name,
			DYN_ARR_get_as_type(tdynarr_array_t_person, 0, t_person *)->age,
			(*(t_person**)DYN_ARR_get(tdynarr_array_t_person, 1))->name,
			(*(t_person**)DYN_ARR_get(tdynarr_array_t_person, 1))->age,
			DYN_ARR_get_as_type(tdynarr_array_t_person, 2, t_person *)->name,
			DYN_ARR_get_as_type(tdynarr_array_t_person, 2, t_person *)->age,
			DYN_ARR_get_as_type(tdynarr_array_t_person, 3, t_person *)->name,
			DYN_ARR_get_as_type(tdynarr_array_t_person, 3, t_person *)->age
			);

	return 0;
}
