#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
The generator program reads from standard input a line of ASCII text containing an n-bit message consisting of a string of 0s and 1s.
The second line is the k-bit polynomial, also in ASCII.
It outputs to standard output a line of ASCII text with n + k 0s and 1s representing the message to be transmitted.
Then it outputs the polynomial, just as it read it in.
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
			for (j = 0; j < k-1; j++) {
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

	int n = strlen(message), k = strlen(polynomial), i;

	for (i = 0; i < k - 1; i++)
		message[n + i] = '0';
	message[n + i] = '\0';

	getRemainder(message, polynomial, n + k - 1, k, remainder);

	for (i = 1; i < k; i++) {
		if (remainder[i] == '1')
			message[n] = '1';
		else
			message[n] = '0';
		n++;
	}
	printf("%s\n", message);
	printf("%s", polynomial);
	return 0;
}