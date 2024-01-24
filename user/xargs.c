/*** xargs ***/

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
	int x_argc;
	char *x_argv[MAXARG];
	char buf[512];
	char *arg_s, *arg_e;	//pointer that point to arg_start and arg_end in buf

	x_argc = argc-1;
	for (int i=1; i<argc; i++)
		x_argv[i-1]=argv[i];
	arg_s = arg_e = buf;

	while (read(0, arg_e, sizeof(char))) {
		if (*arg_e == '\n') {
			//put in arg
			if (x_argc == MAXARG-1) {	//if full
				fprintf(2, "xargs: too many arguments\n");
				arg_s = arg_e = buf;
				*arg_e = '\0';
				while (*arg_e != '\n') read(0, arg_e, sizeof(char));
			} else {			// not full
				*arg_e = '\0';
				x_argv[x_argc++] = arg_s;
				arg_e++;
				arg_s = arg_e;
			}

			//fork and exec
			if (fork() == 0) {
				exec(x_argv[0], x_argv);
			}

			//empty buffer and recover x_argc = argc-1
			arg_s = arg_e = buf;
			x_argc = argc-1;

		} else if (*arg_e == ' ') {
			if (x_argc == MAXARG-1) {	//if full
				fprintf(2, "xargs: too many arguments\n");
				arg_s = arg_e = buf;
				*arg_e = '\0';
				while (*arg_e != '\n') read(0, arg_e, sizeof(char));
			} else {			// not full
				*arg_e = '\0';
				x_argv[x_argc++] = arg_s;
				arg_e++;
				arg_s = arg_e;
			}
		} else {
			arg_e++;
		}
	}

	if (buf != arg_e) {	//while finished, but not empty
		if (x_argc == MAXARG-1) {	//if full
			fprintf(2, "xargs: too many arguments\n");
			arg_s = arg_e = buf;
			*arg_e = '\0';
			while (*arg_e != '\n') read(0, arg_e, sizeof(char));
		} else {			// not full
			*arg_e = '\0';
			x_argv[x_argc++] = arg_s;
			arg_e++;
			arg_s = arg_e;
		}
		
		if(fork() == 0) {
			exec(x_argv[0], x_argv);
		}
	}
	
	wait(0);
	exit(0);
}
