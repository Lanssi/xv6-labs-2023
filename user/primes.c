#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0
#define WR 1

int main(int argc, char *argv[])
{
	int pp[2];	//pipe_port
	int prime;
	int data;
	int pid=0;

	// initialize
	pipe(pp);
	for (int i=2; i<=35; i++)
		write(pp[WR], &i, sizeof(int));
	close(0);
	dup(pp[RD]);
	close(pp[RD]);
	close(pp[WR]);
	prime=2;
	printf("prime 2\n");

	// every sub-process execute the same logic
	while (read(0, &data, sizeof(int))) {
		if (data % prime != 0) {
			if (!pid) {	//is a child
				pipe(pp);
				pid=fork();
				if (pid) {	//parent
					close(pp[RD]);
				} else {	//child
					close(0);
					dup(pp[RD]);
					close(pp[RD]);
					close(pp[WR]);
					prime=data;
					printf("prime %d\n", data);
				}
			}
			write(pp[WR], &data, sizeof(int));
		}
	}
	close(pp[WR]);
	wait(0);
	exit(0);
}
