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

static
void _ytf_format_flat_string
  (vec_t* str, vec_t* flat)
{
  int do_b64 = 0;

  for (unsigned i=0; i < str->size; i++) {
    if (str->data[ i ] < 32 || str->data[ i ] > 127) {
      do_b64 = 1;
      break;
    }
  }
  if (do_b64) {
    vec_base64_encode(str, 0);
    vec_appendstr(flat, "b64(");
    vec_append(flat, str->data, str->size);
    vec_appendstr(flat, ")");
  } else {
    vec_appendchr(flat, '"');
    for (unsigned i=0; i < str->size; i++) {
      if (str->data[ i ] == '\\' || str->data[ i ] == '"') {
        vec_appendchr(flat, '\\');
      }
      vec_appendchr(flat, str->data[ i ]);
    }
    vec_appendchr(flat, '"');
  }
}

static
void _ytf_format_flat
  (ytf_t* ytf, vec_t* key, vec_t* flat)
{
  switch (ytf->type) {
  case YTF_TYPE_NULL:
    vec_printf(flat, "%s:null\n", (char*)(key->data));
    break;
  case YTF_TYPE_BOOLEAN:
    if (ytf->value.boolint) {
      vec_printf(flat, "%s:true\n", (char*)(key->data));
    } else {
      vec_printf(flat, "%s:false\n", (char*)(key->data));
    }
    break;
  case YTF_TYPE_INTEGER:
    vec_printf(flat, "%s:%"PRId64"\n", (char*)(key->data), ytf->value.boolint);
    break;
  case YTF_TYPE_FLOAT:
    vec_printf(flat, "%s:%f", (char*)(key->data), ytf->value.fraction);
    while (vec_endswith(flat, "0") && !vec_endswith(flat, ".0")) {
      vec_reduce(flat, 1);
    }
    vec_appendchr(flat, '\n');
    break;
  case YTF_TYPE_STRING:
    vec_printf(flat, "%s:", (char*)(key->data));
    _ytf_format_flat_string(&(ytf->value.string), flat);
    vec_printf(flat, "\n");
    break;
  case YTF_TYPE_ARRAY:
    for (unsigned i=0; i < ytf->value.array.count; i++) {
      vec_t keycopy = { 0 };
      vec_append(&keycopy, key->data, key->size);
      if (key->size) { vec_appendchr(&keycopy, '.'); }
      vec_printf(&keycopy, "%u", i);
      _ytf_format_flat(ytf->value.array.list[ i ], &keycopy, flat);
      free(keycopy.data);
    }
    break;
  case YTF_TYPE_HASHTABLE:
    for (unsigned i=0; i < ytf->value.hash.count; i++) {
      vec_t keycopy = { 0 };
      vec_append(&keycopy, key->data, key->size);
      if (key->size) { vec_appendchr(&keycopy, '.'); }
      vec_appendstr(&keycopy, ytf->value.hash.keys[ i ]);
      _ytf_format_flat(ytf->value.hash.values[ i ], &keycopy, flat);
      free(keycopy.data);
    }
    break;
  }
}

void ytf_format_flat_tagged
  (ytf_t* ytf, vec_t* flat, char* tag)
{
  vec_t key = { 0 };

  vec_printf(flat, "#%s\n", tag);
  _ytf_format_flat(ytf, &key, flat);
}

void ytf_format_flat
  (ytf_t* ytf, vec_t* flat)
{
  ytf_format_flat_tagged(ytf, flat, "ytf");
}
