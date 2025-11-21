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

#include <ytf/ytf.h>

static
#include "json_bytecode.h"

static
int parser_init = 0;


static
vec_t err = { 0 };

static
gpege_t parser = { 0 };

int ytf_parse_json
  (const vec_t* string, ytf_t* ytf)
{
  DEBUGFUNCTION
  ASSERT(string)
  ASSERT(obj)

  gpege_ec_t result = { 0 };
  gpeg_capturelist_t resobj = { 0 };
  GPEG_ERR_T e;

  result.input = string;
  result.errorstr = &err;
  if (parser_init == 0) {
    gpege_init(&parser);
    parser.bytecode.data = json_byc;
    parser.bytecode.size = json_byc_len;
    parser_init = 1;
  }
  e = gpege_run(&parser, &result);
  if (e.code) {
    vec_printf(&err, "Parser error %s", (char*)(err.data));
    gpege_ec_free(&result);
    return ~0;
  }
  e = gpege_actions2captures(result.input, &(result.actions), &resobj);
  if (e.code) {
    vec_printf(&err, "Parser error %s", (char*)(err.data));
    gpege_ec_free(&result);
    return ~0;
  }
  gpege_ec_free(&result);

  int i = json_grammar_process_node(&(resobj.list[ 0 ]), NULL); (void)i;
  gpeg_capturelist_free(&resobj);

  return 0;
}


