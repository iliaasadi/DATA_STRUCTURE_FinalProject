#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


enum Column {
    STUDENT_NUMBER,
    CORE_COURSE_SCORE,
    GENERAL_COURSE_SCORE,
    GENERAL_COURSE_NAME,
    CORE_COURSE_INSTRUCTOR,
    GENERAL_COURSE_INSTRUCTOR,
    CORE_COURSE_NAME,
    UNKNOWN
};

//core_course_score general_course_score could be 'float'

typedef struct Record {
    long student_number;
    char general_course_name[32];
    char general_course_instructor[32];
    int general_course_score;
    char core_course_name[32];
    char core_course_instructor[32];
    int core_course_score;
    struct Record *prev, *next;
} Record;

typedef struct Table {
    char name[32];
    Record *head;
    Record *tail;
} Table;

typedef struct Database {
    Table *tables[30];
    int table_count;
} Database;

Table *create_table(const char *name);

Table *find_table(Database *db, const char *name) {
    for (int i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i]->name, name) == 0) {
            return db->tables[i];
        }
    }
    return NULL;
}

Database *create_database() {
    Database *db = (Database *) malloc(sizeof(Database));
    db->table_count = 0;
    return db;
}

Table *create_table(const char *name) {
    Table *table = (Table *) malloc(sizeof(Table));
    strncpy(table->name, name, 32);
    table->head = NULL;
    table->tail = NULL;
    return table;
}

void add_table(Database *db, const char *name) {
    Table *table = create_table(name);
    if (find_table(db, name) != NULL) {
        printf("Duplicated Table.\n");
        return;
    }
    db->tables[db->table_count] = table;
    db->table_count++;
    printf("Table created.\n");
}

int is_student_existed(Table *table, long student_number) {
    Record *current = table->head;
    while (current != NULL) {
        if (current->student_number == student_number) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void delete_table(Database *db, const char *name) {
    int i;
    for (i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i]->name, name) == 0) {
            free(db->tables[i]);
            db->table_count--;
            printf("Table deleted.\n");
            return;
        }
    }
    printf("No Table.\n");
}

void print_record(Record *record) {
    printf("\nStudent Number: %ld\n", record->student_number);
    printf("General Course Name: %s\n", record->general_course_name);
    printf("General Course Instructor: %s\n", record->general_course_instructor);
    printf("General Course Score: %d\n", record->general_course_score);
    printf("Core Course Name: %s\n", record->core_course_name);
    printf("Core Course Instructor: %s\n", record->core_course_instructor);
    printf("Core Course Score: %d\n\n", record->core_course_score);
}

void add_record(Table *table, char *student_number, const char *general_course_name,
                const char *general_course_instructor, char *general_course_score,
                const char *core_course_name, const char *core_course_instructor, char *core_course_score) {

    if (!is_student_existed(table, strtol(student_number, NULL, 10))) {
        printf("Duplicated Student number.\n");
        return;
    }
    Record *new_record = (Record *) malloc(sizeof(Record));
    new_record->student_number = strtol(student_number, NULL, 10);
    strncpy(new_record->general_course_name, general_course_name, 32);
    strncpy(new_record->general_course_instructor, general_course_instructor, 32);
    new_record->general_course_score = atoi(general_course_score);
    strncpy(new_record->core_course_name, core_course_name, 32);
    strncpy(new_record->core_course_instructor, core_course_instructor, 32);
    new_record->core_course_score = atoi(core_course_score);

    new_record->prev = NULL;
    new_record->next = NULL;

    if (table->head == NULL) {
        table->head = new_record;
        table->tail = new_record;
    } else {
        new_record->prev = table->tail;
        table->tail->next = new_record;
        table->tail = new_record;
    }

    printf("Record added.\n");
}

int find_match(Record *current, const char *column_name, const char *value) {

    enum Column column;
    if (strcmp(column_name, "student_number") == 0)
        column = STUDENT_NUMBER;
    else if (strcmp(column_name, "general_course_name") == 0)
        column = GENERAL_COURSE_NAME;
    else if (strcmp(column_name, "general_course_instructor") == 0)
        column = GENERAL_COURSE_INSTRUCTOR;
    else if (strcmp(column_name, "general_course_score") == 0)
        column = GENERAL_COURSE_SCORE;
    else if (strcmp(column_name, "core_course_name") == 0)
        column = CORE_COURSE_NAME;
    else if (strcmp(column_name, "core_course_instructor") == 0)
        column = CORE_COURSE_INSTRUCTOR;
    else if (strcmp(column_name, "core_course_score") == 0)
        column = CORE_COURSE_SCORE;
    else
        column = UNKNOWN;

    switch (column) {
        case STUDENT_NUMBER:
            return current->student_number == atoi(value);
        case GENERAL_COURSE_NAME:
            return strcmp(current->general_course_name, value) == 0;
        case GENERAL_COURSE_INSTRUCTOR:
            return strcmp(current->general_course_instructor, value) == 0;
        case GENERAL_COURSE_SCORE:
            return current->general_course_score == atoi(value);
        case CORE_COURSE_NAME:
            return strcmp(current->core_course_name, value) == 0;
        case CORE_COURSE_INSTRUCTOR:
            return strcmp(current->core_course_instructor, value) == 0;
        case CORE_COURSE_SCORE:
            return current->core_course_score == atoi(value);
        default:
            return 0;
    }
}

void delete_record(Table *table, const char *column_name, const char *value) {
    Record *current = table->head;
    Record *prev = NULL;
    bool temp = false;
    while (current != NULL) {
        if (find_match(current, column_name, value)) {
            if (prev == NULL) {
                table->head = current->next;
            } else {
                prev->next = current->next;
            }
            if (current->next == NULL) {
                table->tail = prev;
            }
            free(current);
            temp = true;
            printf("Record deleted.\n");
        }
        prev = current;
        current = current->next;
    }
    if (!temp) {
        printf("No Record.\n");
        return;
    }

}

void
update_record(Database *db, const char *table_name, const char *column_name, const char *value, const char *new_value) {
    Table *table = find_table(db, table_name);
    if (table == NULL) {
        printf("No Table.\n");
        return;
    }

    Record *record = table->head;

    enum Column column;
    if (strcmp(column_name, "student_number") == 0)
        column = STUDENT_NUMBER;
    else if (strcmp(column_name, "general_course_name") == 0)
        column = GENERAL_COURSE_NAME;
    else if (strcmp(column_name, "general_course_instructor") == 0)
        column = GENERAL_COURSE_INSTRUCTOR;
    else if (strcmp(column_name, "general_course_score") == 0)
        column = GENERAL_COURSE_SCORE;
    else if (strcmp(column_name, "core_course_name") == 0)
        column = CORE_COURSE_NAME;
    else if (strcmp(column_name, "core_course_instructor") == 0)
        column = CORE_COURSE_INSTRUCTOR;
    else if (strcmp(column_name, "core_course_score") == 0)
        column = CORE_COURSE_SCORE;
    else
        column = UNKNOWN;

    while (record != NULL) {
        if (find_match(record, column_name, value)) {
            switch (column) {
                case STUDENT_NUMBER:
                    record->student_number = atoi(new_value);
                    break;
                case GENERAL_COURSE_NAME:
                    strncpy(record->general_course_name, new_value, 32);
                    break;
                case GENERAL_COURSE_INSTRUCTOR:
                    strncpy(record->general_course_instructor, new_value, 32);
                    break;
                case GENERAL_COURSE_SCORE:
                    record->general_course_score = atoi(new_value);
                    break;
                case CORE_COURSE_NAME:
                    strncpy(record->core_course_name, new_value, 32);
                    break;
                case CORE_COURSE_INSTRUCTOR:
                    strncpy(record->core_course_instructor, new_value, 32);
                    break;
                case CORE_COURSE_SCORE:
                    record->core_course_score = atoi(new_value);
                    break;
                default:
                    printf("Unknown column name.\n");
                    return;
            }
        }
        record = record->next;
    }

    printf("Record updated.\n");
    return;
}


Record *merge_lists(Record *first, Record *second) {
    if (first == NULL)
        return second;
    if (second == NULL)
        return first;

    if (first->student_number <= second->student_number) {
        first->next = merge_lists(first->next, second);
        if (first->next != NULL)
            first->next->prev = first;
        first->prev = NULL;
        return first;
    } else {
        second->next = merge_lists(first, second->next);
        if (second->next != NULL)
            second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

void divide_list(Record *original, Record **firstHalf, Record **secondHalf) {
    Record *slowPointer = original;
    Record *fastPointer = original->next;

    while (fastPointer != NULL) {
        fastPointer = fastPointer->next;
        if (fastPointer != NULL) {
            slowPointer = slowPointer->next;
            fastPointer = fastPointer->next;
        }
    }

    *firstHalf = original;
    *secondHalf = slowPointer->next;
    slowPointer->next = NULL;
    if (*secondHalf != NULL) {
        (*secondHalf)->prev = NULL;
    }
}

void sort_list(Record **headRef) {
    if (*headRef == NULL || (*headRef)->next == NULL)
        return;

    Record *firstPart;
    Record *secondPart;

    divide_list(*headRef, &firstPart, &secondPart);

    sort_list(&firstPart);
    sort_list(&secondPart);

    *headRef = merge_lists(firstPart, secondPart);
}


void create_link(Record *current, Record **result_head, Record **result_tail) {
    Record *new_record = (Record *) malloc(sizeof(Record));
    new_record->student_number = current->student_number;
    strncpy(new_record->general_course_name, current->general_course_name, 32);
    strncpy(new_record->general_course_instructor, current->general_course_instructor, 32);
    new_record->general_course_score = current->general_course_score;
    strncpy(new_record->core_course_name, current->core_course_name, 32);
    strncpy(new_record->core_course_instructor, current->core_course_instructor, 32);
    new_record->core_course_score = current->core_course_score;
    new_record->next = NULL;

    if (*result_tail == NULL) {
        *result_head = new_record;
        *result_tail = new_record;
    } else {
        (*result_tail)->next = new_record;
        *result_tail = new_record;
    }
}


void select_records(Database *db, const char *table_name, const char *column_name, const char *value, int sorted) {
    Table *table = find_table(db, table_name);
    if (table == NULL) {
        printf("\nNo Table.\n");
        return;
    }

    Record *current = table->head;
    Record *result_head = NULL;
    Record *result_tail = NULL;


    enum Column column;
    if (strcmp(column_name, "student_number") == 0)
        column = STUDENT_NUMBER;
    else if (strcmp(column_name, "core_course_score") == 0)
        column = CORE_COURSE_SCORE;
    else if (strcmp(column_name, "general_course_score") == 0)
        column = GENERAL_COURSE_SCORE;
    else if (strcmp(column_name, "general_course_name") == 0)
        column = GENERAL_COURSE_NAME;
    else if (strcmp(column_name, "core_course_instructor") == 0)
        column = CORE_COURSE_INSTRUCTOR;
    else if (strcmp(column_name, "general_course_instructor") == 0)
        column = GENERAL_COURSE_INSTRUCTOR;
    else if (strcmp(column_name, "core_course_name") == 0)
        column = CORE_COURSE_NAME;
    else
        column = UNKNOWN;

    while (current != NULL) {
        switch (column) {
            case STUDENT_NUMBER:
                if (current->student_number == atol(value)) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            case CORE_COURSE_SCORE:
                if (current->core_course_score == atoi(value)) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            case GENERAL_COURSE_SCORE:
                if (current->general_course_score == atoi(value)) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            case GENERAL_COURSE_NAME:
                if (strcmp(current->general_course_name, value) == 0) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            case CORE_COURSE_INSTRUCTOR:
                if (strcmp(current->core_course_instructor, value) == 0) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            case GENERAL_COURSE_INSTRUCTOR:
                if (strcmp(current->general_course_instructor, value) == 0) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            case CORE_COURSE_NAME:
                if (strcmp(current->core_course_name, value) == 0) {
                    create_link(current, &result_head, &result_tail);
                }
                break;
            default:
                printf("\nUnknown column name.\n");
                return;
        }
        current = current->next;
    }

    if (sorted) {
        sort_list(&result_head);
    }
    current = result_head;
    if (current == NULL) {
        printf("\nNo Records.\n");
        return;
    }

    while (current != NULL) {
        print_record(current);
        Record *temp = current;
        current = current->next;
        free(temp);
    }
}

void print_table(Database *db, const char *table_name) {
    Table *table = find_table(db, table_name);
    if (table == NULL) {
        printf("Table '%s' does not exist.\n", table_name);
        return;
    }

    printf("Table: %s\n", table_name);
    printf("-----------------------------------------------------------------------------------\n");

    printf("STUDENT_NUMBER | CORE_COURSE_SCORE | GENERAL_COURSE_SCORE | GENERAL_COURSE_NAME | CORE_COURSE_INSTRUCTOR | GENERAL_COURSE_INSTRUCTOR | CORE_COURSE_NAME\n");
    printf("-----------------------------------------------------------------------------------\n");

    Record *current = table->head;
    while (current != NULL) {
        printf("%ld | %s | %s | %d | %s | %s | %d\n",
               current->student_number,
               current->general_course_name,
               current->general_course_instructor,
               current->general_course_score,
               current->core_course_name,
               current->core_course_instructor,
               current->core_course_score);
        current = current->next;

    }

    printf("-----------------------------------------------------------------------------------\n");
}



int main() {
    Database *db = create_database();
    char request[256];

    printf("\nEnter command: "
           "CREATE / DELETE TABLE <table_name>\n"
           "CREATE INDEX <table_name>\n"
           "DELETE <table_name> <column_name> <value>\n"
           "ADD <table_name> <column_name-1> <value-1> ... <column_name-7> <value-7>\n"
           "UPDATE <table_name> <column_name> <value> <new_value>\n"
           "SELECT <table_name> <column_name> <value> [SORTED]\n"
           "PRINT <table_name>\n"
           "EXIT\n");
    while (1) {
        fgets(request, sizeof(request), stdin);

        char command[256];
        sscanf(request, "%s", command);

        int action = -1;
        if (strcmp(command, "CREATE") == 0)
            action = 1;
        else if (strcmp(command, "DELETE") == 0)
            action = 2;
        else if (strcmp(command, "ADD") == 0)
            action = 3;
        else if (strcmp(command, "UPDATE") == 0)
            action = 4;
        else if (strcmp(command, "SELECT") == 0)
            action = 5;
        else if (strcmp(command, "PRINT") == 0)
            action = 6;
        else if (strcmp(command, "EXIT") == 0)
            action = 0;

        switch (action) {
            case 1: // CREATE
            {
                char table_name[256];
                if (strstr(request, "TABLE") != NULL) {
                    sscanf(request, "%*s TABLE %s", table_name);
                    add_table(db, table_name);
                }
                break;
            }
            case 2: // DELETE
            {
                char table_name[256], column_name[256], value[256];
                if (strstr(request, "TABLE") != NULL) {
                    sscanf(request, "DELETE TABLE %s", table_name);
                    delete_table(db, table_name);
                } else {
                    sscanf(request, "DELETE %s %s %s", table_name, column_name, value);
                    Table *table = find_table(db, table_name);
                    if (table == NULL) {
                        printf("No Table.\n");
                        continue;
                    }
                    delete_record(table, column_name, value);
                }
                break;
            }
            case 3: // ADD
            {
                char table_name[256];
                char column1[256], value1[256];
                char column2[256], value2[256];
                char column3[256], value3[256];
                char column4[256], value4[256];
                char column5[256], value5[256];
                char column6[256], value6[256];
                char column7[256], value7[256];

                sscanf(request, "ADD %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", table_name,
                       column1, value1, column2, value2, column3, value3,
                       column4, value4, column5, value5, column6, value6,
                       column7, value7);

                Table* table = find_table(db, table_name);
                if (table == NULL) {
                    printf("\nNo Table.\n");
                    break;
                }

                const char* correct_order[] = {
                        "student_number", "general_course_name", "general_course_instructor",
                        "general_course_score", "core_course_name", "core_course_instructor", "core_course_score"
                };

                char* sorted_values[7] = {NULL};

                char* columns[7] = {column1, column2, column3, column4, column5, column6, column7};
                char* values[7] = {value1, value2, value3, value4, value5, value6, value7};

                for (int i = 0; i < 7; i++) {
                    if (strcmp(columns[i], correct_order[0]) == 0) sorted_values[0] = values[i];
                    if (strcmp(columns[i], correct_order[1]) == 0) sorted_values[1] = values[i];
                    if (strcmp(columns[i], correct_order[2]) == 0) sorted_values[2] = values[i];
                    if (strcmp(columns[i], correct_order[3]) == 0) sorted_values[3] = values[i];
                    if (strcmp(columns[i], correct_order[4]) == 0) sorted_values[4] = values[i];
                    if (strcmp(columns[i], correct_order[5]) == 0) sorted_values[5] = values[i];
                    if (strcmp(columns[i], correct_order[6]) == 0) sorted_values[6] = values[i];
                }

                add_record(table, sorted_values[0], sorted_values[1], sorted_values[2], sorted_values[3],
                           sorted_values[4], sorted_values[5], sorted_values[6]);
                break;
            }
            case 4: // UPDATE
            {
                char table_name[256], column_name[256], value[256], new_value[256];
                sscanf(request, "UPDATE %s %s %s %s", table_name, column_name, value, new_value);
                update_record(db, table_name, column_name, value, new_value);
                break;
            }
            case 5: // SELECT
            {
                char table_name[256], column_name[256], value[256];
                int sorted = 0;
                if (strstr(request, "SORTED") != NULL) {
                    sscanf(request, "%*s %s %s %s SORTED", table_name, column_name, value);
                    sorted = 1;
                } else {
                    sscanf(request, "SELECT %s %s %s", table_name, column_name, value);
                }
                select_records(db, table_name, column_name, value, sorted);
                break;
            }
            case 6: // PRINT
            {
                char table_name[256];
                sscanf(request, "PRINT %s", table_name);
                print_table(db, table_name);
                break;
            }
            case 0: // EXIT
                return 0;
            default:
                printf("\nUnknown command.\n");
                break;
        }
    }

    return 0;
}