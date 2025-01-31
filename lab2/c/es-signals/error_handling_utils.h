#ifndef ERROR_HANDLING_UTILS
#define ERROR_HANDLING_UTILS
#include <stdlib.h>
#include <stdio.h>

#define ERR_EXIT(cond, msg) 				\
if ((cond)) { 								\
	fprintf(stderr, msg); 					\
	exit(EXIT_FAILURE); 					\
}

#endif
