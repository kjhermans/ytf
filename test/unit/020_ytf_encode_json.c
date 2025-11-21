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
#include <stdint.h>

/**
 *
 */
int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  char* tests[] = {
    "[1]",
    "[1,2,3]",
    "{\"foo\":\"bar\"}",
    "{\"foo\":{}}",
    "{\"foo\":[]}",
    "{\"foo\":1.0}",
    "{\"foo\":1.1}",
    "{\"foo\":-1.1}",
    "{\"foo\":9.19999980926513671875}",
    "{\"foo\":[1,2,3]}",
    "{\"foo\":\"bar\",\"oi\":\"foobar\"}",
    "{\"foo\":\"bar\",\"foo\":\"oi\"}",
    "{\"a\":[\"a\",0.444,false,3]}",
    "{\"a\":[\"a\",0.444,false,3],\"b\":{\"c\":{\"d\":[3,3,3,3,3.141592]}}}",
    "{\"a\":\"It was the best of times, it was the worst of times.\"}",
    NULL
  };
  unsigned i=0;

  while (1) {
    ytf_t ytf = { 0 };
    char* jsonstring = tests[ i++ ];
    vec_t json = { jsonstring, strlen(jsonstring) };
    vec_t json_compare = { 0 };

    if (jsonstring == NULL) {
      break;
    }
    if (ytf_parse_json(&json, &ytf)) {
      fprintf(stderr, "Failed at %s\n", jsonstring);
      return ~0;
    }

    ytf_encode_json(&ytf, &json_compare);
    if (0 == vec_compare(&json, &json_compare)) {
      fprintf(stderr, "Test succeeded.\n");
    } else {
      fprintf(stderr, "Test failed.\n");
      exit(1);
    }
  }
  return 0;
}
