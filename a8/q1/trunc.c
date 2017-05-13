#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "list.h"
#include <string.h>

void strToNum(element_t* output_i, element_t input_i) {
	char* input = (char*) input_i;
	intptr_t* output = (intptr_t*) output_i;
	char** input_t = &input;
	*output = strtol(input, input_t, 10);
	if (*input_t != NULL && *output == 0){
		*output = -1;
	}
}

void numRv(element_t* oi, element_t e1i, element_t e2i) {
	char* e1 = e1i;
	intptr_t e2 = (intptr_t) e2i;
	char** o = (char**) oi;
	if (e2 < 0) {
		*o = e1;
	} else {
		*o = 0;
	}
}

int isPositive(element_t ei) {
	intptr_t element = (intptr_t) ei;
	if (element != -1) {
		return 1;
	} else {
		return 0;
	}
}

int strRv(element_t ei) {
	char* element = ei;
	if (element != NULL) {
		return 1;
	} else {
		return 0;
	}
}

void truncString(element_t* oi, element_t e1i, element_t e2i) {
	char** element = (char**) oi;
	char* e1 = e1i;
	int e2 = (int) e2i;
	*element = strdup(e1);
	if (strlen(*element) > e2) {
		(*element)[e2] = '\0';
	}
}


void maxValue(element_t* av, element_t bv, element_t cv){
	intptr_t*a = (intptr_t*) av;
	intptr_t b = (intptr_t) bv;
	intptr_t c = (intptr_t) cv;
	if (c > b) {
		*a = c;
	}
}

void print_string (element_t ei) {
	char* e = ei;
	printf ("%s\n", e);
}

void print_num (element_t ei) {
	intptr_t e = (intptr_t) ei;
	printf ("%ld\n", e);
}

int main(int argc, char** argv) {

	struct list* l0 = list_create();
	for (int i = 1; i < argc; i++) {
		list_append(l0, argv[i]);
	}

	// l1 is the list of num
	struct list* l1 = list_create();
	list_map1(strToNum, l1, l0);

	struct list* l2 = list_create();
	list_map2(numRv, l2, l0, l1);

	struct list* l3 = list_create();
	list_filter(isPositive, l3, l1);

	struct list* l4 = list_create();
	list_filter(strRv, l4, l2);

	struct list* l5 = list_create();
	list_map2(truncString, l5, l4, l3);
	list_foreach(print_string, l5);
	list_foreach(free, l5);

	intptr_t max = 0;
	list_foldl(maxValue, (element_t*) &max, l3);
	printf("%ld\n", max);

	list_destroy(l0);
	list_destroy(l1);
	list_destroy(l2);
	list_destroy(l3);
	list_destroy(l4);
	list_destroy(l5);
}