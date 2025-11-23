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

#include <inttypes.h>

#include <ytf/lib.h>

void ytf_format_json_string
  (unsigned char* str, unsigned size, vec_t* json)
{
  for (unsigned i=0; i < size; i++) {
    if (str[ i ] < 32) {
      switch (str[ i ]) {
      case '\n':
        vec_appendstr(json, "\\n"); break;
      case '\r':
        vec_appendstr(json, "\\r"); break;
      case '\t':
        vec_appendstr(json, "\\t"); break;
      default:
        vec_printf(json, "\\x%.2x", str[ i ]);
      }
    } else if (str[ i ] == '"') {
      vec_appendstr(json, "\\\"");
    } else if (str[ i ] == '\\') {
      vec_appendstr(json, "\\\\");
    } else if (str[ i ] > 127) {
      vec_printf(json, "\\x%.2x", str[ i ]);
    } else {
      vec_appendchr(json, str[ i ]);
    }
  }
}

/**
 *
 */
void ytf_format_json
  (ytf_t* ytf, vec_t* json)
{
  switch (ytf->type) {
  case YTF_TYPE_NULL:
    vec_printf(json, "null");
    break;
  case YTF_TYPE_BOOLEAN:
    if (ytf->value.boolint) {
      vec_printf(json, "true");
    } else {
      vec_printf(json, "false");
    }
    break;
  case YTF_TYPE_INTEGER:
    vec_printf(json, "%"PRId64, ytf->value.boolint);
    break;
  case YTF_TYPE_FLOAT:
    vec_printf(json, "%f", ytf->value.fraction);
    while (vec_endswith(json, "0") && !vec_endswith(json, ".0")) {
      vec_reduce(json, 1);
    }
    break;
  case YTF_TYPE_STRING:
    vec_appendchr(json, '"');
    ytf_format_json_string(ytf->value.string.data, ytf->value.string.size, json);
    vec_appendchr(json, '"');
    break;
  case YTF_TYPE_ARRAY:
    vec_appendchr(json, '[');
    for (unsigned i=0; i < ytf->value.array.count; i++) {
      ytf_format_json(ytf->value.array.list[ i ], json);
      if (i < ytf->value.array.count - 1) {
        vec_appendchr(json, ',');
      }
    }
    vec_appendchr(json, ']');
    break;
  case YTF_TYPE_HASHTABLE:
    vec_appendchr(json, '{');
    for (unsigned i=0; i < ytf->value.hash.count; i++) {
      char* key = ytf->value.hash.keys[ i ];
      vec_appendchr(json, '"');
      if (key) {
        ytf_format_json_string((unsigned char*)key, strlen(key), json);
      }
      vec_appendstr(json, "\":");
      ytf_format_json(ytf->value.hash.values[ i ], json);
      if (i < ytf->value.hash.count - 1) {
        vec_appendchr(json, ',');
      }
    }
    vec_appendchr(json, '}');
    break;
  }
}
