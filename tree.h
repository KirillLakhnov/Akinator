#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <locale.h>

#include "libraries/FileProcessing.h"
#include "libraries/stack.h"

#ifndef NDEBUG
#define ASSERT_OK(tree) do                                      \
                        {                                       \
                            if (tree_error (tree) != 0)         \
                            {                                   \
                                tree_graph_dump (tree);         \
                                abort ();                       \
                            }                                   \
                        } while (0)

#define CHECK_ERROR(condition, error) (condition) ? error : 0

#else
#define ASSERT_OK(tree)
#define CHECK_ERROR(condition, error)
#endif

const size_t NAME_MAX_LEN = 100;
const size_t DTOR_SIZE_T = 0xBABADEDA;
const int DTOR_INT = 0xBABADEAD;

enum TREE_ERROR
{
    TREE_ERROR_SYNTAX_IN_BASE = 1 << 0,
    TREE_WRONG_NAME_DATA_BASE = 1 << 1,

    TREE_ERROR_CALLOC         = 1 << 2,
    TREE_ERROR_STRCPY         = 1 << 3,
    TREE_ERROR_BUFFER_CREATER = 1 << 4,
};

struct Knot
{
    struct Knot* prev;

    struct Knot* right;
    struct Knot* left;

    int length;
    char* string;
};

struct Tree 
{
    struct FileInfo* file_database;
    struct Text* buffer_database;

    struct Knot* root;

    size_t size;
    size_t code_error;

    char* current_position_buffer;
};

void tree_ctor (struct Tree* tree);

void tree_dtor (struct Tree* tree);

void knot_dtor (struct Knot* current_knot);

void tree_creater (struct Tree* tree);

struct Knot* knot_creater (struct Tree* tree, struct Knot* prev);

//-----------------------------------------------------------------------------------

int tree_error (struct Tree* tree)
{

}

int decoder_list_error (struct Tree* tree)
{

}

//-----------------------------------------------------------------------------------

void tree_print (struct Knot* knot);

void tree_graph_node (struct Knot* knot, FILE* tree_log_graph);

void tree_graph_communications (struct Knot* knot, FILE* tree_log_graph);

int tree_graph_dump (struct Tree* tree);

#endif // TREE_H