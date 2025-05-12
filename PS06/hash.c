#include <stdio.h>
#include <string.h>
#include "main.h"

#define HV_BITS     14      /* bit length of hash */
#define HT_LEN      (1<<HV_BITS)    /* hash table length */

#define db_flag 1

static int hash_map[HT_LEN];

/* hash function */
int hash_func(char *s)
{
    int i;
    int h = 0, g;
    for (i=0; s[i]; i++) {
        /* good hash function */
        h = s[i] + (h << 6) + (h << 16) - h; /* h = s[i] + h*65631 */
    }
    h &= (1<<HV_BITS)-1; /* mask to length of hash value */
    return h;
}

/* create hash map */
void create_hash_map(struct PhoneBook *phone_book, int num_entries)
{
    int i, j, k, n, status, num_collisions, chain_len;
    float avg_chain_len;;

    /* initialize hash map table */
    for (i=0; i<HT_LEN; i++) {
        hash_map[i] = -1; /* an invalid entry */
    }

    /* create hash for each last name in book */
    for (i=0; i<num_entries; i++) {
         j = hash_func(phone_book[i].last);
        if (hash_map[j] < 0) {
            /* empty table entry, so store hash */
            hash_map[j] = i;
        }
        else {
            /* hash collision */
            /* find end of chain at collision */
            printf("collision\n");
            k = hash_map[j];
            while (phone_book[k].chain >= 0) {
                k = phone_book[k].chain;
            }
            phone_book[k].chain = i; /* end of chain */
        }
        /* debug printout, where j is hash value */
        if (db_flag) printf("%6d %6d %s\n", i, j, phone_book[i].last);
    }

    /* report statistics of hash table */
    k=0;
    n=0;
    num_collisions = 0;
    avg_chain_len = 0;
    for (i=0; i<HT_LEN; i++) {
        if (hash_map[i] >= 0) {
            /* hash map occupied */
            k = i;
            n++;
            j = hash_map[i];
            if (phone_book[j].chain >= 0) {
                /* collision for this hash value */
                num_collisions++;
                chain_len = 1;
                j = phone_book[j].chain;
                while (phone_book[j].chain >= 0) {
                    chain_len++;
                    j = phone_book[j].chain;
                }
                if (db_flag) printf("Entry %d has chain length %d\n", i, chain_len);
                avg_chain_len += chain_len;
            }
        }
    }
    avg_chain_len /= num_collisions;
    printf("Hash table statistics:\n");
    printf("%6.2f%% map occupancy; top map entry %d out of %d entries\n",
        100.0*n/HT_LEN, k, HT_LEN);
    printf("Collisions: %d (%6.2f%%)\n", num_collisions, 100.0*num_collisions/HT_LEN);
    printf("Average chain length %6.2f\n", avg_chain_len);
}

int get_hash_map_value(int j)
{
    return hash_map[j];
}
