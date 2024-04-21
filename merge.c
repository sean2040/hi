#include <stdio.h>
#include <stdlib.h>/*header file for atoi function*/
#include <time.h>/*header file for checking sorting time and making random number*/

void ms_c(int* arr, int l, int r);
void ms_s(int* arr, int l, int r);
void merge_c(int* arr, int l, int m, int r);
void merge_s(int* arr, int l, int m, int r);

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);/*first input*/
	/*we will make n integers from 1 to n.*/


	/*making array with n integers*/
	int* arr_c = (int*)malloc(n * sizeof(int));
	int i;
	if (arr_c == NULL) { return -1; }/*handling exception*/
	for (i = 0; i < n; i++) {
		arr_c[i] = i + 1; /*make k, 2k, ..., nk*/
	}

	srand(time(NULL));/*setting seed by srand function from time.h*/

	/*swap arrint[n-1] with random index*/
	/*next swap arrint[n - 2] with random index*/
	/*repeat this for arrint[n-3], arrint[n-4], ..., arrint[1]*/
	/*by this swapping algorithm, we can make random orders of multiple of k*/
	for (i = n - 1; i >= 1; i--) {
		int rn; int seed = 0; int digit;
		/*make 8 digits randomly*/
		for (digit = 10000000; digit >= 1; digit = digit / 10) {
			seed = seed + digit * (rand() % 10);/*each digit is given by 'rand()%10'*/
		}
		/*seed means the final 8 digit number we've made*/
		rn = seed % (i + 1);/*remainder by dividing seed with i+1, we can get random number from 0 to i.*/
		if (rn != i) {
			int temp;
			temp = arr_c[rn]; arr_c[rn] = arr_c[i]; arr_c[i] = temp;/*swapping two integers*/
		}
	}

	if (n <= 20) {
		printf("Before sort     :");
		printf(" [");
		for (i = 0; i < n; i++) { printf(" %d", arr_c[i]); }
		printf(" ]\n");
	}

	int* arr_s = (int*)malloc(n * sizeof(int));
	if (arr_s == NULL) { return -1; }/*handling exception*/
	for (i = 0; i < n; i++) {
		arr_s[i] = arr_c[i];
	}

	ms_c(arr_c, 0, n - 1);
	ms_s(arr_s, 0, n - 1);

	if (n <= 20) {
		printf("After sort    (C):");
		printf(" [");
		for (i = 0; i < n; i++) { printf(" %d", arr_c[i]); }
		printf(" ]\n");
		printf("After sort (ASM):");
		printf(" [");
		for (i = 0; i < n; i++) { printf(" %d", arr_s[i]); }
		printf(" ]\n");
	}
	free(arr_c);
	free(arr_s);
	return 0;

}

void ms_c(int* arr, int l, int r) {
	if (l < r) {
		int m = (l + r) / 2;
		ms_c(arr, l, m);
		ms_c(arr, m + 1, r);
		merge_c(arr, l, m, r);
	}
}
void ms_s(int* arr, int l, int r) {
	if (l < r) {
		int m = (l + r) / 2;
		ms_s(arr, l, m);
		ms_s(arr, m + 1, r);
		merge_s(arr, l, m, r);
	}
}
void merge_c(int* arr, int l, int m, int r) {
	int* merged= (int*)malloc((r-l+1) * sizeof(int));
	int a=l; int b=m+1; int c=0;

	while ((a<=m) && (b<=r)) {
		if (arr[a] < arr[b]) {
			merged[c] = arr[a];
			a++;
		}
		else {
			merged[c] = arr[b];
			b++;
		}
		c++;
	}

	if (a > m) {
		while (b <= r) {
			merged[c] = arr[b];
			b++; c++;
		}
	}

	else {
		while (a <= m) {
			merged[c] = arr[a];
			a++; c++;
		}
	}

	for (c = 0; c < r - l + 1; c++) {
		arr[l+c] = merged[c];
	}
	free(merged);
}
void merge_s(int* arr, int l, int m, int r) {
	int* merged = (int*)malloc((r - l + 1) * sizeof(int));
	asm(

	);
	:
	//input operands
	:
	[merged] "r"(merged), [arr] "r"(arr), [l] "m"(l), [m] "m"(m), [r] "m"(r)
	//output operands
	:
	//clobbers
}

