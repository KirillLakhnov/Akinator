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

    printf ("Введите слово, определение которого хотите посмотреть.");
    SPEECH_SYNTHESIZER (work_synthesizer, "Введите слово, определение которого хотите посмотреть.", 0);

    definition_mode (tree);
}

void definition_mode (struct Tree* tree)
{
    screen_clear ();


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
        case COMMAND_GUESSING:          
            guessing_menu (tree);          
            break;
        case COMMAND_DEFINITION:        
            definition_menu (tree);        
            break;
        case COMMAND_OBJECT_COMPARISON: 
            object_comparison_menu (tree); 
            break;
        case COMMAND_EXITING_PROGRAMM:
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
                while (get_word (new_object) != GOOD_WORKING)
                {
                    printf ("Ввод был не корректен, попробуйте еще раз. Ваше слово: ");
                }
                printf ("Ввод был завершен.\n");

//--------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------Проверка на наличие такого же объекта в дереве
                printf ("Введите отличительный признак вашего слова от \"%s\": ", current_knot->string);
                SPEECH_SYNTHESIZER (work_synthesizer, "Введите отличительный признак вашего слова", 0);

                char distinctive_property[MAX_STR_SIZE] = "";
                while (get_word (distinctive_property) != GOOD_WORKING)
                {
                    printf ("Ввод был не корректен, попробуйте еще раз. Отличительный признак: ");
                }
                printf ("Ввод был завершен.\n");

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
    case COMMAND_AGAIN:
        guessing_menu (tree);          
        break;
    case COMMAND_UPDATE_BASE:
        update_base (tree);
        menu_after_game (tree);
        break;
    case COMMAND_OUTPUT_MAIN:
        main_menu (tree);
        break;
    default:
        printf("Неверный режим %c, попробуй еще раз\n", mode);
        processing_selected_mode_after_game (tree);
        break;
    }
}

int get_command()
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

void tree_search (struct Tree* tree, const char* object)
{
    
}
