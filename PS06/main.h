#ifndef  _MAIN_H
#define  _MAIN_H

#define LINE_LEN    80      /* length of line in CSV phone book file */
#define NAME_LEN    64      /* name string length */
#define NUM_LEN     11      /* 10 digit US phone number string, plus NULL*/
#define PB_LEN      1024    /* max phone book length */


struct PhoneBook {
    char last [NAME_LEN];
    char first[NAME_LEN];
    char phone[NUM_LEN];
    int chain;
};

int linear_search(char *target, struct PhoneBook *phone_book, int num_entries);
int binary_search(char *target, struct PhoneBook *phone_book, int num_entries);
int hash_search(char *target, struct PhoneBook *phone_book);
int comp_last(const void * a, const void * b);
int get_hash_map_value(int j);
int hash_func(char *s);
void create_hash_map(struct PhoneBook *phone_book, int num_entries);

#endif
