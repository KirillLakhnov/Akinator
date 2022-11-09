#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <locale.h>

#define SPEECH_SYNTHESIZER(work_synthesizer, text)  if (work_synthesizer)       \
                                                    {                           \
                                                        system ("say " #text);  \
                                                    }                           

void screen_clear ();

int stricmp (const char* str1, const char* str2);

#endif // COMMON_H