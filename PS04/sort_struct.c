#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

int read_students(FILE *ifp, struct Student *sdata)
{
    /*/
    read through the file line by line

    /*/
    int i;
    char line[LINE_LEN]; // looks like the csv file, each line have numbers of characters small than 50

    // traverse through each line of the file
    for (i = 0; i < MAX_STUDENTS; i++)
    {

        if (fgets(line, sizeof(line), ifp) == NULL)
        {
            break;
        }

        // split str
        char *last = strtok(line, ",\n");
        char *first = strtok(NULL, ",\n");
        char *id_str = strtok(NULL, ",\n");

        // copy data into new file
        strcpy(sdata[i].last, last);
        strcpy(sdata[i].first, first);
        sdata[i].id = atoi(id_str);
    }
    return i;
}

int sort_students(char *sort_key, int num_students, struct Student *sdata)
{
    // pointer setup
    int (*comp_func)(const void *, const void *) = NULL;

    if (strcmp(sort_key, "first") == 0)
    {
        comp_func = comp_first;
    }
    else if (strcmp(sort_key, "last") == 0)
    {
        comp_func = comp_last;
    }
    else if (strcmp(sort_key, "id") == 0)
    {
        comp_func = comp_id;
    }
    else
    {
        return -1;
    }

    qsort(sdata, num_students, sizeof(struct Student), comp_func);
    return 0;
}

int comp_first(const void *a, const void *b)
{
    const struct Student *pa = (const struct Student *)a;
    const struct Student *pb = (const struct Student *)b;
    return strcmp(pa->first, pb->first);
}

int comp_last(const void *a, const void *b)
{
    const struct Student *pa = (const struct Student *)a;
    const struct Student *pb = (const struct Student *)b;
    return strcmp(pa->last, pb->last);
}

int comp_id(const void *a, const void *b)
{
    const struct Student *pa = (const struct Student *)a;
    const struct Student *pb = (const struct Student *)b;
    return pa->id - pb->id;
}

void write_students(FILE *ofp, int num_students, struct Student *sdata)
{
    for (int i = 0; i < num_students; i++)
    {
        // write file
        fprintf(ofp, "%s,%s,%d\n", sdata[i].last, sdata[i].first, sdata[i].id);
        // terminal output
        printf("%s,%s,%d\n", sdata[i].last, sdata[i].first, sdata[i].id);
    }
}