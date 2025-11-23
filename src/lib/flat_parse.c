/**
 * This file is part of the YTF project.

Copyright (c) 2024, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
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

#include <ytf/lib.h>
#include "flat_slotmap.h"

static
ytf_t* flat_parse_assert_path
  (ytf_t* top, gpeg_capture_t* c)
{
  ytf_t* parent = top;

  for (unsigned i=0; i < c->children.count; i += 2) {
    char* key = (char*)(c->children.list[ i ].data.data);
    if (strspn(key, "0123456789") == strlen(key)) {
      if (parent->type == 0 || parent->type == YTF_TYPE_ARRAY) {
        unsigned index = strtoull(key, 0, 10);
        parent->type = YTF_TYPE_ARRAY;
        if (index >= parent->value.array.count) {
          for (unsigned i=parent->value.array.count; i <= index; i++) {
            ytf_t* nll = calloc(1, sizeof(ytf_t));
            nll->type = (i < index) ? YTF_TYPE_NULL : 0;
            ytf_array_push(&(parent->value.array), nll);
          }
        }
        parent = parent->value.array.list[ index ];
      }
    } else {
      key = strdup(key);
      if (parent->type == 0 || parent->type == YTF_TYPE_HASHTABLE) {
        ytf_t* value = 0;
        parent->type = YTF_TYPE_HASHTABLE;
        if (ytf_hash_get(&(parent->value.hash), key, &value) != 0) {
          value = calloc(1, sizeof(ytf_t));
          ytf_hash_put(&(parent->value.hash), key, value);
        }
        parent = value;
      }
    }
  }
  return parent;
}

static
void flat_parse_value
  (gpeg_capture_t* c, ytf_t* value)
{
  switch (c->children.list[ 0 ].type) {
  case SLOT_NULL:
    value->type = YTF_TYPE_NULL;
    break;
  case SLOT_BOOL:
    value->type = YTF_TYPE_BOOLEAN;
    if (0 == strcmp((char*)(c->children.list[ 0 ].data.data), "true")) {
      value->value.boolint = 1;
    }
    break;
  case SLOT_FLOAT:
    value->type = YTF_TYPE_FLOAT;
    value->value.fraction = strtod((char*)(c->children.list[ 0 ].data.data), 0);
    break;
  case SLOT_INT:
    value->type = YTF_TYPE_INTEGER;
    value->value.boolint = strtoll((char*)(c->children.list[ 0 ].data.data), 0, 10);
    break;
  case SLOT_STRING:
    value->type = YTF_TYPE_STRING;
    vec_reduce(&(c->children.list[ 0 ].data), 1);
    vec_delete(&(c->children.list[ 0 ].data), 0, 1);
    value->value.string = c->children.list[ 0 ].data;
//.. unescape
    c->children.list[ 0 ].data.data = NULL;
    c->children.list[ 0 ].data.size = 0;
    break;
  case SLOT_BLOB:
    value->type = YTF_TYPE_STRING;
    vec_reduce(&(c->children.list[ 0 ].data), 1);
    vec_delete(&(c->children.list[ 0 ].data), 0, 4);
    if (vec_base64_decode(&(c->children.list[ 0 ].data))) { }
    value->value.string = c->children.list[ 0 ].data;
    c->children.list[ 0 ].data.data = NULL;
    c->children.list[ 0 ].data.size = 0;
    break;
  }
}

static
void flat_parse_nvpair
  (gpeg_capture_t* c, ytf_t* ytf)
{
  ytf_t value = { 0 };
  flat_parse_value(&(c->children.list[ 2 ]), &value);
  ytf_t* name = flat_parse_assert_path(ytf, &(c->children.list[ 0 ]));
  *name = value;
}

static
ytf_t* flat_parse_object
  (gpeg_capture_t* object)
{
  ytf_t* result = calloc(1, sizeof(ytf_t));

  for (unsigned i=0; i < object->children.count; i++) {
    flat_parse_nvpair(&(object->children.list[ i ]), result);
  }
  return result;
}

/**
 *
 */
void flat_parse
  (gpeg_capture_t* c, ytf_array_t* array)
{
  if (c->type == SLOT_YTF
      && c->children.count >= 1
      && c->children.list[ 0 ].type == SLOT_OBJECTS)
  {
    for (unsigned i=0; i < c->children.list[ 0 ].children.count; i++) {
      gpeg_capture_t* object = &(c->children.list[ 0 ].children.list[ i ]);
      if (object->type == SLOT_OBJECT) {
        ytf_t* ytf = flat_parse_object(
                       &(c->children.list[ 0 ].children.list[ i ])
                     );
        ytf_array_push(array, ytf);
      }
    }
  }
}
