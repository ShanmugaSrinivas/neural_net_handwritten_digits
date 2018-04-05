#ifndef SFS_LINUX_NU_HPP
#define SFS_LINUX_NU_HPP

#include <stdio.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

class file
{
public:
	string name;
	size_t size;
	bool is_directory;
	file()
	{
		name="";
		size=0;
		is_directory=false;
	}
	bool is_file()
	{
		return !is_directory;
	}
	bool is_dir()
	{
		return is_directory;
	}
};

class open_folder
{
public:
	string path;
	file f;
	DIR *dir;
    struct dirent *entry;
	open_folder()
	{
		path="";
		dir=nullptr;
		entry=nullptr;
	}
	~open_folder()
	{
		closedir(dir);
	}
	int open(string path_arg)
	{
		path=path_arg;
		if (!(dir = opendir(path_arg.c_str())))
		{
			cout << "\nerror: unable to open dir " << path_arg << "\n returning...\n";
        	return -2;
		}
		entry = readdir(dir);
		if(entry==NULL)
		{
			cout << "\nnot able to read files in dir " << path_arg << "\n returning...\n";
			return -1;
		}
		f.name=entry->d_name;
		if(entry->d_type==DT_DIR)
		{
			f.is_directory=true;
			f.size=0;
		}
		else
		{
			f.is_directory=false;
			struct stat buffer;
			if(stat("path to file", &buffer)==0)
			{
				f.size=buffer.st_size;
			}
		}
		return 0;
	}
	int load_next()
	{
		entry = readdir(dir);
		if(entry==NULL)
		{
			return -1;
		}
		f.name=entry->d_name;
		if(entry->d_type==DT_DIR)
			f.is_directory=true;
		else
		{
			f.is_directory=false;
			struct stat buffer;
			if(stat("path to file", &buffer)==0)
			{
				f.size=buffer.st_size;
			}
		}
		return 0;
	}
};
#endif
