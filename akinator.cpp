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

    SPEECH_SYNTHESIZER (work_synthesizer, "Здарова. Это Акинатор! Выбери соответствующий режим игры!");

    processing_selected_mode (tree);
}

void guessing_menu (struct Tree* tree)
{
    screen_clear ();

    printf ("Ну что ж, давай я попробую отгадать твоё слово.\n");
    SPEECH_SYNTHESIZER (work_synthesizer, "Ну что ж, давай я попробую отгадать твоё слово.");

    struct Knot* current_knot = tree->root;

    guessing_mode (current_knot);


}

void guessing_mode (struct Knot* current_knot)
{
    screen_clear ();

    printf ("Это %s ["  RED_TEXT(Y)"/" RED_TEXT(N) "]\n", current_knot->string);
    
    processing_selected_response (current_knot);
}

void definition_menu ()
{
    screen_clear ();

}

void object_comparison_menu ()
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
            definition_menu ();        
            break;
        case COMMAND_OBJECT_COMPARISON: 
            object_comparison_menu (); 
            break;
        case COMMAND_EXITING_PROGRAMM:
            screen_clear ();
            SPEECH_SYNTHESIZER (work_synthesizer, "Прощай, плак плак");
            return;
        default: 
            printf("Неверный режим %c, попробуй еще раз\n", mode);
            processing_selected_mode (tree); 
            break;
    }
}

void processing_selected_response (struct Knot* current_knot)
{
    int answer = get_command();

    switch (answer)
    {
        case ANSWER_YES:
            if (current_knot->left == nullptr)
            {
                printf ("Юхху! Я победил! Вы не смогли меня обыграть!\n");
                SPEECH_SYNTHESIZER (work_synthesizer, "Юхху! Я победил! Вы не смогли меня обыграть!");
            }
            else
            {
                guessing_mode (current_knot->left);
            }                    
            break;
        case ANSWER_NO:
            if (current_knot->right == nullptr)
            {
                printf ("Эх, в этот раз не вышло. Кто же это был?\n");
                SPEECH_SYNTHESIZER (work_synthesizer, "Эх, в этот раз не вышло. Кто же это был?");
            }
            else
            {
                guessing_mode (current_knot->right);
            }          
            break;
        default: 
            printf("Неверный ответ %c, попробуй еще раз\n", answer);
            processing_selected_response (current_knot);
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
