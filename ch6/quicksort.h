#define VOIDCOMP int (*)(void *, void *)

void quicksort(void *v[], int left, int right, int (*comp)(void *, void *));
