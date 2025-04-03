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

#include <string.h>
#include <json.h>

#include <gpeg/lib/gpeg/gpeg.h>
#include <gpeg/lib/engine/gpege.h>

#undef MAP_EQUALS
#define MAP_EQUALS(a,b) (json_string_compare(&a, &b) == 0)

MAKE_ARRAY_CODE(json_t*, json_array_)
MAKE_MAP_CODE(json_string_t, json_t*, json_hashtable_)

#include "json_bytecode.h"
#include "json_slotmap.h"

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

#include "json.h"

static
int parser_init = 0;

static
gpege_t parser;

static
int pathparser_init = 0;

static
gpege_t pathparser;

static
int replacepolicy = 0; // do not replace subsequent duplicate hashtable entries.

void json_set_replacepolicy
  (int r)
{
  replacepolicy = r;
}

/**
 * Like strcmp
 */
int json_string_compare
  (json_string_t* s1, json_string_t* s2)
{
  unsigned smallestlen = (s1->length > s2->length) ? s2->length : s1->length;

  if (s1->length == 0) {
    if (s2->length) {
      return 1;
    } else {
      return 0;
    }
  } else if (s2->length == 0) {
    if (s1->length) {
      return -1;
    } else {
      return 0;
    }
  }
  if (!(s1->wide) && !(s2->wide)) {
    return strcmp((char*)(s1->value.bytes), (char*)(s2->value.bytes));
  }
  for (unsigned i=0; i < smallestlen; i++) {
    unsigned c1 = (s1->wide ? s1->value.unicode[ i ] : s1->value.bytes[ i ]);
    unsigned c2 = (s2->wide ? s2->value.unicode[ i ] : s2->value.bytes[ i ]);
    if (c1 < c2) {
      return -1;
    } else if (c1 > c2) {
      return 1;
    }
  }
  if (s1->length == s2->length) {
    return 0;
  } else if (s1->length > s2->length) {
    return 1;
  } else {
    return -1;
  }
}

int json_string_compare2
  (json_string_t* s1, char* str)
{
  json_string_t s2 = {
    .wide = 0,
    .length = (str ? strlen(str) : 0),
    .value.bytes = (uint8_t*)str
  };

  return json_string_compare(s1, &s2);
}

json_string_t json_string_dup
  (json_string_t* tgt, json_string_t* src)
{
  json_string_t res = *src;
  if (res.wide) {
//..
  } else {
    res.value.bytes = calloc(res.length + 1, 1);
    memcpy(res.value.bytes, src->value.bytes, res.length);
  }
  if (tgt) { *tgt = res; }
  return res;
}

json_string_t json_string_copy
  (json_string_t* tgt, void* mem, unsigned size)
{
  json_string_t res = { .length = size };

  res.value.bytes = calloc(res.length + 1, 1);
  memcpy(res.value.bytes, mem, size);
  if (tgt) { *tgt = res; }
  return res;
}

static
json_t* json_reconstruct
  (gpeg_capture_t* obj);

static
unsigned json_charval
  (gpeg_capture_t* resobj)
{
  unsigned char* str = (unsigned char*)(resobj->data.data);

  if (resobj->data.size == 1 && str[ 0 ] < 128) {
    return (unsigned)(str[ 0 ]);
  } else {
    if (0 == strcmp((char*)(resobj->data.data), "\\n")) {
      return (unsigned)'\n';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\r")) {
      return (unsigned)'\r';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\t")) {
      return (unsigned)'\t';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\b")) {
      return (unsigned)'\b';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\f")) {
      return (unsigned)'\f';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\/")) {
      return (unsigned)'/';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\\"")) {
      return (unsigned)'"';
    } else if (0 == strcmp((char*)(resobj->data.data), "\\\\")) {
      return (unsigned)'\\';
    } else if (resobj->data.data[ 0 ] == '\\' && resobj->data.data[ 1 ] == 'u') {
      //.. four hex characters at string offsets 2,3,4,5
    }
  }
  return 0;
}

static
json_string_t json_reconstruct_jsonstring
  (gpeg_capture_t* obj)
{
  json_string_t result = { 0 };

  for (unsigned i=0; i < obj->children.count; i++) {
    unsigned charval = json_charval(&(obj->children.list[ i ]));
    if (charval > 127) {
      result.wide = 1;
    }
  }
  result.length = obj->children.count;
  if (result.wide) {
    result.value.unicode = calloc(4, obj->children.count + 1);
  } else {
    result.value.bytes = calloc(1, obj->children.count + 1);
  }
  for (unsigned i=0; i < obj->children.count; i++) {
    unsigned charval = json_charval(&(obj->children.list[ i ]));
    if (result.wide) {
      result.value.unicode[ i ] = charval;
    } else {
      result.value.bytes[ i ] = charval;
    }
  }
  return result;
}

static
json_t* json_reconstruct_hashtable
  (gpeg_capture_t* obj)
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_HASHTABLE;
  for (unsigned i=0; i < obj->children.count; i++) {
    json_string_t key = json_reconstruct_jsonstring(&(obj->children.list[ i ].children.list[ 0 ]));
    json_t* value = json_reconstruct(&(obj->children.list[ i ].children.list[ 1 ]));
    int replace = json_hashtable_put(&(result->value.hashtable), key, value);
    if (replace) {
      if (replacepolicy) {
        json_t* oldvalue;
        json_hashtable_del(&(result->value.hashtable), key, &oldvalue);
        json_free(oldvalue);
        json_hashtable_put(&(result->value.hashtable), key, value);
      }
    }
  }
  return result;
}

static
json_t* json_reconstruct_array
  (gpeg_capture_t* obj)
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_ARRAY;
  for (unsigned i=0; i < obj->children.count; i++) {
    json_t* value = json_reconstruct(&(obj->children.list[ i ]));
    json_array_push(&(result->value.array), value);
  }
  return result;
}

static
json_t* json_reconstruct_string
  (gpeg_capture_t* obj)
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_STRING;
  result->value.string = json_reconstruct_jsonstring(obj);
  return result;
}

static
json_t* json_reconstruct_int
  (gpeg_capture_t* obj)
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_INTEGER;
  result->value.boolint = strtoll((char*)(obj->data.data), 0, 10);
  return result;
}

static
json_t* json_reconstruct_float
  (gpeg_capture_t* obj)
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_FLOAT;
  result->value.fraction = strtod((char*)(obj->data.data), 0);
  return result;
}

static
json_t* json_reconstruct_boolean
  (gpeg_capture_t* obj)
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_BOOLEAN;
  result->value.boolint = (0 == strcmp((char*)(obj->data.data), "true")) ? 1 : 0;
  return result;
}

static
json_t* json_reconstruct_null
  ()
{
  json_t* result = calloc(1, sizeof(json_t));

  result->type = JSON_TYPE_NULL;
  return result;
}

static
json_t* json_reconstruct
  (gpeg_capture_t* obj)
{
  switch (obj->type) {
  case 1: // SLOTMAP_HASH_CBOPENOPTHASHELTSCBCLOSE 
    return json_reconstruct_hashtable(obj);
  case 3: // SLOTMAP_ARRAY_ABOPENOPTARRAYELTSABCLOSE
    return json_reconstruct_array(obj);
  case 5: // SLOTMAP_STRING_NRTVUTFCHAR
    return json_reconstruct_string(obj);
  case 7: // SLOTMAP_INT_
    return json_reconstruct_int(obj);
  case 8: // SLOTMAP_FLOAT_
    return json_reconstruct_float(obj);
  case 9: // SLOTMAP_BOOL_TRUEFALSE
    return json_reconstruct_boolean(obj);
  case 10: // SLOTMAP_NULL_NULL
    return json_reconstruct_null();
  default:
    fprintf(stderr, "UNKNOWN TOKEN TYPE %u\n", obj->type);
    abort();
  }
}

json_t* json_new
  (unsigned type)
{
  json_t* result = calloc(1, sizeof(json_t));
  result->type = type;
  return result;
}

json_t* json_parse
  (char* string)
{
  gpege_ec_t result = { 0 };
  gpeg_capturelist_t resobj = { 0 };
  vec_t input = { (unsigned char*)string, strlen(string) };
  GPEG_ERR_T e;

  result.input = &input;
  if (parser_init == 0) {
    gpege_init(&parser);
    parser.bytecode.data = json_byc;
    parser.bytecode.size = json_byc_len;
    parser_init = 1;
  }
  e = gpege_run(&parser, &result);
  if (e.code) {
    gpege_ec_free(&result);
    return NULL;
  }
  e = gpege_actions2captures(&input, &(result.actions), &resobj);
  if (e.code) {
    gpege_ec_free(&result);
    return NULL;
  }

  json_t* json = json_reconstruct(&(resobj.list[ 0 ]));
  gpege_ec_free(&result);
  gpeg_capturelist_free(&resobj);
  return json;
}

int json_array_setvalue_atindex
  (json_t* array, unsigned i, json_t* element, json_t** original)
{
  if (NULL == array) {
    return ~0;
  }
  if (NULL == element) {
    element = json_new(JSON_TYPE_NULL);
  }
  if (array->type == JSON_TYPE_ARRAY) {
    while (i >= array->value.array.count) {
      json_t* nll = json_new(JSON_TYPE_NULL);
      json_array_push(&(array->value.array), nll);
    }
    if (i < array->value.array.count) {
      if (original) {
        *original = array->value.array.list[ i ];
      }
      array->value.array.list[ i ] = element;
      return 0;
    } else {
      if (original) {
        *original = NULL;
      }
      json_array_push(&(array->value.array), element);
      return 0;
    }
  } else {
    return ~0;
  }
}

int json_array_add_element
  (json_t* array, json_t* element)
{
  if (NULL == array) {
    return ~0;
  }
  if (NULL == element)
  {
    return ~0;
  }
  json_array_push(&(array->value.array), element);
  return 0;
}

int json_hashtable_add_entry
  (json_t* hash, char* key, json_t* value, json_t** original)
{
  if (NULL == hash) {
    return ~0;
  }
  if (hash->type != JSON_TYPE_HASHTABLE) {
    return ~0;
  }
  if (NULL == key) {
    return ~0;
  }

  json_string_t str = {
    .wide = 0,
    .length = strlen(key),
    .value.bytes = (uint8_t*)strdup(key)
  };

  if (NULL == value) {
    value = json_new(JSON_TYPE_NULL);
  }
  if (original) {
    json_hashtable_del(&(hash->value.hashtable), str, original);
  }
  return json_hashtable_put(&(hash->value.hashtable), str, value);
}

#include <stdarg.h>

int json_hashtable_add_entries
  (json_t* hash, unsigned nentries, ...)
{
  va_list ap;
  char* key;
  unsigned type;
  json_t* value;
  json_t* orig;
  int boo;
  int64_t num;
  double frc;
  char* str;

  if (NULL == hash) {
    return ~0;
  }
  if (hash->type != JSON_TYPE_HASHTABLE) {
    return ~0;
  }
  va_start(ap, nentries);
  for (unsigned i=0; i < nentries; i++) {
    key = va_arg(ap, char*);
    type = va_arg(ap, unsigned);
    orig = NULL;
    switch (type) {
    case JSON_TYPE_NULL:
      value = json_new(JSON_TYPE_NULL);
      if (json_hashtable_add_entry(hash, key, value, &orig)) {
        return ~0;
      }
      break;
    case JSON_TYPE_BOOLEAN:
      boo = va_arg(ap, int);
      value = JSON_NEW_BOOLEAN(boo);
      if (json_hashtable_add_entry(hash, key, value, &orig)) {
        return ~0;
      }
      break;
    case JSON_TYPE_INTEGER:
      num = va_arg(ap, int64_t);
      value = JSON_NEW_INTEGER(num);
      if (json_hashtable_add_entry(hash, key, value, &orig)) {
        return ~0;
      }
      break;
    case JSON_TYPE_FLOAT:
      frc = va_arg(ap, double);
      value = JSON_NEW_FLOAT(frc);
      if (json_hashtable_add_entry(hash, key, value, &orig)) {
        return ~0;
      }
      break;
    case JSON_TYPE_STRING:
      str = va_arg(ap, char*);
      value = JSON_NEW_STRING(str);
      if (json_hashtable_add_entry(hash, key, value, &orig)) {
        return ~0;
      }
      break;
    case JSON_OBJECT:
      {
        json_t* obj = va_arg(ap, json_t*);
        if (json_hashtable_add_entry(hash, key, obj, &orig)) {
          return ~0;
        }
      }
      break;
    }
    if (orig) {
      json_free(orig);
    }
  }
  va_end(ap);
  return 0;
}

#include <vec_t.h>

static
void json_string__
  (json_string_t jsonstring, vec_t* str)
{
  for (unsigned i=0; i < jsonstring.length; i++) {
    unsigned c;
    if (jsonstring.wide) {
      c = jsonstring.value.unicode[ i ];
    } else {
      c = jsonstring.value.bytes[ i ];
    }
    if (c < 128 && !(jsonstring.wide)) {
      switch (c) {
      case '\\':
      case '"':
        vec_printf(str, "\\%c", (char)c);
        break;
      case '\n':
        vec_printf(str, "\\n");
        break;
      case '\r':
        vec_printf(str, "\\r");
        break;
      case '\t':
        vec_printf(str, "\\t");
        break;
      default:
        vec_appendchr(str, (unsigned char)c);
      }
    } else {
//.. translate to UTF-8
    }
  }
}

static
void json_string_
  (json_t* json, vec_t* str)
{
  switch (json->type) {
  case JSON_TYPE_NULL:
    vec_printf(str, "null");
    break;
  case JSON_TYPE_BOOLEAN:
    vec_printf(str, "%s", (json->value.boolint ? "true" : "false"));
    break;
  case JSON_TYPE_INTEGER:
    vec_printf(str, "%" PRId64, json->value.boolint);
    break;
  case JSON_TYPE_FLOAT:
    vec_printf(str, "%f", json->value.fraction);
    break;
  case JSON_TYPE_STRING:
    vec_appendchr(str, '"');
    json_string__(json->value.string, str);
    vec_appendchr(str, '"');
    break;
  case JSON_TYPE_ARRAY:
    vec_printf(str, "[");
    for (unsigned i=0; i < json->value.array.count; i++) {
      json_string_(json->value.array.list[ i ], str);
      if (i+1 < json->value.array.count) {
        vec_printf(str, ",");
      }
    }
    vec_printf(str, "]");
    break;
  case JSON_TYPE_HASHTABLE:
    vec_printf(str, "{");
    for (unsigned i=0; i < json->value.hashtable.count; i++) {
      vec_printf(str, "\"");
      json_string__(json->value.hashtable.keys[ i ], str);
      vec_printf(str, "\":");
      json_string_(json->value.hashtable.values[ i ], str);
      if (i+1 < json->value.hashtable.count) {
        vec_printf(str, ",");
      }
    }
    vec_printf(str, "}");
    break;
  }
}

char* json_string
  (json_t* json)
{
  vec_t str = { 0 };

  json_string_(json, &str);

  return (char*)(str.data);
}

void json_fdebug
  (FILE* f, json_t* json)
{
  char* string = json_string(json);

  fprintf(f, "%s", string);
  free(string);
}

void json_debug
  (json_t* json)
{
  json_fdebug(stderr, json);
}

void json_free
  (json_t* json)
{
  switch (json->type) {
  case JSON_TYPE_NULL:
  case JSON_TYPE_BOOLEAN:
  case JSON_TYPE_INTEGER:
  case JSON_TYPE_FLOAT:
    break;
  case JSON_TYPE_STRING:
    free(json->value.string.value.bytes);
    break;
  case JSON_TYPE_ARRAY:
    for (unsigned i=0; i < json->value.array.count; i++) {
      json_free(json->value.array.list[ i ]);
    }
    free(json->value.array.list);
    break;
  case JSON_TYPE_HASHTABLE:
    for (unsigned i=0; i < json->value.hashtable.count; i++) {
     free(json->value.hashtable.keys[ i ].value.bytes);
     json_free(json->value.hashtable.values[ i ]);
    }
    free(json->value.hashtable.keys);
    free(json->value.hashtable.values);
    break;
  }
  free(json);
}

json_t* json_query_path
  (json_t* json, const char* path)
{
  gpege_ec_t result = { 0 };
  gpeg_capturelist_t resobj = { 0 };
  vec_t input = { (unsigned char*)path, strlen(path) };
  GPEG_ERR_T e;

  result.input = &input;
  if (pathparser_init == 0) {
    gpege_init(&pathparser);
    pathparser_init = 1;
  }
  e = gpege_run(&pathparser, &result);
  if (e.code) {
    fprintf(stderr, "Jpath parse error %d.\n", e.code);
    gpege_ec_free(&result);
    return NULL;
  }
  e = gpege_actions2captures(&input, &(result.actions), &resobj);
  if (e.code) {
    gpege_ec_free(&result);
    return NULL;
  }
  gpege_ec_free(&result);
  gpeg_capturelist_free(&resobj);

  return json;
}

#include <stdarg.h>

static
const json_t* json_iterate_
  (const json_t* json, unsigned nlist, va_list ap)
{
  for (unsigned i=0; i < nlist; i++) {
    int type = va_arg(ap, int);
    if (json->type == JSON_TYPE_HASHTABLE && type == JSON_QUERY_KEY) {
      char* key = va_arg(ap, char*);
      for (unsigned j=0; j < json->value.hashtable.count; j++) {
        if (0 == json_string_compare2(&(json->value.hashtable.keys[ j ]), key))
        {
          json = json->value.hashtable.values[ j ];
          goto KEYFOUND;
        }
      }
      return NULL;
KEYFOUND: ;
    } else if (json->type == JSON_TYPE_ARRAY && type == JSON_QUERY_INDEX) {
      unsigned ndx = va_arg(ap, unsigned);
      if (ndx < json->value.array.count) {
        json = json->value.array.list[ ndx ];
      } else {
        return NULL;
      }
    } else {
      return NULL;
    }
  }
  return json;
}

const json_t* json_iterate
  (const json_t* json, unsigned nlist, ...)
{
  const json_t* node;
  va_list ap;

  if (json == 0) { return NULL; }
  va_start(ap, nlist);
  node = json_iterate_(json, nlist, ap);
  va_end(ap);
  return node;
}

static
char jqbuf[ 64 ];

const char* json_query
  (const json_t* json, unsigned nlist, ...)
{
  const json_t* node;
  va_list ap;

  if (json == 0) { return NULL; }
  va_start(ap, nlist);
  node = json_iterate_(json, nlist, ap);
  va_end(ap);

  if (NULL == node) {
    return NULL;
  }

  switch (node->type) {
  case JSON_TYPE_NULL:
    snprintf(jqbuf, sizeof(jqbuf), "null");
    return jqbuf;
  case JSON_TYPE_BOOLEAN:
    snprintf(jqbuf, sizeof(jqbuf), "%s",
      (node->value.boolint ? "true" : "false")
    );
    return jqbuf;
  case JSON_TYPE_INTEGER:
    snprintf(jqbuf, sizeof(jqbuf), "%" PRId64, node->value.boolint);
    return jqbuf;
  case JSON_TYPE_FLOAT:
    snprintf(jqbuf, sizeof(jqbuf), "%f", node->value.fraction);
    return jqbuf;
  case JSON_TYPE_STRING:
    return (char*)(node->value.string.value.bytes);
  default:
    return NULL;
  }
}

/**
 * Utility function. Performs an errorless 'cast' on the result.
 */
int json_query_as_boolean
  (const json_t* json, unsigned nlist, ...)
{
  const json_t* node;
  va_list ap;

  if (json == 0) { return 0; }
  va_start(ap, nlist);
  node = json_iterate_(json, nlist, ap);
  va_end(ap);

  if (NULL == node) {
    return 0;
  }

  switch (node->type) {
  case JSON_TYPE_BOOLEAN:
  case JSON_TYPE_INTEGER:
    return ((json->value.boolint) ? 1 : 0);
  case JSON_TYPE_FLOAT:
    return ((json->value.fraction) ? 1 : 0);
  default:
    return 0;
  }
}

int64_t json_query_as_integer
  (const json_t* json, unsigned nlist, ...)
{
  const json_t* node;
  va_list ap;

  if (json == 0) { return 0; }
  va_start(ap, nlist);
  node = json_iterate_(json, nlist, ap);
  va_end(ap);

  if (NULL == node) {
    return 0;
  }

  switch (node->type) {
  case JSON_TYPE_BOOLEAN:
  case JSON_TYPE_INTEGER:
    return node->value.boolint;
  case JSON_TYPE_FLOAT:
    return (int64_t)(node->value.fraction);
  case JSON_TYPE_STRING:
    return strtoll((char*)(node->value.string.value.bytes), 0, 10);
  default:
    return 0;
  }
}

json_string_t json_dup_string
  (const json_string_t orig)
{
  json_string_t result = orig;
  unsigned nbytes = (orig.wide ? (orig.length + 1)*4 : (orig.length + 1));
  result.value.bytes = calloc(1, nbytes);
  memcpy(result.value.bytes, orig.value.bytes, nbytes);
  return result;
}

/**
 * Makes a deep copy of the given json structure.
 */
json_t* json_dup
  (const json_t* json)
{
  json_t* replica = json_new(json->type);
  switch (json->type)
  {
  case JSON_TYPE_NULL:
    break;
  case JSON_TYPE_BOOLEAN:
  case JSON_TYPE_INTEGER:
    replica->value.boolint = json->value.boolint;
    break;
  case JSON_TYPE_FLOAT:
    replica->value.fraction = json->value.fraction;
    break;
  case JSON_TYPE_STRING:
    replica->value.string = json_dup_string(json->value.string);
    break;
  case JSON_TYPE_ARRAY:
    {
      for (unsigned i=0; i < json->value.array.count; i++) {
        json_array_add_element(replica, json_dup(json->value.array.list[ i ]));
      }
    }
    break;
  case JSON_TYPE_HASHTABLE:
    {
      for (unsigned i=0; i < json->value.hashtable.count; i++) {
        json_string_t key = json->value.hashtable.keys[ i ];
        json_t* value = json->value.hashtable.values[ i ];
        json_hashtable_add_entry(
          replica,
          (char*)(key.value.bytes),
          json_dup(value),
          0
        );
      }
    }
    break;
  }
  return replica;
}
