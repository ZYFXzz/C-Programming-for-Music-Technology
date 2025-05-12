#include "main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* useful to get program running, but may want to set to 0 after bugs are out */
#define db_flag 1

enum {
    MD_LINEAR = 1,
    MD_BINARY,
    MD_HASH
};

int main(int argc, char *argv[]) {
    int mode, i, j, k, n, num_entries, status;
    float avg_chain_len;
    char line[LINE_LEN], *s;
    char *ifile;
    FILE *ifp;
    struct PhoneBook phone_book[PB_LEN];

    /* parse command line
     *  Usage printout
     *  Determine mode
     *  Open phone book file
     */
    // Your code here
    if (argc != 3) {
        fprintf(stderr, "usage: program_name + search_mode + .csv");
        return 1;
    }

    // mode
    if (strcmp(argv[1], "linear")) {
        mode = MD_LINEAR;
    } else if (strcmp(argv[1], "binary")) {
        mode = MD_BINARY;
    } else if (strcmp(argv[1], "hash")) {
        mode = MD_HASH;
    } else {
        fprintf(stderr, "Error: invalid search mode");
        return 1;
    }

    ifile = argv[2];
    ifp = fopen(ifile, "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open file");
        return 1;
    }

    /* initialize phone book structure:
     * all strings have first character as zero (NULL)
     * chain value is set to -1
     */
    // Your code here
    for (i = 0; i < PB_LEN; i++) {
        phone_book[i].first[0] = 0;
        phone_book[i].last[0] = 0;
        phone_book[i].phone[0] = 0;
        phone_book[i].chain = -1;
    }
    /* read in phone book
     * use fgets(), strtok() and strncpy()
     */
    // YOUR CODE HERE
    num_entries = 0;
    while (fgets(line, LINE_LEN, ifp) != NULL && num_entries < PB_LEN) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        s = strtok(line, ",");
        if (s != NULL) {
            strncpy(phone_book[num_entries].last, s, NAME_LEN - 1);
            phone_book[num_entries].last[NAME_LEN - 1] = '\0'; // Ensure null termination
        }

        s = strtok(NULL, ",");
        if (s != NULL) {
            strncpy(phone_book[num_entries].first, s, NAME_LEN - 1);
            phone_book[num_entries].first[NAME_LEN - 1] = '\0';
        }

        s = strtok(NULL, ",");
        if (s != NULL) {
            strncpy(phone_book[num_entries].phone, s, NUM_LEN - 1);
            phone_book[num_entries].phone[NUM_LEN - 1] = '\0';
        }

        num_entries++;
    }
    fclose(ifp);

    /* processing prior to search */
    switch (mode) {
    case MD_BINARY:
        /* binary search assumes list is sorted in alpha-numeric order
         * so sort phone book by last name
         */
        printf("Sorting phone book\n");
        qsort(phone_book, num_entries, sizeof(phone_book[0]), comp_last);
        break;
    case MD_HASH:
        printf("Building hash map table\n");
        create_hash_map(phone_book, num_entries);
        break;
    }

    while (1) {
        /* now ask for any last name */
        printf("Input any last name in phone book (0 to exit):\n");
        scanf("%s", line);
        if (line[0] == '0') {
            break; /* quit */
        }
        if (db_flag)
            printf("You entered:  %s\n", line);

        switch (mode) {
        case MD_LINEAR:
            /* linear search */
            i = linear_search(line, phone_book, num_entries);
            break;
        case MD_BINARY:
            /* binary search */
            i = binary_search(line, phone_book, num_entries);
            break;
        case MD_HASH:
            /* hashed search */
            i = hash_search(line, phone_book);
            break;
        }
        if (i < 0) {
            fprintf(stderr, " ERROR: %s not found\n", line);
        } else {
            printf("%s, %s - %s\n",
                   phone_book[i].last,
                   phone_book[i].first,
                   phone_book[i].phone);
        }
    }
    return 0;
}

/* ordering function for qsort() */
int comp_last(const void *a, const void *b) {
    int sortVal;
    struct PhoneBook *p1 = (struct PhoneBook *)a;
    struct PhoneBook *p2 = (struct PhoneBook *)b;
    return strncmp(p1->last, p2->last, NAME_LEN - 1);
};

/*
 * linear search
 */
int linear_search(char *target, struct PhoneBook *phone_book, int num_entries) {
    // Your code here
    int i;
    for (i = 0; i < num_entries; i++) {
        if (strcmp(phone_book[i].last, target) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * binary search
 */
int binary_search(char *target, struct PhoneBook *phone_book, int num_entries) {
    /* binary search
     * low is low index in search interval
     * high is high index in search interval
     */
    int low, mid, high, status, n, i;
    low = 0;
    high = num_entries - 1;
    mid = (low + high) / 2;
    n = 0; // count loop iterations
    while (low <= high) {
        // Your code here
        mid = (low + high) / 2;
        status = strcmp(target, phone_book[mid].last);

        if (status == 0) {
            return mid;
        } else if (status < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    if (low > high) {
        fprintf(stderr, "ERROR: %s not found\n", target);
        return -1;
    }
    return i;
}

/* hash search */
int hash_search(char *target, struct PhoneBook *phone_book) {
    // Your code here
    int hash_value, index;
    hash_value = hash_func(target);
    index = get_hash_map_value(hash_value);

    if (index == -1) {
        return -1;
    }

    if (strcmp(phone_book[index].last, target) == 0) {
        return index;
    }

    // Follow chain to resolve collisions
    while (phone_book[index].chain != -1) {
        index = phone_book[index].chain;
        if (strcmp(phone_book[index].last, target) == 0) {
            return index; // Found
        }
    }

    return -1;
}
