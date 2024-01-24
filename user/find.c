#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define MAX_SIZE 512

char * fmtname(char* path)	//"fmt" stands for "format"
{
	char *p;
	for (p=path+strlen(path); p>=path && *p!='/'; p--)
		;
	p++;

	return p;
}

void find(char *path , char *file)
{
	struct stat st;
	struct dirent de;
	int fd;
	char buf[MAX_SIZE], *p;

	if ((fd=open(path, O_RDONLY)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot fstat %s\n", path);
	}

	switch (st.type) {
	case T_DEVICE:
	case T_FILE:
	      if (strcmp(fmtname(path), fmtname(file)) == 0) fprintf(1, "%s\n", path);
	      break;
	case T_DIR:
	      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
		      fprintf(2, "find: path too long\n");
		      break;
	      }
	      strcpy(buf, path);
	      p = buf + strlen(buf);
	      *p++ = '/';
	      while(read(fd, &de, sizeof(de)) == sizeof(de)) {
	      	if (de.inum == 0) continue;
		if (strcmp(de.name, ".") && strcmp(de.name, "..")) {
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			find(buf, file);
		}
	      }
	      break;
	}
	close(fd);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(2, "Usage: find [PATH] FILE\n");
		exit(1);
	}
	if (argc < 3) find(".", argv[1]);
	else {
		for(int i=1; i<argc-1; i++) find(argv[i], argv[argc-1]);
	}

	exit(0);
}
