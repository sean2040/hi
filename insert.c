#include <stdio.h>
#include <stdlib.h>/*header file for atoi function*/
#include <time.h>/*header file for checking sorting time and making random number*/

void insert_c(int n, int* arr);
void insert_s(int n, int* arr);

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

	insert_c(n, arr_c);
	insert_s(n, arr_s);

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
	return 0;

}

void insert_c(int n, int* arr) {
	int i;
	for (i = 1; i < n; i++) {
		int j;
		int k;
		for (j = i; j--; j > 0) {
			k = arr[j];
			if (arr[j - 1] < k) { break; }
			else { arr[j] = arr[j - 1];}
		}
		arr[j] = k;
	}
}

void insert_s(int n, int* arr) {
	asm(
		"mov r4, #1\n\t" //i=1
		"mov r5, #0\n\t" //j=0
		"mov r6, #0\n\t" //k
		"ldr r8, %[n]\n\t" //reg_n=n

		"cmp r8, #1\n\t"
		"beq End\n\t"

		"Loop1:\n\t"
		"mov r5, r4\n\t"//j=i
		"Loop2:\n\t"
		"ldr r6, [%[arr], r5]\n\t"//r6=arr[j]
		"sub r5, r5, #4\n\t"//j--
		"ldr r9, [%[arr], r5]\n\t"//r9=arr[j-1]
		"cmp r9, r6\n\t"
		"bcc Loopend\n\t"//arr[j-1]<k -> break
		"add r5, r5, #4\n\t"//j++
		"str r9, [%[arr], r5]"//arr[j]=arr[j-1]
		"sub r5, r5, #4\n\t"//j--
		"b Loop2\n\t"
		"Loopend:\n\t"
		"add r5, r5, #4\n\t"//j++
		"str r6, [%[arr], r5]\n\t"//arr[j]=k
		"add r4, r4, #4\n\t"//i++
		"cmp r4, r8\n\t"
		"bcc Loop1\n\t"//i<n

		"End:\n\t"

		:
		//input operands
		:
		[n] "m"(n), [arr] "r"(arr)
		//output operands
		:
		"r4","r5","r6","r8","r9"
		//clobbers
	);
}