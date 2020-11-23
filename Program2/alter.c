#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char message[50];
	char polynomial[50];
	scanf("%s", message);
	scanf("%s", polynomial);

	if (message[0] == '0')
		message[0] = '1';
	else
		message[0] = '0';

	printf("%s\n", message);
	printf("%s", polynomial);
	return 0;
}