#define MAX_CHAN	2
#define MAX_COEF	256


/* define Filt struct */
struct FIR_Filt {
	int num_b;			//number of coefficients in FIR filer b
	double b[MAX_COEF]; //FIR coefficients
};

/* filter function prototypes */
void filter_file(float *x, float *y, int num_frames, int num_channels, 
	struct FIR_Filt *pf);

void filter_block(float *x, float *y, int num_frames, int num_channels, 
	struct FIR_Filt *pf, float *s);

