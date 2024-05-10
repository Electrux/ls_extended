/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef VEC_H
#define VEC_H

typedef struct
{
	int count;
	int size;
	int data_size;
	void **data;
	void (*data_del_func)(const void *data);
} vec_t;

vec_t *vec_create(int data_size, void (*data_del_func)(const void *data));
void vec_destroy(vec_t **v);

void vec_add(vec_t *v, const void *data);

const void *vec_get_data(const vec_t *v, int loc);
void *vec_get_data_mut(const vec_t *v, int loc);
void *vec_get_data_copy(const vec_t *v, int loc);

int vec_count(const vec_t *v);

void vec_del_at(vec_t *v, int loc);

void vec_clear(const vec_t *v);

void vec_sort(vec_t *v, int (*sorter)(const void *a, const void *b));

#endif // VEC_H
