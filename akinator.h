#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>

#include "libraries/common.h"
#include "tree.h"

#define RED_TEXT(text) "\e[31m" #text "\e[0m"

enum COMMAND {
    COMMAND_GUESSING          = 49,
    COMMAND_DEFINITION        = 50,
    COMMAND_OBJECT_COMPARISON = 51,
    COMMAND_EXITING_PROGRAMM  = 52,

    ANSWER_YES = 89,
    ANSWER_NO  = 78,
};

void main_menu (struct Tree* tree);

void guessing_menu (struct Tree* tree);

void guessing_mode (struct Knot* current_knot);

void definition_menu ();

void object_comparison_menu ();

void processing_selected_mode (struct Tree* tree);

void processing_selected_response (struct Knot* current_knot);

int get_command();

#endif // AKINATOR_H