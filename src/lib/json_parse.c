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
#include "json_slotmap.h"

ytf_t* json_parse
  (gpeg_capture_t* c);

static
void json_parse_hashelt
  (gpeg_capture_t* c, ytf_t* parent)
{
  switch (c->type) {
  case SLOT_HASHELT:
    {
      char* name = strdup((char*)(c->children.list[ 0 ].data.data));
      ytf_t* value = json_parse(&(c->children.list[ 1 ]));
      ytf_hash_put(&(parent->value.hash), name, value);
    }
  }
}

static
void json_parse_string
  (gpeg_capture_t* parent, ytf_t* elt)
{
  for (unsigned i=0; i < parent->children.count; i++) {
    gpeg_capture_t* child = &(parent->children.list[ i ]);
    if (child->type == SLOT_UTF8CHAR) {
      vec_append(&(elt->value.string), child->data.data, child->data.size);
    } else if (child->type == SLOT_STRING_0) {
      switch (child->data.data[ 0 ]) {
      case 'n': vec_appendchr(&(elt->value.string), 0x0a); break;
      case 'r': vec_appendchr(&(elt->value.string), 0x0d); break;
      case 't': vec_appendchr(&(elt->value.string), 0x09); break;
      case 'b': vec_appendchr(&(elt->value.string), 0x08); break;
      case 'f': vec_appendchr(&(elt->value.string), 0x0c); break;
      case '"': vec_appendchr(&(elt->value.string), 0x22); break;
      case '\\': vec_appendchr(&(elt->value.string), 0x5c); break;
      case 'u': /* TODO: Unicode escape */ break;
      }
    }
  }
}

/**
 *
 */
ytf_t* json_parse
  (gpeg_capture_t* c)
{
  switch (c->type) {
  case SLOT_HASH:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_HASHTABLE;
      for (unsigned i=0; i < c->children.count; i++) {
        json_parse_hashelt(&(c->children.list[ i ]), ytf);
      }
      return ytf;
    }
    break;
  case SLOT_ARRAY:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_ARRAY;
      for (unsigned i=0; i < c->children.count; i++) {
        ytf_t* child = json_parse(&(c->children.list[ i ]));
        ytf_array_push(&(ytf->value.array), child);
      }
      return ytf;
    }
    break;
  case SLOT_BIGSTRING:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_STRING;
      json_parse_string(c, ytf);
      return ytf;
    }
    break;
  case SLOT_STRING:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_STRING;
      json_parse_string(c, ytf);
      return ytf;
    }
    break;
  case SLOT_INT:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_INTEGER;
      ytf->value.boolint = strtoll((char*)(c->data.data), 0, 10);
      return ytf;
    }
    break;
  case SLOT_FLOAT:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_FLOAT;
      ytf->value.fraction = strtod((char*)(c->data.data), 0);
      return ytf;
    }
    break;
  case SLOT_BOOL:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_BOOLEAN;
      if (0 == strcmp((char*)(c->data.data), "true")) {
        ytf->value.boolint = 1;
      }
      return ytf;
    }
    break;
  case SLOT_NULL:
    {
      ytf_t* ytf = calloc(1, sizeof(ytf_t));
      ytf->type = YTF_TYPE_NULL;
      return ytf;
    }
    break;
  default:
    fprintf(stderr, "WARNING: Unexpected token type %d\n", c->type);
    return NULL;
  }
}
