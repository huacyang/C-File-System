#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <fuse/fuse.h>
#include <math.h>

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

int
findNumberOfOccurances(const char * string , char occurance)
{
	int i;
	int count = 0;
	for(i=0 ; string[i]!='\0' ; i++)
	{
		if(string[i] == occurance)
			count++;	
	}
	return count;
}

int
numDigits(int number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}


// FUSE function implementations.
static int
mathfs_getattr(const char *path, struct stat *stbuf) {
	int res = 0;
	int numberOfOccurances = findNumberOfOccurances(path,'/');
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
	}
	 else if (strstr(path, _factor_path) != NULL ||
			   strstr(path, _fib_path) != NULL ||
			   strstr(path, _add_path) != NULL ||
			   strstr(path, _sub_path) != NULL ||
			   strstr(path, _mul_path) != NULL ||
			   strstr(path, _div_path) != NULL ||
			   strstr(path, _exp_path) != NULL) 
	 {
		if(strstr(path,"doc") != NULL || numberOfOccurances == 3 || (numberOfOccurances == 2 && strstr(path,"factor")))
		{
			stbuf->st_mode = S_IFREG | 0444;
			//Link to the file and its number
			stbuf->st_nlink = 1;
			stbuf->st_size = _input_len;
		}
		else
		{
			stbuf->st_mode = S_IFDIR | 0755;
			//Link to the file and its number
			stbuf->st_nlink = 2;
			//stbuf->st_size = _input_len;	
		}
	} 
	else {
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

    char *temp = calloc(strlen(path),sizeof(char *));
    char *token = strtok(temp,"/");
    char * first = strtok(NULL,"/");
    char * second = strtok(NULL,"/");


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
    } 
    else if (second != NULL)
    {
    	filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, second, NULL, 0);
    }
    else 
    {
		return -ENOENT;
    }

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

    int  numberOfDigits;
    double resultValue,firstValue, secondValue;
    char *temp, *token, *first, *second, *result;
    
	if(strstr(path, _factor_path) != NULL) {
		if(strstr(path,"doc")) {
			size = help_read(buf, "Show the prime factors of a number\nThe file factor/n contains the prime factors of n.\n", size, offset);
		} 
		else 
		{
			temp = calloc(strlen(path),sizeof(char *));
			strcpy(temp,path);
			token = strtok(temp,"/");
			first = strtok(NULL,"/");
			if(findNumberOfOccurances(first , '.') != 0)
			{
				size = help_read(buf, "the number to factor must be an integer\n", size, offset);
			}
			else
			{
				size = help_read(buf, first, size, offset);	
			}
			//int firstValuefactor = atoi(first);
		}

	} else if(strstr(path, _fib_path) != NULL) {
		size = help_read(buf, "Produce a fibonacci sequence.\nThe file fib/n contains the first n fibonacci numbers.\n", size, offset);
	} else if(strstr(path, _add_path) != NULL) {
		if(strstr(path,"doc")) {
			size = help_read(buf, "Add two numbers.\nThe file add/a/b contains the sum a+b.\n", size, offset);
		} else {
			//add the two numbers hooker
			temp = calloc(strlen(path),sizeof(char *));
			strcpy(temp,path);
			token = strtok(temp,"/");
			first = strtok(NULL,"/");
			firstValue = atof(first);
			second = strtok(NULL,"/");
			secondValue = atof(second);

			resultValue = secondValue + firstValue;
			numberOfDigits = numDigits(resultValue);
			result = calloc(numberOfDigits,sizeof(char *));
			sprintf(result, "%g\n", resultValue);
			size = help_read(buf, result, size, offset);
		}
	} else if(strstr(path, _sub_path) != NULL) {
		if(strstr(path,"doc")) {
			size = help_read(buf, "Subtract two numbers.\nThe file sub/a/b contains the difference a-b.\n", size, offset);
		} else {
			//add the two numbers hooker
			temp = calloc(strlen(path),sizeof(char *));
			strcpy(temp,path);
			token = strtok(temp,"/");
			first = strtok(NULL,"/");
			firstValue = atof(first);
			second = strtok(NULL,"/");
			secondValue = atof(second);

			resultValue = firstValue - secondValue;
			numberOfDigits = numDigits(resultValue);
			result = calloc(numberOfDigits,sizeof(char *));
			sprintf(result,"%g\n",resultValue);
			size = help_read(buf, result, size, offset);
		}
	} else if(strstr(path, _mul_path) != NULL) {
		if(strstr(path,"doc")) {
			size = help_read(buf, "Multiply two numbers.\nThe file mul/a/b contains the product a*b.\n", size, offset);
		} else {
			//add the two numbers hooker
			temp = calloc(strlen(path),sizeof(char *));
			strcpy(temp,path);
			token = strtok(temp,"/");
			first = strtok(NULL,"/");
			firstValue = atof(first);
			second = strtok(NULL,"/");
			secondValue = atof(second);

			resultValue = firstValue * secondValue;
			numberOfDigits = numDigits(resultValue);
			result = calloc(numberOfDigits,sizeof(char *));
			sprintf(result,"%g\n",resultValue);
			size = help_read(buf, result, size, offset);
		}
	} else if(strstr(path, _div_path) != NULL) {
		if(strstr(path,"doc")) {
			size = help_read(buf, "Divide two numbers.\nThe file div/a/b contains the quotient a/b\n", size, offset);
		} else {
			//add the two numbers hooker
			temp = calloc(strlen(path),sizeof(char *));
			strcpy(temp,path);
			token = strtok(temp,"/");
			first = strtok(NULL,"/");
			firstValue = atof(first);
			second = strtok(NULL,"/");
			secondValue = atof(second);

			// handler for when dividing by 0
			if (secondValue == 0)
				return help_read(buf, "divide by zero error\n", size, offset);

			resultValue = (double) firstValue / (double) secondValue;
			numberOfDigits = numDigits(resultValue);
			result = calloc(numberOfDigits,sizeof(char *));
			sprintf(result,"%g\n",resultValue);
			size = help_read(buf, result, size, offset);
		}	
	} else if(strstr(path, _exp_path) != NULL) {
		if(strstr(path,"doc")) {
			size = help_read(buf, "Raise a number to an exponent.\nThe file exp/a/b contains a raised to the power of b.\n", size, offset);
		} else {
			//add the two numbers hooker
			temp = calloc(strlen(path),sizeof(char *));
			strcpy(temp,path);
			token = strtok(temp,"/");
			first = strtok(NULL,"/");
			firstValue = atof(first);
			second = strtok(NULL,"/");
			secondValue = atof(second);

			resultValue = pow(firstValue,secondValue);
			numberOfDigits = numDigits(resultValue);
			result = calloc(numberOfDigits,sizeof(char *));
			sprintf(result,"%g\n",resultValue);
			size = help_read(buf, result, size, offset);
		}	
	} else {
		return -ENOENT;
	}

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