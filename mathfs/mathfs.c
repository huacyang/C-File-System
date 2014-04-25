#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <fuse/fuse.h>

static const char *_factor_path = "/factor/";
static const char *_fib_path = "/fib/";
static const char *_add_path = "/add/";
static const char *_sub_path = "/sub/";
static const char *_mul_path = "/mul/";
static const char *_div_path = "/div/";
static const char *_exp_path = "/exp/";

static const char *_factor_path_dir = "/factor";
static const char *_fib_path_dir = "/fib";
static const char *_add_path_dir = "/add";
static const char *_sub_path_dir = "/sub";
static const char *_mul_path_dir = "/mul";
static const char *_div_path_dir = "/div";
static const char *_exp_path_dir = "/exp";

static int _input_len = 20;

// FUSE function implementations.
static int
mathfs_getattr(const char *path, struct stat *stbuf) {
	int res = 0;

	//Reset stbuf
	memset(stbuf, 0, sizeof(struct stat));
	//Marks the file as a direcrory
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 8;
	} else if (strcmp(_factor_path_dir, path) == 0 ||
			   strcmp(_fib_path_dir, path) == 0 ||
			   strcmp(_add_path_dir, path) == 0 ||
			   strcmp(_sub_path_dir, path) == 0 ||
			   strcmp(_mul_path_dir, path) == 0 ||
			   strcmp(_div_path_dir, path) == 0 ||
			   strcmp(_exp_path_dir, path) == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strstr(path, _factor_path) != NULL ||
			   strstr(path, _fib_path) != NULL ||
			   strstr(path, _add_path) != NULL ||
			   strstr(path, _sub_path) != NULL ||
			   strstr(path, _mul_path) != NULL ||
			   strstr(path, _div_path) != NULL ||
			   strstr(path, _exp_path) != NULL) {
		stbuf->st_mode = S_IFREG | 0444;
		//Link to the file and its number
		stbuf->st_nlink = 1;
		stbuf->st_size = _input_len;
	} else {
		res = -ENOENT;
	}

	return res;
}

//Buf is used to hold the contents of a directory
//Path to the directory we want to use
//filler is used to add contents to the directory


static int
mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;
    char * resultPath = calloc(6,sizeof(char));

    if (strcmp(path, "/") == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, _factor_path_dir + 1, NULL, 0);
		filler(buf, _fib_path_dir + 1, NULL, 0);
		filler(buf, _add_path_dir + 1, NULL, 0);
		filler(buf, _sub_path_dir + 1, NULL, 0);
		filler(buf, _mul_path_dir + 1, NULL, 0);
		filler(buf, _div_path_dir + 1, NULL, 0);
		filler(buf, _exp_path_dir + 1, NULL, 0);
    } else if (strcmp(_factor_path_dir, path) == 0 ||
			   strcmp(_fib_path_dir, path) == 0 ||
			   strcmp(_add_path_dir, path) == 0 ||
			   strcmp(_sub_path_dir, path) == 0 ||
			   strcmp(_mul_path_dir, path) == 0 ||
			   strcmp(_div_path_dir, path) == 0 ||
			   strcmp(_exp_path_dir, path) == 0) {

    	filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
    } else {
		return -ENOENT;
    }

    //check what path the user is asking for
    /*
    if(strstr(_add_path_dir,path))
    {

    	//look at the numbers in the path bro
		//math/add/1/2
		//we want to get the characters at 9 and 11
    	int firstNumber = path[9] - '0';
    	int secondNumber = path[11] - '0';
    	int result = firstNumber + secondNumber;
    	printf("\n\nThe result is %d\n\n",result);
    	strcpy(resultPath,_add_path);
    	//add 4 in the path 1 for the slash and the other 3 for 'add'
    	filler(buf, path + 4, NULL, 0);
    }
    else
    {
    	return -ENOENT;
    }
*/

	return 0;
}		

static int
help_open(struct fuse_file_info *fi) {
	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;
	return 0;
}

static int
mathfs_open(const char *path, struct fuse_file_info *fi) {

	if(strstr(path, _factor_path_dir) != NULL)
		return help_open(fi);
	else if (strstr(path, _fib_path_dir) != NULL)
		return help_open(fi);
	else if (strstr(path, _add_path_dir) != NULL)
		return help_open(fi);
	else if (strstr(path, _sub_path) != NULL)
		return help_open(fi);
	else if (strstr(path, _mul_path) != NULL)
		return help_open(fi);
	else if (strstr(path, _div_path) != NULL)
		return help_open(fi);
	else if (strstr(path, _exp_path) != NULL)
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
    
	if(strstr(path, _factor_path) != NULL) {
		size = help_read(buf, (char*)path, size, offset);
	}
	else if(strstr(path, _fib_path) != NULL)
		size = help_read(buf, "Fibonacci...\n", size, offset);
	else if(strstr(path, _add_path) != NULL)
		size = help_read(buf, "Addition...\n", size, offset);
	else if(strstr(path, _sub_path) != NULL)
		size = help_read(buf, "Subtraction...\n", size, offset);
	else if(strstr(path, _mul_path) != NULL)
		size = help_read(buf, "Multiplication\n", size, offset);
	else if(strstr(path, _div_path) != NULL)
		size = help_read(buf, "Division...\n", size, offset);
	else if(strstr(path, _exp_path) != NULL)
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