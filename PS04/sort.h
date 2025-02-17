/* define constants */
#define MAX_STUDENTS	100
#define LINE_LEN		80
#define NAME_LEN		32

/* structure to read, sort and write */
struct Student {
	char last[NAME_LEN];
	char first[NAME_LEN];
	int id;
};

/* function prototypes your code */
int read_students(FILE *ifp, struct Student *sdata);
int sort_students(char *sort_key, int num_students, struct Student *sdata);
int comp_first(const void * a, const void * b);
int comp_last(const void * a, const void * b);
int comp_id(const void * a, const void * b);
void write_students(FILE *ofp, int num_students, struct Student *sdata);
