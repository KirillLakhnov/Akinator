#include "akinator.h"

void main_menu (struct Tree* tree)
{
    screen_clear ();

    static int work_synthesizer_main = 1;

    printf ("Выбери соответствующий режим игры\n"
            "[" RED_TEXT(1) "] Отгадывание\n"
            "[" RED_TEXT(2) "] Определения\n"
            "[" RED_TEXT(3) "] Сравнения обьектов\n"
            "[" RED_TEXT(4) "] Просмотреть базу данных\n"
            "[" RED_TEXT(5) "] Выход из программы\n");

    SPEECH_SYNTHESIZER (work_synthesizer_main, "А саламаалейкум брад. Это Акинатор! Выбери соответствующий режим игры!");

    work_synthesizer_main = 0;

    tree_ctor (tree);
    tree_creater (tree);

    processing_selected_mode (tree);
}

void processing_selected_mode (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    int mode = get_command();

    switch (mode)
    {
        case COMMAND_1:          
            guessing_menu (tree);          
            break;
        case COMMAND_2:        
            definition_menu (tree);        
            break;
        case COMMAND_3: 
            object_comparison_menu (tree); 
            break;
        case COMMAND_4:
            graph_open (tree);
            screen_clear ();
            main_menu (tree);
            break;
        case COMMAND_5:
            screen_clear ();
            tree_dtor (tree);
            SPEECH_SYNTHESIZER (work_synthesizer, "Прощай, плак плак");
            return;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
            break;
    }
}

//---------------------------------------------------------------------------------

void guessing_menu (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    screen_clear ();

    printf ("Ну что ж, давай я попробую отгадать твоё слово.\n");
    SPEECH_SYNTHESIZER (work_synthesizer, "Ну что ж, давай я попробую отгадать твоё слово.");

    struct Knot* current_knot = tree->root;

    guessing_mode (tree, current_knot);
}

void guessing_mode (struct Tree* tree, struct Knot* current_knot)
{
    ASSERT_OK_TREE (tree);

    screen_clear ();

    printf ("Это %s? ["  RED_TEXT(Y)"/" RED_TEXT(N) "]\n", current_knot->string);

    SPEECH_SYNTHESIZER (work_synthesizer, "Это %s", current_knot->string);
    
    processing_selected_response (tree, current_knot);

}

void processing_selected_response (struct Tree* tree, struct Knot* current_knot)
{
    assert (current_knot);
    ASSERT_OK_TREE (tree);

    int answer = get_command();

    switch (answer)
    {
        case ANSWER_YES:
            if (current_knot->left == nullptr)
            {
                screen_clear ();

                printf ("Юхху! Я победил! Вы не смогли меня обыграть!\n");
                SPEECH_SYNTHESIZER (work_synthesizer, "Юхху! Я победил! Вы не смогли меня обыграть!");

                menu_after_game (tree);
            }
            else
            {
                guessing_mode (tree, current_knot->left);
            }                    
            break;

        case ANSWER_NO:
            if (current_knot->right == nullptr)
            {
                screen_clear ();

                printf ("Эх, в этот раз не вышло. Кто же это был? Ваше слово: ");
                SPEECH_SYNTHESIZER (work_synthesizer, "Эх, в этот раз не вышло. Кто же это был?");

                char new_object[MAX_STR_SIZE] = "";
                input_word (new_object);

                if (tree_search (tree, new_object) != 0)
                {
                    printf ("Введите отличительный признак вашего слова от \"%s\": ", current_knot->string);
                    SPEECH_SYNTHESIZER (work_synthesizer, "Введите отличительный признак вашего слова");

                    char distinctive_property[MAX_STR_SIZE] = "";
                    input_word (distinctive_property);

                    update_tree (tree, current_knot, new_object, distinctive_property);
                }
                else
                {
                    printf ("Объект \"%s\" был найден в базе данных \"%s\"\n", new_object, tree->file_database->file_name);
                }

                menu_after_game (tree);
            }
            else
            {
                guessing_mode (tree, current_knot->right);
                // заменить рекурсию циклом
            }          
            break;

        default: 
            printf("Неверный ответ %c, попробуй еще раз\n", answer);
            processing_selected_response (tree, current_knot);
            break;
    }

}

void processing_selected_mode_after_game (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    int mode = get_command ();

    switch (mode)
    {
        case COMMAND_1:
            guessing_menu (tree);          
            break;
        case COMMAND_2:
            update_base (tree);
            menu_after_game (tree);
            break;
        case COMMAND_3:
            main_menu (tree);
            break;
        default:
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode_after_game (tree);
            break;
    }
}

void menu_after_game (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    printf ("\nХотите продолжить?\n"
            "[" RED_TEXT (1) "]" "Начать заново\n"
            "[" RED_TEXT (2) "]" "Сохранить изменения базы данных?\n"
            "[" RED_TEXT (3) "]" "Выйти в меню\n");

    SPEECH_SYNTHESIZER (work_synthesizer, " Хотите играть в угадайку дальше? Или попробовать что-то другое?");

    processing_selected_mode_after_game (tree);

}

//---------------------------------------------------------------------------------

void definition_menu (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    screen_clear ();

    printf ("Введите слово, определение которого хотите посмотреть: ");
    SPEECH_SYNTHESIZER (work_synthesizer, "Введите слово, определение которого хотите посмотреть.");

    definition_mode (tree);
}

void definition_mode (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    screen_clear ();

    char object[MAX_STR_SIZE] = "";
    input_word (object);

    struct Stack* path_element = tree_search(tree, object);
    if (path_element == nullptr)
    {
        printf ("ERROR");
    }
        
    printf ("\n%lu\n",path_element->size);
    if (path_element->size == 0)
    {
        screen_clear ();
        printf ("Объект \"%s\" не найден в базе данных \"%s\"\n", object, tree->file_database->file_name);

        SPEECH_SYNTHESIZER (work_synthesizer, "объек %s не найден в базе данных", object);
    }
    else
    {
        screen_clear ();

        printf ("%s - это", object);

        SPEECH_SYNTHESIZER (work_synthesizer, "%s - это", object);

        for (int i = 0; i < path_element->size - 2; i++)
        {   
            if ((path_element->data)[i]->left == (path_element->data)[i + 1])
            {
                printf(" %s,", (path_element->data)[i]->string);

                SPEECH_SYNTHESIZER (work_synthesizer, "%s", (path_element->data)[i]->string);
            }

            if ((path_element->data)[i]->right == (path_element->data)[i + 1])
            {
                printf(" не %s,", (path_element->data)[i]->string);

                SPEECH_SYNTHESIZER (work_synthesizer, "не %s", (path_element->data)[i]->string);
            }
        }

        if ((path_element->data)[path_element->size - 2]->left  == (path_element->data)[path_element->size - 1])
            {
                printf(" %s.\n", (path_element->data)[path_element->size - 2]->string);

                SPEECH_SYNTHESIZER (work_synthesizer, "%s", (path_element->data)[path_element->size - 2]->string);
            }

        if ((path_element->data)[path_element->size - 2]->right == (path_element->data)[path_element->size - 1])
        {
            printf(" не %s.\n", (path_element->data)[path_element->size - 2]->string);

            SPEECH_SYNTHESIZER (work_synthesizer, "не %s", (path_element->data)[path_element->size - 2]->string);
        }
    }

    StackDtor (path_element);
    if (path_element != nullptr)
    {
        free (path_element);
        path_element = nullptr;
    }

    menu_after_definition (tree);
}

void menu_after_definition (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    printf ("\nХотите продолжить?\n"
            "[" RED_TEXT (1) "]" "Продолжить смотреть опредления\n"
            "[" RED_TEXT (2) "]" "Выйти в меню\n");

    SPEECH_SYNTHESIZER (work_synthesizer, "Хотите узнавать кто есть кто? Или попробуете что-то другое?");

    processing_selected_mode_after_definition (tree);
}

void processing_selected_mode_after_definition (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    int mode = get_command ();

    switch (mode)
    {
        case COMMAND_1:
            definition_menu (tree);          
            break;
        case COMMAND_2:
            main_menu (tree);
            break;
        default:
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode_after_game (tree);
            break;
    }
}

//---------------------------------------------------------------------------------

void object_comparison_menu (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    screen_clear ();

    printf ("Введите 2 слова, которые хотите сравнить:\n");
    SPEECH_SYNTHESIZER (work_synthesizer, "Введите 2 слова, которые хотите сравнить");

    object_comparison_mode (tree);
}

void object_comparison_mode (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    char object_1[MAX_STR_SIZE] = "";
    char object_2[MAX_STR_SIZE] = "";

    printf("1. ");
    input_word (object_1);

    printf("2. ");
    input_word (object_2);

    struct Stack* path_element_1 = tree_search(tree, object_1);
    struct Stack* path_element_2 = tree_search(tree, object_2);

    if (path_element_1->size == 0)
    {
        screen_clear ();
        printf ("Объект \"%s\" не найден в базе данных \"%s\"\n", object_1, tree->file_database->file_name);

        SPEECH_SYNTHESIZER (work_synthesizer, "объект %s не найден в базе данных", object_1);
    }
    else if (path_element_2->size == 0)
    {
        screen_clear ();
        printf ("Объект \"%s\" не найден в базе данных \"%s\"\n", object_2, tree->file_database->file_name);

        SPEECH_SYNTHESIZER (work_synthesizer, "объект %s не найден в базе данных", object_2);
    }
    else
    {
        screen_clear();

        if (((path_element_1->data)[1] == (path_element_2->data)[1]) && 
            (0 < path_element_1->size - 1) && (0 < path_element_2->size - 1))
        {
            printf ("%s и %s схожи тем, что", object_1, object_2);
            SPEECH_SYNTHESIZER (work_synthesizer, "%s и %s схожи тем, что", object_1, object_2);
        }
        else
        {
            printf ("%s и %s ничем не схожи", object_1, object_2);
            SPEECH_SYNTHESIZER (work_synthesizer, "%s и %s ничем не схожи", object_1, object_2);
        }

        int index_1 = 0;
        int index_2 = 0;

        while (((path_element_1->data)[index_1 + 1] == (path_element_2->data)[index_1 + 1]) && 
               (index_1 < path_element_1->size - 1) && (index_2 < path_element_2->size - 1))
        {   
            if ((path_element_1->data)[index_1]->left  == (path_element_1->data)[index_1 + 1])
            {
                printf(" %s,", (path_element_1->data)[index_1]->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "%s", (path_element_1->data)[index_1]->string);
            }
            else if ((path_element_1->data)[index_1]->right == (path_element_1->data)[index_1 + 1])
            {
                printf(" не %s,", (path_element_1->data)[index_1]->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "не %s", (path_element_1->data)[index_1]->string);
            }

            index_1++;
            index_2++;
        }

        printf("\n%s отличается тем, что", object_1);
        SPEECH_SYNTHESIZER (work_synthesizer, "%s отличается тем, что", object_1);
        
        while (index_1 < path_element_1->size - 1)
        {   
            if ((path_element_1->data)[index_1]->left == (path_element_1->data)[index_1 + 1])
            {
                printf(" %s,", (path_element_1->data)[index_1]->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "%s", (path_element_1->data)[index_1]->string);
            }
            else if ((path_element_1->data)[index_1]->right == (path_element_1->data)[index_1 + 1])
            {
                printf(" не %s,", (path_element_1->data)[index_1]->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "не %s", (path_element_1->data)[index_1]->string);
            }

            index_1++;
        }

        printf("\n%s отличается тем, что", object_2);
        SPEECH_SYNTHESIZER (work_synthesizer, "%s отличается тем, что", object_2);

        while (index_2 < path_element_2->size - 2)
        {
            if ((path_element_2->data)[index_2]->left  == (path_element_2->data)[index_2 + 1])
            {
                printf(" %s,", (path_element_2->data)[index_2]->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "%s", (path_element_2->data)[index_2]->string);
            }

            else if ((path_element_2->data)[index_2]->right == (path_element_2->data)[index_2 + 1])
            {
                printf(" не %s,", (path_element_2->data)[index_2]->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "не %s", (path_element_2->data)[index_2]->string);
            }

            index_2++;
        }

        if ((path_element_2->data)[index_2]->left  == (path_element_2->data)[index_2 + 1])
        {
            printf(" %s.\n", (path_element_2->data)[index_2]->string);
            SPEECH_SYNTHESIZER (work_synthesizer, "%s", (path_element_2->data)[index_2]->string);
        }

        if ((path_element_2->data)[index_2]->right == (path_element_2->data)[index_2 + 1])
        {
            printf(" не %s.\n", (path_element_2->data)[index_2]->string);
            SPEECH_SYNTHESIZER (work_synthesizer, "не %s", (path_element_2->data)[index_2]->string);
        }
    }

    StackDtor (path_element_1);
    if (path_element_1 != nullptr)
    {
        free (path_element_1);
        path_element_1 = nullptr;
    }

    StackDtor (path_element_2);
    if (path_element_2 != nullptr)
    {
        free (path_element_2);
        path_element_2 = nullptr;
    }

    menu_after_comparison (tree);
}

void menu_after_comparison (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    printf ("\nХотите продолжить?\n"
            "[" RED_TEXT (1) "]" "Продолжить сравнивать объекты\n"
            "[" RED_TEXT (2) "]" "Выйти в меню\n");

    SPEECH_SYNTHESIZER (work_synthesizer, "Хотите узнавать кто есть кто? Или попробуете что-то другое?");

    processing_selected_mode_after_comparison (tree);
}

void processing_selected_mode_after_comparison (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    int mode = get_command ();

    switch (mode)
    {
        case COMMAND_1:
            object_comparison_menu (tree);         
            break;
        case COMMAND_2:
            main_menu (tree);
            break;
        default:
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode_after_game (tree);
            break;
    }
}

//---------------------------------------------------------------------------------

int get_command ()
{
    struct termios oldt = {};
    struct termios newt = {};

    int command = 0;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    command = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return command;
}

int get_word (char* word)
{
    assert (word);

    if (fgets (word, MAX_STR_SIZE, stdin) != nullptr)
    {
        char* find_symbol = strchr (word, '\n');

        if (find_symbol != nullptr)
        {
            *find_symbol = '\0';
        }

        return GOOD_WORKING;
    }

    return ERROR_GETS;
}

void input_word (char* word)
{   
    assert (word);

    while (get_word (word) != GOOD_WORKING)
    {
        printf ("Ввод был не корректен, попробуйте еще раз: ");
    }
    printf ("Ввод был завершен.\n");
}

void graph_open (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    int number_graph = tree_graph_dump (tree);
    char command [MAX_STR_SIZE] = "";
    snprintf (command, MAX_STR_SIZE, "open graph/graph_log_tree_%d.png", number_graph);
    system (command);
}

//---------------------------------------------------------------------------------

void update_tree (struct Tree* tree, struct Knot* current_knot, char* new_object, char* distinctive_property)
{
    ASSERT_OK_TREE (tree);
    assert (current_knot);
    assert (new_object);
    assert (distinctive_property);

    struct Knot* new_knot = (Knot*) calloc (1, sizeof (Knot));
    if (new_knot == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;
        ASSERT_OK_TREE (tree);
    }

    new_knot->length = strlen(distinctive_property) + 1;
    new_knot->string = (char*) calloc (new_knot->length, sizeof(char));
    if (new_knot->string == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;
        ASSERT_OK_TREE (tree);
    }

    strcpy (new_knot->string, distinctive_property);

    new_knot->prev = current_knot->prev;
    new_knot->right = current_knot;

    if (current_knot->prev->left == current_knot)
    {
        current_knot->prev->left = new_knot;
    }
    else if (current_knot->prev->right == current_knot)
    {
        current_knot->prev->right = new_knot;
    }

    current_knot->prev = new_knot;

    new_knot->left = (Knot*) calloc (1, sizeof (Knot));

    new_knot->left->length = strlen(new_object) + 1;
    new_knot->left->string = (char*) calloc (new_knot->left->length, sizeof(char));
    if (new_knot->left == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;
        ASSERT_OK_TREE (tree);
    }

    strcpy (new_knot->left->string, new_object);

    new_knot->left->prev = new_knot;
    new_knot->left->right = nullptr;
    new_knot->left->left = nullptr;

    ASSERT_OK_TREE (tree);
}

void update_base_print (FILE* base, struct Knot* knot_root)
{
    assert (base);
    assert (knot_root);

    fprintf (base, "{\n");

    if (!knot_root)
    {
        return;
    }
    fprintf (base, "\"%s\"\n", knot_root->string);
    if (knot_root->left)
    {
        update_base_print (base, knot_root->left);
    }
    
    if (knot_root->right)
    {
        update_base_print (base, knot_root->right);
    }

    fprintf (base, "}\n");
}

void update_base (struct Tree* tree)
{
    ASSERT_OK_TREE (tree);

    FILE* base = fopen (tree->file_database->file_name, "w");
    update_base_print (base, tree->root);
    fclose (base);

    ASSERT_OK_TREE (tree);
}

//---------------------------------------------------------------------------------

struct Stack* tree_search (struct Tree* tree, const char* object)
{
    ASSERT_OK_TREE (tree);
    assert (object);

    struct Stack* path_element = (struct Stack*) calloc (1, sizeof(struct Stack));
    //Зачем калочить структуру
    if (path_element == nullptr)
    {
        printf ("Error calloc in tree.cpp on line = %d", __LINE__);
        tree->code_error |= TREE_ERROR_CALLOC;
        ASSERT_OK_TREE (tree);
    }

    StackCtor (path_element, 10);

    node_search(object, tree->root, path_element);

    ASSERT_OK_TREE (tree);
    
    return path_element;
}

int node_search(const char* object, struct Knot* current_knot, struct Stack* path_element)
{
    assert (object);
    assert (current_knot);
    assert (path_element);

    StackPush(path_element, current_knot);

    if (strcmp(object, current_knot->string) == 0) 
    {
        return 1;
    }
    else
    {
        if (current_knot->left != nullptr && current_knot->right != nullptr)
        {
            if (node_search (object, current_knot->left, path_element) || node_search (object, current_knot->right, path_element))
            {
                return 1;
            }
            else
            {
                StackPop(path_element);
                return 0;
            }
        }
        else if (current_knot->left == nullptr && current_knot->right == nullptr)
        {
            StackPop(path_element);
            return 0;
        }

        return 0;
    }
}
