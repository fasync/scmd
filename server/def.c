#include "def.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

static char* home = NULL;
static FILE *f = NULL;

DIR *open_dir(const char* cat_path, const int len)
{
	static DIR* d = NULL;
	
	char path[strlen(home)+1 + len];
	strncpy(path,home,strlen(home)+1);

	if(!strncat(path, cat_path, len))
		return NULL;
	
	d = opendir(path);
	
	if(!d) {
		write_log("Could not find directory %s.", path);

		if (0 != mkdir(path, S_IRWXU | S_IRWXG))
			return NULL;

		write_log("Created directory %s.", path);

		d = opendir(path);
	}
	return d;
}

void prepare_log()
{
	home = getenv("HOME");

	if(!home)
		die(EXIT_FAILURE, "Could not find $HOME variable.");
	
	char path[strlen(home) + 10];
	strncpy(path, home, strlen(home)+1);
	strcat(path, "/.scmd/log");
	
	f = fopen(path, "w");

	puts(path);
	
	if(!f)
		die(ENOENT, "Could not open log file.");
}

void _write_log(const char* func, const char* file, const char* msg, ...) {
	va_list va;
	va_start(va, msg);

	fprintf(f, "%s:[%s]: ", func, file);
	vfprintf(f, msg, va);
	fprintf(f, "\n");

	#ifdef VERBOSE
	printf("%s:[%s]: ", func, file);
	vprintf(msg, va);
	puts("");
	#endif

	va_end(va);
}

void close_log()
{
	fclose(f);
}

void die(int err, char *msg, ...)
{
	va_list va;
	va_start(va, msg);
	vfprintf(stderr, msg, va);
	puts("");
	va_end(va);
	exit(err);
}
