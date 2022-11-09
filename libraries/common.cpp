#include "common.h"

void screen_clear ()
{
    system ("clear");
}

int stricmp (const char* str1, const char* str2)
{
    assert (str1);
    assert (str2);
    
    int i = 0;
    while (tolower (str1[i]) == tolower (str2[i]) && (str1[i] != '\0' && str2[i++] != '\0'));
    
    return tolower (str1[i]) - tolower (str2[i]);
}
