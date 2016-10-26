
#line 1 "version_sorter.rl"
/*
 *  version_sorter.c
 *  version_sorter
 *
 * Created with Ragel.
 *   ragel -C -G2 version_sorter.rl
 *
 *  Created by K. Adam Christensen on 10/10/09.
 *  Copyright 2009. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ruby.h>

#define MAX_COMP 16
#define MAX_ITEMS 512

#define min(a, b) ((a) < (b) ? (a) : (b))
typedef int compare_callback_t(const void *, const void *);

void Init_version_sorter(void);


#line 30 "version_sorter.c"
static const int parser_start = 8;
static const int parser_first_final = 8;
static const int parser_error = 0;

static const int parser_en_main = 8;


#line 29 "version_sorter.rl"


struct version_number {
	const char *original;
	VALUE rb_version;
	uint64_t num_flags;
	int32_t size;
	union version_comp {
		uint32_t number;
		struct strchunk {
			uint16_t offset;
			uint16_t len;
		} string;
	} comp[MAX_COMP];
};

static int
strchunk_cmp(const char *original_a, const struct strchunk *a,
		const char *original_b, const struct strchunk *b)
{
	size_t len = min(a->len, b->len);
	int cmp = memcmp(original_a + a->offset, original_b + b->offset, len);
	return cmp ? cmp : (int)(a->len - b->len);
}

static int
compare_version_number(const struct version_number *a,
		const struct version_number *b)
{
	int n, max_n = min(a->size, b->size);

	for (n = 0; n < max_n; ++n) {
		int num_a = (a->num_flags & (1ull << n)) != 0;
		int num_b = (b->num_flags & (1ull << n)) != 0;

		if (num_a == num_b) {
			const union version_comp *ca = &a->comp[n];
			const union version_comp *cb = &b->comp[n];
			int cmp = 0;

			if (num_a) {
				cmp = (int)ca->number - (int)cb->number;
			} else {
				cmp = strchunk_cmp(
						a->original, &ca->string,
						b->original, &cb->string);
			}

			if (cmp) return cmp;
		} else {
			return num_a ? 1 : -1;
		}
	}

	if (a->size < b->size)
		return (b->num_flags & (1ull << n)) ? -1 : 1;

	if (a->size > b->size)
		return (a->num_flags & (1ull << n)) ? 1 : -1;

	return 0;
}

static int
version_compare_cb(const void *a, const void *b)
{
	return compare_version_number(
		(const struct version_number *)a,
		(const struct version_number *)b);
}

static int
version_compare_cb_r(const void *a, const void *b)
{
	return -compare_version_number(
		(const struct version_number *)a,
		(const struct version_number *)b);
}

static struct version_number *
parse_version_number(
		struct version_number *version, const char *string, long len)
{
	uint32_t num_flags = 0x0;
	uint32_t number = 0;
	uint comp_n = 0, comp_alloc = 4;
	int overflown = 0;
	const char *p = string, *pe = string + len, *eof = pe;  // Ragel variables.
	int cs;
	const char *start = NULL;

	
#line 131 "version_sorter.c"
	{
	cs = parser_start;
	}

#line 136 "version_sorter.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 8:
	if ( (*p) == 45 )
		goto tr4;
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto tr2;
		} else if ( (*p) >= -62 )
			goto tr1;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr5;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr3;
	goto st1;
tr12:
#line 152 "version_sorter.rl"
	{
			version->comp[comp_n].string.offset = (uint16_t)(start - string);
			version->comp[comp_n].string.len = (uint16_t)(p - start);
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
	goto st1;
tr16:
#line 142 "version_sorter.rl"
	{
			if (overflown) {
				version->comp[comp_n].string.offset = (uint16_t)(start - string);
				version->comp[comp_n].string.len = (uint16_t)(p - start);
			} else {
				version->comp[comp_n].number = number;
				num_flags |= (1 << comp_n);
			}
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 194 "version_sorter.c"
	if ( (*p) == 45 )
		goto tr4;
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto tr2;
		} else if ( (*p) >= -62 )
			goto tr1;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr5;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr3;
	goto st1;
tr1:
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st2;
tr17:
#line 142 "version_sorter.rl"
	{
			if (overflown) {
				version->comp[comp_n].string.offset = (uint16_t)(start - string);
				version->comp[comp_n].string.len = (uint16_t)(p - start);
			} else {
				version->comp[comp_n].number = number;
				num_flags |= (1 << comp_n);
			}
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 251 "version_sorter.c"
	if ( (*p) == 45 )
		goto tr4;
	if ( (*p) < -16 ) {
		if ( (*p) < -62 ) {
			if ( (*p) <= -65 )
				goto st9;
		} else if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto tr2;
		} else
			goto tr1;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr5;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr3;
	goto st1;
tr4:
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st9;
tr14:
#line 152 "version_sorter.rl"
	{
			version->comp[comp_n].string.offset = (uint16_t)(start - string);
			version->comp[comp_n].string.len = (uint16_t)(p - start);
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st9;
tr20:
#line 142 "version_sorter.rl"
	{
			if (overflown) {
				version->comp[comp_n].string.offset = (uint16_t)(start - string);
				version->comp[comp_n].string.len = (uint16_t)(p - start);
			} else {
				version->comp[comp_n].number = number;
				num_flags |= (1 << comp_n);
			}
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 329 "version_sorter.c"
	if ( (*p) == 45 )
		goto tr14;
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -62 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr15;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st9;
		} else
			goto st9;
	} else
		goto st5;
	goto tr12;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) <= -65 )
		goto st9;
	goto st0;
st0:
cs = 0;
	goto _out;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) <= -65 )
		goto st3;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) <= -65 )
		goto st4;
	goto st0;
tr5:
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
#line 128 "version_sorter.rl"
	{
			number = 0;
			overflown = 0;
		}
#line 133 "version_sorter.rl"
	{
			if (!overflown) {
				uint32_t old_number = number;
				number = (10 * number) + (uint32_t)(*p - '0');
				if (number < old_number)
					overflown = 1;
			}
		}
	goto st10;
tr15:
#line 152 "version_sorter.rl"
	{
			version->comp[comp_n].string.offset = (uint16_t)(start - string);
			version->comp[comp_n].string.len = (uint16_t)(p - start);
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
#line 128 "version_sorter.rl"
	{
			number = 0;
			overflown = 0;
		}
#line 133 "version_sorter.rl"
	{
			if (!overflown) {
				uint32_t old_number = number;
				number = (10 * number) + (uint32_t)(*p - '0');
				if (number < old_number)
					overflown = 1;
			}
		}
	goto st10;
tr21:
#line 133 "version_sorter.rl"
	{
			if (!overflown) {
				uint32_t old_number = number;
				number = (10 * number) + (uint32_t)(*p - '0');
				if (number < old_number)
					overflown = 1;
			}
		}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 444 "version_sorter.c"
	if ( (*p) == 45 )
		goto tr20;
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto tr18;
		} else if ( (*p) >= -62 )
			goto tr17;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr21;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr20;
		} else
			goto tr20;
	} else
		goto tr19;
	goto tr16;
tr2:
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st6;
tr18:
#line 142 "version_sorter.rl"
	{
			if (overflown) {
				version->comp[comp_n].string.offset = (uint16_t)(start - string);
				version->comp[comp_n].string.len = (uint16_t)(p - start);
			} else {
				version->comp[comp_n].number = number;
				num_flags |= (1 << comp_n);
			}
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 501 "version_sorter.c"
	if ( (*p) == 45 )
		goto tr4;
	if ( (*p) < -16 ) {
		if ( (*p) < -62 ) {
			if ( (*p) <= -65 )
				goto st2;
		} else if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto tr2;
		} else
			goto tr1;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr5;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr3;
	goto st1;
tr3:
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st7;
tr19:
#line 142 "version_sorter.rl"
	{
			if (overflown) {
				version->comp[comp_n].string.offset = (uint16_t)(start - string);
				version->comp[comp_n].string.len = (uint16_t)(p - start);
			} else {
				version->comp[comp_n].number = number;
				num_flags |= (1 << comp_n);
			}
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
#line 121 "version_sorter.rl"
	{
			if (comp_n >= comp_alloc) {
				goto FINALIZE;
			}
			start = p;
		}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 561 "version_sorter.c"
	if ( (*p) == 45 )
		goto tr4;
	if ( (*p) < -16 ) {
		if ( (*p) < -62 ) {
			if ( (*p) <= -65 )
				goto st6;
		} else if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto tr2;
		} else
			goto tr1;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr5;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr3;
	goto st1;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 10: 
#line 142 "version_sorter.rl"
	{
			if (overflown) {
				version->comp[comp_n].string.offset = (uint16_t)(start - string);
				version->comp[comp_n].string.len = (uint16_t)(p - start);
			} else {
				version->comp[comp_n].number = number;
				num_flags |= (1 << comp_n);
			}
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
	break;
	case 9: 
#line 152 "version_sorter.rl"
	{
			version->comp[comp_n].string.offset = (uint16_t)(start - string);
			version->comp[comp_n].string.len = (uint16_t)(p - start);
		}
#line 157 "version_sorter.rl"
	{
			comp_n++;
		}
	break;
#line 627 "version_sorter.c"
	}
	}

	_out: {}
	}

#line 175 "version_sorter.rl"


FINALIZE:
	version->original = string;
	version->num_flags = num_flags;
	version->size = (int32_t)comp_n;

	return version;
}

static VALUE
rb_version_sort_1(VALUE rb_self, VALUE rb_version_array, compare_callback_t cmp)
{
	(void)rb_self;  // Unused.


	struct version_number *versions = NULL;
	struct version_number versions_stack[MAX_ITEMS];
	long length, i;
	VALUE *rb_version_ptr;

	Check_Type(rb_version_array, T_ARRAY);

	length = RARRAY_LEN(rb_version_array);
	if (!length)
		return rb_ary_new();

	if (length <= MAX_ITEMS) {
		versions = &versions_stack[0];
	} else {
		versions = xcalloc(length, sizeof(struct version_number));
	}

	for (i = 0; i < length; ++i) {
		VALUE rb_version, rb_version_string;

		rb_version = rb_ary_entry(rb_version_array, i);
		if (rb_block_given_p())
			rb_version_string = rb_yield(rb_version);
		else
			rb_version_string = rb_version;

		parse_version_number(
				&versions[i],
				RSTRING_PTR(rb_version_string),
				RSTRING_LEN(rb_version_string));
		versions[i].rb_version = rb_version;
	}

	qsort(versions, (size_t)length, sizeof(struct version_number), cmp);
	rb_version_ptr = RARRAY_PTR(rb_version_array);

	for (i = 0; i < length; ++i) {
		rb_version_ptr[i] = versions[i].rb_version;
	}
	if (length > MAX_ITEMS) {
		xfree(versions);
	}
	return rb_version_array;
}

static VALUE
rb_version_sort(VALUE rb_self, VALUE rb_versions)
{
	return rb_version_sort_1(rb_self, rb_ary_dup(rb_versions), version_compare_cb);
}

static VALUE
rb_version_sort_r(VALUE rb_self, VALUE rb_versions)
{
	return rb_version_sort_1(rb_self, rb_ary_dup(rb_versions), version_compare_cb_r);
}

static VALUE
rb_version_sort_bang(VALUE rb_self, VALUE rb_versions)
{
	return rb_version_sort_1(rb_self, rb_versions, version_compare_cb);
}

static VALUE
rb_version_sort_r_bang(VALUE rb_self, VALUE rb_versions)
{
	return rb_version_sort_1(rb_self, rb_versions, version_compare_cb_r);
}

void
Init_version_sorter(void)
{
	VALUE rb_mVersionSorter = rb_define_module("VersionSorter");
	rb_define_module_function(rb_mVersionSorter, "sort", rb_version_sort, 1);
	rb_define_module_function(rb_mVersionSorter, "rsort", rb_version_sort_r, 1);
	rb_define_module_function(rb_mVersionSorter, "sort!", rb_version_sort_bang, 1);
	rb_define_module_function(rb_mVersionSorter, "rsort!", rb_version_sort_r_bang, 1);
}
