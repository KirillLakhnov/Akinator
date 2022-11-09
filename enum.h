#ifndef ENUM_H
#define ENUM_H

enum ERRORS {
    GOOD_WORKING                    = 0,
    ERROR_MEMMORY                   = 1,
    ERROR_READING_FILE              = 2,
    ERROR_FILE_OPEN                 = 3,
    ERROR_FILE_CLOSE                = 4,
    ERROR_FSEEK                     = 5,
    ERROR_FTELL                     = 6,
    ERROR_FPRINTF                   = 7,
    ERROR_FPUTS                     = 8,
    ERROR_BUFFER_CREATER            = 9,
    INCORRECT_COMMAND               = 10,
    ERROR_SIGNATURE                 = 11,
    ERROR_VERSION_CPU               = 12,
    ERROR_SIZE_FILE                 = 13,
    ERROR_CALLOC                    = 14,
    ERROR_REALLOC                   = 15,
    ERROR_MARK_FIND                 = 16,
    ERROR_SSCANF                    = 17,
    ERROR_COMMAND_CMP               = 18,
    ERROR_STACK_CTOR                = 19,
    ERROR_FWRITE                    = 20,
    ERROR_COMPILATION_FIRST_PASS    = 21,
};

#endif // ENUMS_H
