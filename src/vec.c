// To enable strdup, strndup when using strict standards (c99 for exampple)

#ifdef __linux__
#define _GNU_SOURCE
#endif

#include "vec.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_MSG(is_true, msg)      \
	if(is_true) {                 \
		fprintf(stderr, msg); \
		assert(!(is_true));   \
	}

#define CHECK_NULL_VEC(v) ASSERT_MSG(((v) == NULL), "The given vector is empty!")

vec_t *vec_create(int data_size, void (*data_del_func)(const void *data))
{
	vec_t *tmp = (vec_t *)malloc(sizeof(vec_t));
	if(tmp == NULL) return NULL;

	tmp->data_size	   = data_size;
	tmp->size	   = 1;
	tmp->count	   = 0;
	tmp->data	   = (void **)malloc(sizeof(void *) * tmp->size);
	tmp->data_del_func = data_del_func;

	return tmp;
}

void vec_destroy(vec_t **v)
{
	if(v == NULL) {
		fprintf(stderr, "The given vector pointer pointer is null!");
		assert(v != NULL);
	}

	CHECK_NULL_VEC(*v);

	vec_clear(*v);
	free((*v)->data);
	free(*v);

	*v = NULL;
}

void vec_add(vec_t *v, const void *data)
{
	CHECK_NULL_VEC(v);
	// Double the vector size if full
	if(v->count >= v->size) {
		v->size *= 2;
		v->data = (void **)realloc(v->data, sizeof(void *) * v->size);
	}

	if(v->data_size <= 0) {
		v->data[v->count] = strdup((const char *)data);
	} else {
		v->data[v->count] = malloc(v->data_size);
		memcpy(v->data[v->count], data, v->data_size);
	}
	v->count++;
}

const void *vec_get_data(const vec_t *v, int loc)
{
	CHECK_NULL_VEC(v);
	ASSERT_MSG(loc >= v->count, "Location entered is bigger than total elements in the vector");
	return v->data[loc];
}

void *vec_get_data_mut(const vec_t *v, int loc)
{
	CHECK_NULL_VEC(v);
	ASSERT_MSG(loc >= v->count, "Location entered is bigger than total elements in the vector");
	return v->data[loc];
}

void *vec_get_data_copy(const vec_t *v, int loc)
{
	CHECK_NULL_VEC(v);
	ASSERT_MSG(loc >= v->count, "Location entered is bigger than total elements in the vector");
	if(v->data_size <= 0) return strdup((const char *)vec_get_data(v, loc));
	void *tmp = malloc(v->data_size);
	memcpy(tmp, vec_get_data(v, loc), v->data_size);
	return tmp;
}

int vec_count(const vec_t *v)
{
	CHECK_NULL_VEC(v);
	return v->count;
}

void vec_del_at(vec_t *v, int loc)
{
	CHECK_NULL_VEC(v);
	ASSERT_MSG(loc >= v->count, "Location entered is bigger than total elements in the vector");
	if(v->data_size > 0 && v->data_del_func != NULL) v->data_del_func(v->data[loc]);
	free(v->data[loc]);
	for(int i = loc; i < v->count - 1; ++i) {
		v->data[i] = v->data[i + 1];
	}
	v->count--;

	if(v->count * 2 < v->size && v->size > 1) {
		v->size /= 2;
		v->data = (void **)realloc(v->data, sizeof(void *) * v->size);
	}
}

void vec_clear(const vec_t *v)
{
	CHECK_NULL_VEC(v);
	for(int i = 0; i < v->count; ++i) {
		if(v->data_size > 0 && v->data_del_func != NULL) v->data_del_func(v->data[i]);
		free(v->data[i]);
	}
}

void vec_sort(vec_t *v, int (*sorter)(const void *a, const void *b))
{
	CHECK_NULL_VEC(v);
	if(v->count <= 0) return;
	qsort(v->data, v->count, sizeof(v->data[0]), sorter);
}
