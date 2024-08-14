// Thanks idle from stackoverflow for the means to execute the program recursively
// https://stackoverflow.com/questions/40954829/c-program-that-runs-itself-using-system

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <signal.h>

int main(int argc, char** argv)
{
	int depth = argc > 1 ? atoi(argv[1]) : 0;
	if (depth > 10) {
		return 0;
	}
	printf("Depth: %d\n", depth);
	char cmd[100];
	sprintf(cmd, "%s %d", argv[0], depth + 1);
	system(cmd);
	return 0;
}
