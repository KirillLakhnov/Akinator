#include "akinator.h"

int main ()
{
    setlocale (LC_ALL, "ru_RU.cp1251");
    char name_database[NAME_MAX_LEN] = "database_akinator.txt";

    struct Text buffer_database = {};
    struct FileInfo file_database = {.file_name = name_database};
    struct Tree tree_akinator = {.file_database = &file_database, .buffer_database = &buffer_database};

    tree_graph_dump (&tree_akinator);

    main_menu (&tree_akinator);

    return 0;
}