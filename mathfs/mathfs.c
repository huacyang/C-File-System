#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <fuse/fuse.h>

static const char *_factor_path = "/factor";
static const char *_fib_path = "/fib";
static const char *_add_path = "/add";
static const char *_sub_path = "/sub";
static const char *_mul_path = "/mul";
static const char *_div_path = "/div";
static const char *_exp_path = "/exp";
static int _input_len = 20;

// FUSE function implementations.
static int
mathfs_getattr(const char *path, struct stat *stbuf) {
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strstr(_factor_path, path) != NULL ||
			   strstr(_fib_path, path) != NULL ||
			   strstr(_add_path, path) != NULL ||
			   strstr(_sub_path, path) != NULL ||
			   strstr(_mul_path, path) != NULL ||
			   strstr(_div_path, path) != NULL ||
			   strstr(_exp_path, path) != NULL) {

		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = _input_len;
	} else {
		res = -ENOENT;
	}

	return res;
}

static int
mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    if (strstr(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, _factor_path + 1, NULL, 0);
	filler(buf, _fib_path + 1, NULL, 0);
	filler(buf, _add_path + 1, NULL, 0);
	filler(buf, _sub_path + 1, NULL, 0);
	filler(buf, _mul_path + 1, NULL, 0);
	filler(buf, _div_path + 1, NULL, 0);
	filler(buf, _exp_path + 1, NULL, 0);
}		

static int
help_open(struct fuse_file_info *fi) {
	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;
}

static int
mathfs_open(const char *path, struct fuse_file_info *fi) {

	if(strstr(_factor_path, path) != NULL)
		return help_open(fi);
	else if (strstr(_fib_path, path) != NULL)
		return help_open(fi);
	else if (strstr(_add_path, path) != NULL)
		return help_open(fi);
	else if (strstr(_sub_path, path) != NULL)
		return help_open(fi);
	else if (strstr(_mul_path, path) != NULL)
		return help_open(fi);
	else if (strstr(_div_path, path) != NULL)
		return help_open(fi);
	else if (strstr(_exp_path, path) != NULL)
		return help_open(fi);
	else
		return -ENOENT;
}

static int
help_read(char *buf, char *str, size_t size, off_t offset) {
	size_t len;
	len = strlen(str);

	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, str + offset, size);
	} else
		size = 0;
	return size;
}

static int 
mathfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    (void) fi;
    
	if(strstr(_factor_path, path) != NULL) {
		size = help_read(buf, "Factor...\n", size, offset);
	}
	else if(strstr(_fib_path, path) != NULL)
		size = help_read(buf, "Fibonacci...\n", size, offset);
	else if(strstr(_add_path, path) != NULL)
		size = help_read(buf, "Addition...\n", size, offset);
	else if(strstr(_sub_path, path) != NULL)
		size = help_read(buf, "Subtraction...\n", size, offset);
	else if(strstr(_mul_path, path) != NULL)
		size = help_read(buf, "Multiplication\n", size, offset);
	else if(strstr(_div_path, path) != NULL)
		size = help_read(buf, "Division...\n", size, offset);
	else if(strstr(_exp_path, path) != NULL)
		size = help_read(buf, "Exponential...\n", size, offset);
	else
		return -ENOENT;

	return size;
}

static struct fuse_operations mathfs_oper = {
    .getattr = mathfs_getattr,
    .readdir = mathfs_readdir,
    .open = mathfs_open,
    .read = mathfs_read,
};

int
main(int argc, char **argv) {
	int i;

    i = fuse_main(argc, argv, &mathfs_oper, NULL);
	printf("=%d\n", i);

	return i;
}
