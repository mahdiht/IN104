#include <stdio.h>

int main(int argc, char *argv[])
{
	char ch[20];
	scanf("%20[0-9 ]",ch);
	printf("%s",ch);
}
