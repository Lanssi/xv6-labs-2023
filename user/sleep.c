#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(2, "Usage: sleep TICKS\n");
		exit(1);
	}
	uint ticks=atoi(argv[1]);
	sleep(ticks);
	exit(0);
}
