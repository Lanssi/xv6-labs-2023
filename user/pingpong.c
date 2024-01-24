#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	//Create pipe
	int fd[2];
	pipe(fd);

	//fprintf(1, "Create pipe success\n");
	char buf[1];

	//Fork
	if (fork() == 0) {	//child
		if (read(fd[0], buf, 1) == 1)
			fprintf(1, "%d: received ping\n", getpid());
		if (write(fd[1], buf, 1) < 1)
			fprintf(2, "error: child write failed\n");
	} else {
		if (write(fd[1], buf, 1 ) < 1)
			fprintf(2, "error: parent write failed\n");
		if (read(fd[0], buf, 1) == 1)
			fprintf(1, "%d: received pong\n", getpid());
	}

	exit(0);
	
}
