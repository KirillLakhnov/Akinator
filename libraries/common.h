#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <locale.h>

const size_t MAX_LEN_CMD = 100;

#define SPEECH_SYNTHESIZER(work_synthesizer, text, there_is_variable)   if (work_synthesizer)                                           \
                                                                        {                                                               \
                                                                            if (there_is_variable == 0)                                 \
                                                                            {                                                           \
                                                                                system ("say " #text);                                  \
                                                                            }                                                           \
                                                                            else                                                        \
                                                                            {                                                           \
                                                                                char command[MAX_LEN_CMD] = "";                         \
                                                                                snprintf (command, MAX_LEN_CMD, "say %s", text);        \
                                                                                system (command);                                       \
                                                                            }                                                           \
                                                                        }                           

void screen_clear ();

int stricmp (const char* str1, const char* str2);

#endif // COMMON_H