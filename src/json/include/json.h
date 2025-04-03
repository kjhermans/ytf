/**
 * This file is part of Sarthaka, a structured data binary codec.

Copyright (c) 2023, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Kees-Jan Hermans BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 * \file
 * \brief
 */

#ifndef _JSON_H_
#define _JSON_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct json json_t;

#include <map.h>
#include <array.h>

typedef struct
{
  unsigned                      wide;
  unsigned                      length;
  union {
    uint8_t*                      bytes;
    uint32_t*                     unicode;
  }                             value;
}
json_string_t;

MAKE_ARRAY_HEADER(json_t*, json_array_)
MAKE_MAP_HEADER(json_string_t, json_t*, json_hashtable_)

typedef union {
  int64_t                       boolint;
  double                        fraction;
  json_string_t                 string;
  json_array_t                  array;
  json_hashtable_t              hashtable;
}
json_value_t;

struct json
{
  unsigned                      type;
#define JSON_TYPE_NULL          0
#define JSON_TYPE_BOOLEAN       1
#define JSON_TYPE_INTEGER       2
#define JSON_TYPE_FLOAT         3
#define JSON_TYPE_STRING        4
#define JSON_TYPE_ARRAY         5
#define JSON_TYPE_HASHTABLE     6
  json_value_t                  value;
};

#define JSON_OBJECT             16

/**
 * Default is zero, which means: silently do not replace hashtable entries
 * that have the same key. Setting it to non-zero means: silenty replace
 * hashtable entries that have the same key.
 */
extern
void json_set_replacepolicy
  (int r);

extern
int json_string_compare
  (json_string_t* s1, json_string_t* s2);

extern
int json_string_compare2
  (json_string_t* s1, char* str);

extern
json_string_t json_string_dup
  (json_string_t* tgt, json_string_t* src);

extern
json_string_t json_string_copy
  (json_string_t* tgt, void* mem, unsigned size);

extern
json_t* json_parse
  (char* string);

extern
void json_debug
  (json_t* json);

extern
void json_fdebug
  (FILE* f, json_t* json);

extern
json_t* json_new
  (unsigned type);

#define JSON_NEW_NULL json_new

#define JSON_NEW_STRING(_str) ({ \
  json_t* _res = json_new(JSON_TYPE_STRING); \
  _res->value.string.wide = 0; \
  _res->value.string.length = strlen(_str); \
  _res->value.string.value.bytes = (uint8_t*)strdup(_str); \
  _res; \
})

#define JSON_NEW_BOOLEAN(_boo) ({ \
  json_t* _res = json_new(JSON_TYPE_BOOLEAN); \
  _res->value.boolint = _boo; \
  _res; \
})

#define JSON_NEW_INTEGER(_int) ({ \
  json_t* _res = json_new(JSON_TYPE_INTEGER); \
  _res->value.boolint = _int; \
  _res; \
})

#define JSON_NEW_FLOAT(_flt) ({ \
  json_t* _res = json_new(JSON_TYPE_FLOAT); \
  _res->value.fraction = _flt; \
  _res; \
})

extern
int json_array_setvalue_atindex
  (json_t* array, unsigned i, json_t* element, json_t** original);

extern
int json_array_add_element
  (json_t* array, json_t* element);

extern
int json_hashtable_add_entry
  (json_t* hash, char* key, json_t* value, json_t** original);

/* Fill using a sequence of nentries long of char* key, unsigned type, value
   (no arg for null values).
   Note that integers will have a sizeof(int64_t) whereas booleans will
   have a sizeof(int).
 */
extern
int json_hashtable_add_entries
  (json_t* hash, unsigned nentries, ...);

extern
void json_free
  (json_t* json);

#define JSON_QUERY_KEY          1
#define JSON_QUERY_INDEX        2

extern
const json_t* json_iterate
  (const json_t* json, unsigned nlist, ...);

extern
const char* json_query
  (const json_t* json, unsigned nlist, ...);

extern
int json_query_as_boolean
  (const json_t* json, unsigned nlist, ...);

extern
int64_t json_query_as_integer
  (const json_t* json, unsigned nlist, ...);

#define json_string_sparse json_string

extern
char* json_string
  (json_t* json);

/**
 * Makes a deep copy of the given json structure.
 */
extern
json_t* json_dup
  (const json_t* json);

#endif
