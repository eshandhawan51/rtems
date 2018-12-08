/**
 *  @file
 *  @brief mbsnrtowcs() API Conformance Test
 */

/*
 *  COPYRIGHT (c) 2018.
 *  Zenon (zehata).
 *
 *  Permission to use, copy, modify, and/or distribute this software
 *  for any purpose with or without fee is hereby granted.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *  WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
 *  BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
 *  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 *  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 *  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <wchar.h>

int test(void);

int test(void)
{
  wchar_t *wc;
  const char* *ch;
  mbstate_t state;
  size_t s0, s1;

  wc = 0;
  ch = 0;
  s0 = 0;
  s1 = 0;
  return mbsnrtowcs(wc, ch, s0, s1, &state);
}