#include "akinator.h"

extern int work_synthesizer = 0;

void main_menu (struct Tree* tree)
{
    screen_clear ();

    printf ("Выбери соответствующий режим игры\n"
            "[" RED_TEXT(1) "] Отгадывание\n"
            "[" RED_TEXT(2) "] Определения\n"
            "[" RED_TEXT(3) "] Сравнения обьектов\n"
            "[" RED_TEXT(4) "] Выход из программы\n");

    SPEECH_SYNTHESIZER (work_synthesizer, "А саламаалейкум брад. Это Акинатор! Выбери соответствующий режим игры!", 0);

    processing_selected_mode (tree);
}

void guessing_menu (struct Tree* tree)
{
    screen_clear ();

    printf ("Ну что ж, давай я попробую отгадать твоё слово.\n");
    SPEECH_SYNTHESIZER (work_synthesizer, "Ну что ж, давай я попробую отгадать твоё слово.", 0);

    struct Knot* current_knot = tree->root;

    guessing_mode (tree, current_knot);
}

void guessing_mode (struct Tree* tree, struct Knot* current_knot)
{
    screen_clear ();

    printf ("Это %s? ["  RED_TEXT(Y)"/" RED_TEXT(N) "]\n", current_knot->string);

    SPEECH_SYNTHESIZER (work_synthesizer, current_knot->string, 1);
    
    processing_selected_response (tree, current_knot);
}

void definition_menu (struct Tree* tree)
{
    screen_clear ();

    printf ("Введите слово, определение которого хотите посмотреть: ");
    SPEECH_SYNTHESIZER (work_synthesizer, "Введите слово, определение которого хотите посмотреть.", 0);

    definition_mode (tree);
}

void definition_mode (struct Tree* tree)
{
    screen_clear ();

    char object[MAX_STR_SIZE] = "";
    input_word (object);

    struct Stack* path_element = tree_search(tree, object);

    printf ("\n%lu\n",path_element->size);
    if (path_element->size == 0)
    {
        screen_clear ();
        printf ("Объект \"%s\" не найден в базе данных \"%s\"\n", object, tree->file_database->file_name);
    }
    else
    {
        screen_clear ();

        printf ("%s - это", object);

        for (int i = 0; i < path_element->size - 2; i++)
        {   
            if ((path_element->data)[i]->left == (path_element->data)[i + 1])
            {
                printf(" %s,", (path_element->data)[i]->string);
            }

            if ((path_element->data)[i]->right == (path_element->data)[i + 1])
            {
                printf(" не %s,", (path_element->data)[i]->string);
            }
        }

        if ((path_element->data)[path_element->size - 2]->left  == (path_element->data)[path_element->size - 1])
            {
                printf(" %s.\n", (path_element->data)[path_element->size - 2]->string);
            }

        if ((path_element->data)[path_element->size - 2]->right == (path_element->data)[path_element->size - 1])
        {
            printf(" не %s.\n", (path_element->data)[path_element->size - 2]->string);
        }
    }

    StackDtor (path_element);
    free (path_element);

    menu_after_definition (tree);
}

void object_comparison_menu (struct Tree* tree)
{
    screen_clear ();

}

void processing_selected_mode (struct Tree* tree)
{
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
            screen_clear ();
            SPEECH_SYNTHESIZER (work_synthesizer, "Прощай, плак плак", 0);
            return;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
            break;
    }
}

void processing_selected_response (struct Tree* tree, struct Knot* current_knot)
{
    int answer = get_command();

    switch (answer)
    {
        case ANSWER_YES:
            if (current_knot->left == nullptr)
            {
                screen_clear ();

                printf ("Юхху! Я победил! Вы не смогли меня обыграть!\n");
                SPEECH_SYNTHESIZER (work_synthesizer, "Юхху! Я победил! Вы не смогли меня обыграть!", 0);

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
                SPEECH_SYNTHESIZER (work_synthesizer, "Эх, в этот раз не вышло. Кто же это был?", 0);

                char new_object[MAX_STR_SIZE] = "";
                input_word (new_object);

//--------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------Проверка на наличие такого же объекта в дереве
                printf ("Введите отличительный признак вашего слова от \"%s\": ", current_knot->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "Введите отличительный признак вашего слова", 0);

                char distinctive_property[MAX_STR_SIZE] = "";
                input_word (distinctive_property);

//--------------------------------------------------------------------------------------------------------------
                update_tree (tree, current_knot, new_object, distinctive_property);

                menu_after_game (tree);
            }
            else
            {
                guessing_mode (tree, current_knot->right);
            }          
            break;
        default: 
            printf("Неверный ответ %c, попробуй еще раз\n", answer);
            processing_selected_response (tree, current_knot);
            break;
    }
}

void menu_after_game (struct Tree* tree)
{
    system ("sleep 2");
    screen_clear ();

    printf ("Хотите продолжить?\n"
            "[" RED_TEXT (1) "]" "Начать заново\n"
            "[" RED_TEXT (2) "]" "Сохранить изменения базы данных?\n"
            "[" RED_TEXT (3) "]" "Выйти в меню\n");

    SPEECH_SYNTHESIZER (work_synthesizer, "Хотите играть в угадайку дальше? Или попробовать что-то другое?", 0);

    processing_selected_mode_after_game (tree);
}

void processing_selected_mode_after_game (struct Tree* tree)
{
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

void menu_after_definition (struct Tree* tree)
{
    system ("sleep 2");
    screen_clear ();

    printf ("Хотите продолжить?\n"
            "[" RED_TEXT (1) "]" "Продолжить смотреть опредления\n"
            "[" RED_TEXT (2) "]" "Выйти в меню\n");

    SPEECH_SYNTHESIZER (work_synthesizer, "Хотите узнавать кто есть кто? Или попробуете что-то другое?", 0);

    processing_selected_mode_after_definition (tree);
}

void processing_selected_mode_after_definition (struct Tree* tree)
{
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
    while (get_word (word) != GOOD_WORKING)
    {
        printf ("Ввод был не корректен, попробуйте еще раз. Отличительный признак: ");
    }
    printf ("Ввод был завершен.\n");
}

//---------------------------------------------------------------------------------

void update_tree (struct Tree* tree, struct Knot* current_knot, char* new_object, char* distinctive_property)
{
    struct Knot* new_knot = (Knot*) calloc (1, sizeof (Knot));

    new_knot->lenght = strlen(distinctive_property) + 1;
    new_knot->string = (char*) calloc (new_knot->lenght, sizeof(char));
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

    new_knot->left->lenght = strlen(new_object) + 1;
    new_knot->left->string = (char*) calloc (new_knot->left->lenght, sizeof(char));
    strcpy (new_knot->left->string, new_object);

    new_knot->left->prev = new_knot;
    new_knot->left->right = nullptr;
    new_knot->left->left = nullptr;
}

void update_base_print (FILE* base, struct Knot* knot_root)
{
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
    FILE* base = fopen (tree->file_database->file_name, "w");
    update_base_print (base, tree->root);
    fclose (base);
}

//---------------------------------------------------------------------------------

struct Stack* tree_search (struct Tree* tree, const char* object)
{
    struct Stack* path_element = (struct Stack*) calloc (1, sizeof(struct Stack));

    StackCtor (path_element, 10);

    node_search(object, tree->root, path_element);
    
    return path_element;
}

int node_search(const char* object, struct Knot* current_knot, struct Stack* path_element)
{
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
