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

const size_t MAX_STR_SIZE = 250;

enum COMMAND {
    COMMAND_GUESSING          = 49,
    COMMAND_DEFINITION        = 50,
    COMMAND_OBJECT_COMPARISON = 51,
    COMMAND_EXITING_PROGRAMM  = 52,

    ANSWER_YES = 89,
    ANSWER_NO  = 78,

    COMMAND_AGAIN       = 49,
    COMMAND_UPDATE_BASE = 50,
    COMMAND_OUTPUT_MAIN = 51,
};

void main_menu (struct Tree* tree);

void guessing_menu (struct Tree* tree);

void guessing_mode (struct Tree* tree, struct Knot* current_knot);

void definition_menu (struct Tree* tree);

void definition_mode (struct Tree* tree);

void object_comparison_menu ();

void processing_selected_mode (struct Tree* tree);

void processing_selected_response (struct Tree* tree, struct Knot* current_knot);

void menu_after_game (struct Tree* tree);

void processing_selected_mode_after_game (struct Tree* tree);

int get_command();

int get_word (char* word);

//---------------------------------------------------------------------------------

void update_tree (struct Tree* tree, struct Knot* current_knot, char* new_object, char* distinctive_property);

void update_base_print (FILE* base, struct Tree* tree, struct Knot* knot_root);

void update_base (struct Tree* tree);

//---------------------------------------------------------------------------------

#endif // AKINATOR_H