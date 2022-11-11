#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <locale.h>

const size_t MAX_LEN_CMD = 350;

#define SPEECH_SYNTHESIZER(work_synthesizer, ...)   if (work_synthesizer)                                           \
                                                    {                                                               \
                                                        char command[MAX_LEN_CMD] = "";                             \
                                                        snprintf (command, MAX_LEN_CMD, "say " __VA_ARGS__);               \
                                                        system (command);                                           \
                                                    }                           

void screen_clear ();

int stricmp (const char* str1, const char* str2);

void speech_synthesizer (int work_synthesizer, char* text);
#endif // COMMON_H