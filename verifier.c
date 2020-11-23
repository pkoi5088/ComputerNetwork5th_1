#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
The verifier program read in the output of the generator program,
alter, that inverts 1 bit on the first line depending on its argument
(the bit number counting the leftmost bit as 1) but copies the rest of the two lines correctly.
*/

void strXOR(char* a, char* b, int size) {
	for (int i = 0; i < size; i++) {
		a[i] = a[i] == b[i] ? '0' : '1';
	}
}

void getRemainder(char* message, char* polynomial, int n, int k, char* result) {
	int i;
	for (i = 0; i < k; i++) {
		result[i] = message[i];
	}
	result[i] = '\0';
	for (i = k; i <= n; i++) {
		if (result[0] == '1') {
			strXOR(result, polynomial, k);
		}
		if (i != n) {
			int j;
			for (j = 0; j < k - 1; j++) {
				result[j] = result[j + 1];
			}
			result[j] = message[i];
		}
	}
	result[i] = '\0';
}

int main() {
	char message[50];
	char polynomial[50];
	char remainder[50];
	scanf("%s", message);
	scanf("%s", polynomial);

	int n = strlen(message), k = strlen(polynomial), i, flag = 0;

	getRemainder(message, polynomial, n, k, remainder);
	for (i = 1; i < k; i++) {
		if (remainder[i] == '1') {
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		printf("correct message");
	}
	else {
		printf("incorrect message");
	}
	return 0;
}