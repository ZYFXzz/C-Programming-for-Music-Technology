#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sort.h"

int main(int argc, char *argv[])
{
	int num_students;
	char *sort_key;
	char *ifile, *ofile;
	FILE *ifp, *ofp;
	struct Student sdata[MAX_STUDENTS];

	if (argc != 4) {
		fprintf(stderr, "Usage: %s last|first|id in_file.csv out_file.csv\n", argv[0]);
		return -1;
	}

	/* command line args */
	sort_key = argv[1];
	ifile = argv[2];
	ofile = argv[3];

	/* open input file */
	ifp = fopen(ifile, "r");
	if ( ifp == NULL ) {
		fprintf(stderr, "Cannot open %s\n", ifile);
		return -1;
	}
	/* open output file */
	ofp = fopen(ofile, "w");
	if ( ofp == NULL ) {
		fprintf(stderr, "Cannot open %s\n", ofile);
		return -1;
	}

	/* read input into student array */
	num_students = read_students(ifp, sdata);

	// print structure contents
	for (int i=0; i<num_students; i++) {
		printf("%4d %s %s %6d\n", i, sdata[i].last, sdata[i].first, sdata[i].id);
	}
	printf("Read records for %d students\n", num_students);

	/* sort student array according to sort_key */
	if ( sort_students(sort_key, num_students, sdata) < 0 ) {
		fprintf(stderr, "ERROR in sorting structure\n");
		return -1;
	}

	/* write sorted student array to output file and also print to terminal */
	write_students(ofp, num_students, sdata);

	/* close input and output files */
	fclose(ifp);
	fclose(ofp);

	return 0;
}
