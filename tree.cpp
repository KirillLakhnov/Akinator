#include "tree.h"

void tree_ctor (struct Tree* tree)
{
    tree->size = 0;
    tree->code_error = 0;

    if (BufferCreater (tree->file_database, tree->buffer_database) != GOOD_WORKING)
    {
        printf ("Error BufferCreater in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_BUFFER_CREATER;
        abort ();
    }

    tree->current_position_buffer = tree->buffer_database->file_buffer;

    tree->root = knot_creater (tree, nullptr);

    ASSERT_OK_TREE(tree);
}

void tree_dtor (struct Tree* tree)
{
    ASSERT_OK_TREE(tree);

    TextDtor (tree->buffer_database);

    tree->file_database->file_name = nullptr;

    knot_dtor (tree->root);

    tree->root = nullptr;
    tree->code_error = DTOR_SIZE_T;
    tree->size = DTOR_SIZE_T;
}

void knot_dtor (struct Knot* current_knot)
{
    assert (current_knot);

    if (current_knot->left != nullptr) 
    {
        knot_dtor (current_knot->left);
    }
    if (current_knot->right != nullptr) 
    {
        knot_dtor (current_knot->right);
    }

    current_knot->prev = nullptr;
    current_knot->right = nullptr;
    current_knot->left = nullptr;

    current_knot->length = DTOR_INT;
    
    if (current_knot->string != nullptr)
    {
        free (current_knot->string);
        current_knot->string = nullptr;
    }
}

void tree_creater (struct Tree* tree)
{
    ASSERT_OK_TREE(tree);

    FILE* base = fopen (tree->file_database->file_name, "rb");
    if (base == nullptr)
    {
        printf ("Error fopen in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_WRONG_NAME_DATA_BASE;
        abort ();
    }

    struct Knot* current_knot = tree->root;

    //исправить
    char* bracket = (char*) calloc (2, sizeof(char));
    if (bracket == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;         
    }

    //исправить

    while (tree->buffer_database->file_buffer + tree->buffer_database->size_buffer > tree->current_position_buffer)
    {
        int lenght = 0;
        if (sscanf (tree->current_position_buffer, "%s %n", bracket, &lenght) != 1)
        {
            printf ("Error sscanf in tree.cpp on line = %d", __LINE__);
            tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE; 
            abort ();
        }

        if (strcmp (bracket, "{") == 0)
        {
            struct Knot* new_knot = knot_creater (tree, current_knot);

            current_knot->left = new_knot;
            current_knot = new_knot;
        }
        else if (strcmp (bracket, "}") == 0)
        {
            tree->current_position_buffer += lenght;
            current_knot = current_knot->prev;
            sscanf (tree->current_position_buffer, "%s ", bracket);
            
            if (strcmp (bracket, "{") == 0)
            {
                struct Knot* new_knot = knot_creater (tree, current_knot);

                current_knot->right = new_knot;
                current_knot = new_knot;
            }
        }
        else
        {
            tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE; 
            ASSERT_OK_TREE (tree);
        }
    }

    free (bracket);
    fclose (base);

    ASSERT_OK_TREE(tree);
}

struct Knot* knot_creater (struct Tree* tree, struct Knot* prev)
{

    struct Knot* current_knot = (Knot*) calloc (1, sizeof (Knot));
    if (current_knot == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;
        ASSERT_OK_TREE (tree);
    }

    tree->size++;

    current_knot->prev = prev;
    current_knot->left = nullptr;
    current_knot->right = nullptr;

    tree->current_position_buffer = strchr (tree->current_position_buffer, '{');
    if (tree->current_position_buffer == nullptr)
    {
        printf ("Error syntax in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE;
        ASSERT_OK_TREE (tree);
    }

    char* pointer_begin = strchr (tree->current_position_buffer, '"');
    if (pointer_begin == nullptr)
    {
        printf ("Error syntax in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE;
        ASSERT_OK_TREE (tree);
    }

    char* pointer_end = strchr (pointer_begin + 1, '"');
    if (pointer_end == nullptr)
    {
        printf ("Error syntax in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_SYNTAX_IN_BASE;
        ASSERT_OK_TREE (tree);
    }

    current_knot->length = pointer_end - pointer_begin;
    current_knot->string = (char*) calloc (current_knot->length, sizeof (char));
    if (current_knot->string == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;
        ASSERT_OK_TREE (tree);
    }
    
    *pointer_end = '\0';

    if (strcpy (current_knot->string, pointer_begin + 1) == nullptr)
    {
        printf ("Error strcpy in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_STRCPY;
        ASSERT_OK_TREE (tree);
    }

    tree->current_position_buffer = pointer_end + 1;

    return current_knot;
}

//-----------------------------------------------------------------------------------

int tree_error (struct Tree* tree)
{
#ifndef NDEBUG
    int pointer_list_check_null = ((!tree) ? TREE_ERROR_POINTER_STRUCT_NULL : 0);

    if (pointer_list_check_null == 0)
    {
        tree->code_error |= CHECK_ERROR (!tree->root,                    TREE_ERROR_POINTER_ROOT_NULL);
        tree->code_error |= CHECK_ERROR (!tree->buffer_database,         TREE_ERROR_POINTER_BUFFER_NULL);
        tree->code_error |= CHECK_ERROR (!tree->file_database,           TREE_ERROR_POINTER_FILE_INFO_NULL);
        tree->code_error |= CHECK_ERROR (tree->size < 0,                 TREE_ERROR_SIZE_SMALLER_ZERO);
    }

    return tree->code_error;
#else
    return GOOD_WORKING;
#endif
}

//-----------------------------------------------------------------------------------
void tree_print (struct Knot* knot)
{
    printf ("{");

    if (!knot)
    {
        return;
    }
    printf ("\"%s\"", knot->string);
    if (knot->left)
    {
        tree_print (knot->left);
    }
    
    if (knot->right)
    {
        tree_print (knot->right);
    }

    printf ("}");
}

void knot_graph (struct Tree* tree, struct Knot* current_knot, FILE* tree_log_graph, int* count)
{
    if (current_knot == nullptr)
    {
        return;
    }

    (*count)++;

    if (current_knot->right == nullptr && current_knot->left == nullptr)
    {
        fprintf(tree_log_graph, "\t\"%s\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#ee9b52\","
		                        "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                current_knot->string, current_knot->prev, current_knot, current_knot->left, current_knot->string, current_knot->right);
    }
    else 
    {
        fprintf(tree_log_graph, "\t \"%s\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
		                        "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                                current_knot->string, current_knot->prev, current_knot, current_knot->left, current_knot->string, current_knot->right);
    }

    if (current_knot->left != nullptr)
    {
        fprintf(tree_log_graph, "\t \"%s\" -> \"%s\" [style = \"bold\", label=\"Да\"]\n", current_knot->string, current_knot->left->string);
    }

    if (current_knot->right != nullptr)
    {
        fprintf(tree_log_graph, "\t \"%s\" -> \"%s\" [style = \"bold\", label=\"Нет\"]\n", current_knot->string, current_knot->right->string);
    }

    if (current_knot->left  != nullptr) 
    {
        knot_graph(tree, current_knot->left, tree_log_graph, count);
    }

    if (current_knot->right != nullptr) 
    {
        knot_graph(tree, current_knot->right, tree_log_graph, count); 
    }
}

int tree_graph_dump (struct Tree* tree)
{
    static int number_of_function_launches = 0;

    FILE* tree_log_graph = fopen ("graph/graph_log_tree.dot", "w");

    fprintf (tree_log_graph, "digraph G\n{\n");
    fprintf (tree_log_graph, "\tgraph [dpi = 250];\n");
    fprintf (tree_log_graph, "\trankdir = TB;\n");

    int count = 0;
    knot_graph (tree, tree->root, tree_log_graph, &count);

    fprintf (tree_log_graph, "\n}");

    fclose (tree_log_graph);

    char command[MAX_LEN_STR] = "";
    sprintf (command, "dot -Tpng -ograph/graph_log_tree_%d.png graph/graph_log_tree.dot", number_of_function_launches);

    system(command);

    FILE* htm_log_file = fopen ("graph/htm_log_file.htm", "a");

    fprintf (htm_log_file, "<pre>\n");
    fprintf (htm_log_file, "<img src = \"graph_log_tree_%d.png\">", number_of_function_launches);
    fprintf (htm_log_file, "<hr>\n");

    fclose(htm_log_file);

    number_of_function_launches++;

    return number_of_function_launches - 1;
}