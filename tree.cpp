#include "tree.h"

void tree_ctor (struct Tree* tree)
{
    BufferCreater (tree->file_database, tree->buffer_database);

    tree->size = 0;
    tree->code_error = 0;

    tree->current_position_buffer = tree->buffer_database->file_buffer;

    tree->root = knot_creater (tree, nullptr);
}

void tree_dtor (struct Tree* tree)
{
    TextDtor (tree->buffer_database);

    tree->file_database->file_name = nullptr;

    tree->code_error = DTOR_SIZE_T;
    tree->size = DTOR_SIZE_T;
}

void tree_creater (struct Tree* tree)
{
    struct Knot* current_knot = tree->root;

    char* bracket = (char*) calloc (2, sizeof(char));

    while (tree->buffer_database->file_buffer + tree->buffer_database->size_buffer > tree->current_position_buffer)
    {
        int lenght = 0;
        sscanf (tree->current_position_buffer, "%s %n", bracket, &lenght);

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
    }

    free (bracket);
}

struct Knot* knot_creater (struct Tree* tree, struct Knot* prev)
{
    struct Knot* current_knot = (Knot*) calloc (1, sizeof (Knot));

    tree->size++;
    current_knot->prev = prev;

    tree->current_position_buffer = strchr (tree->current_position_buffer, '{');
    if (tree->current_position_buffer == nullptr)
    {
        tree->code_error |=  TREE_ERROR_SYNTAX_IN_BASE;
        abort ();
    }

    char* pointer_begin = strchr (tree->current_position_buffer, '"');
    if (pointer_begin == nullptr)
    {
        tree->code_error |=  TREE_ERROR_SYNTAX_IN_BASE;
        abort ();
    }

    char* pointer_end = strchr (pointer_begin + 1, '"');
    if (pointer_end == nullptr)
    {
        tree->code_error |=  TREE_ERROR_SYNTAX_IN_BASE;
        abort ();
    }

    current_knot->lenght = pointer_end - pointer_begin;
    current_knot->string = (char*) calloc (current_knot->lenght, sizeof (char));
    
    *pointer_end = '\0';
    strcpy (current_knot->string, pointer_begin + 1);

    tree->current_position_buffer = pointer_end + 1;

    return current_knot;
}

void tree_search ()
{

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

void tree_graph_node (struct Knot* knot, FILE* tree_log_graph)
{
    static int number_node = 0;

    if (!knot)
    {
        return;
    }
    fprintf(tree_log_graph, "\t\"NOD_%d\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
		                    "\n\t\tlabel = \"prev = %p \\lcurrent = %p | {<left> left = %p| %s | <right> right = %p}\"]\n", 
                            number_node, knot->prev, knot, knot->left, knot->string, knot->string);
    number_node++;

    if (knot->left)
    {
        tree_graph_node (knot->left, tree_log_graph);
    }
    
    if (knot->right)
    {
        tree_graph_node (knot->right, tree_log_graph);
    }
}

void tree_graph_communications (struct Knot* knot, FILE* tree_log_graph)
{
    static int number_communications = 0;

    if (!knot)
    {
        return;
    }
    if (knot->left != nullptr)
    {
        fprintf (tree_log_graph,    "\t\"NOD_%d\" -> \"NOD_%d\"[style = \"bold\", color = \"#df1b1bdf\"]\n", 
                                    number_communications, number_communications + 1);
        number_communications++;
    }
    else
    {
        number_communications--;
        fprintf (tree_log_graph,    "\t\"NOD_%d\" -> \"NOD_%d\"[style = \"bold\", color = \"#df1b1bdf\"]\n", 
                                    number_communications, number_communications+2);
        number_communications += 2;
    }

    if (knot->left)
    {
        tree_graph_communications (knot->left, tree_log_graph);
    }
    
    if (knot->right)
    {
        tree_graph_communications (knot->right, tree_log_graph);
    }
}

int tree_graph_dump (struct Tree* tree)
{
    static int number_of_function_launches = 0;

    FILE* tree_log_graph = fopen ("graph_log_tree.dot", "w");
    if (tree_log_graph == nullptr)
    {
        printf ("ERROR FOPEN on line %d in list.cpp", __LINE__);
        return ERROR_FILE_CLOSE;
    }

    fprintf (tree_log_graph, "digraph G\n{\n");
    fprintf (tree_log_graph, "\tgraph [dpi = 300];\n");
    fprintf (tree_log_graph, "\trankdir = TB;\n");

    tree_graph_node (tree->root, tree_log_graph);
    tree_graph_communications (tree->root, tree_log_graph);

    fprintf (tree_log_graph, "\n}");

    fclose (tree_log_graph);

    char command[100] = "";
    sprintf (command, "dot -Tpng -ograph_log_tree_%d.png graph_log_tree.dot", number_of_function_launches);

    system(command);

    FILE* htm_log_file = fopen ("htm_log_file.htm", "a");

    fprintf (htm_log_file, "<pre>\n");
    fprintf (htm_log_file, "<img src = \"graph_log_tree_%d.png\">", number_of_function_launches);
    fprintf (htm_log_file, "<hr>\n");

    fclose(htm_log_file);

    number_of_function_launches++;

    return GOOD_WORKING;
}