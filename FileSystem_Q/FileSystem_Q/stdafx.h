#pragma once
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <string>
#include<queue>
#include<stack>
#include <iostream>
#include <conio.h>		//windows中用于不回显字符
//#include<termios.h>   //Linux下用于自定义不回显字符
//#include<unistd.h>	//Linux下用于自定义不回显字符
#include<assert.h>
#include<vector>
#include<string.h>
#include<time.h>
#include<Windows.h>

using namespace std;
/*
*	The maximum number of blocks a file can use.
*	Used in struct inode.
*/
const unsigned int NADDR = 6;
/*
*	Size of a block.
*/
const unsigned short BLOCK_SIZE = 512;
/*
*	The maximum size of a file.
*/
const unsigned int FILE_SIZE_MAX = (NADDR - 2) * BLOCK_SIZE + BLOCK_SIZE / sizeof(int) * BLOCK_SIZE;
/*
*	The maximum number of data blocks.
*/
const unsigned short BLOCK_NUM = 512;
/*
*	Size of an inode.
*/
const unsigned short INODE_SIZE = 128;
/*
*	The maximum number of inodes.
*/
const unsigned short INODE_NUM = 256;
/*
*	The start position of inode chain.
*	First four blocks are used for loader sector(empty in this program), super block, inode bitmap and block bitmap.
*/
const unsigned int INODE_START = 3 * BLOCK_SIZE;
/*
*	The start position of data blocks.
*/
const unsigned int DATA_START = INODE_START + INODE_NUM * INODE_SIZE;
/*
*	The maximum number of the file system users.
*/
const unsigned int ACCOUNT_NUM = 10;
/*
*	The maximum number of sub-files and sub-directories in one directory.
*/
const unsigned int DIRECTORY_NUM = 16;
/*
*	The maximum length of a file name.
*/
const unsigned short FILE_NAME_LENGTH = 14;
/*
*	The maximum length of a user's name.
*/
const unsigned short USER_NAME_LENGTH = 14;
/*
*	The maximum length of a accouting password.
*/
const unsigned short USER_PASSWORD_LENGTH = 14;
/*
*	The maximum permission of a file.
*/
const unsigned short MAX_PERMISSION = 511;//0x1ff 11111111'
/*
*	The maximum permission of a file.
*/
const unsigned short MAX_OWNER_PERMISSION = 448;//'0x1c0 11000000'
/*
*	Permission
*/
const unsigned short ELSE_E = 1;
const unsigned short ELSE_W = 1 << 1;
const unsigned short ELSE_R = 1 << 2;
const unsigned short GRP_E = 1 << 3;
const unsigned short GRP_W = 1 << 4;
const unsigned short GRP_R = 1 << 5;
const unsigned short OWN_E = 1 << 6;
const unsigned short OWN_W = 1 << 7;
const unsigned short OWN_R = 1 << 8;
//Data structures.
/*
*	inode(128B)
*/

struct inode
{
	unsigned int i_ino;			//Identification of the inode.
	unsigned int di_addr[NADDR];//Number of data blocks where the file stored.
	unsigned short di_number;	//Number of associated files.
	unsigned short di_mode;		//0 stands for a directory, 1 stands for a file.
	unsigned short icount;		//link number
	unsigned short permission;	//file permission
	unsigned short di_uid;		//File's user id.
	unsigned short di_grp;		//File's group id
	unsigned short di_size;		//File size.
	char time[83];
};

typedef struct copydir{
	
	char fileName[FILE_NAME_LENGTH];
	inode copynode;
	int type;//0--directory  1--file
	vector<copydir*> copyqueue;
}copydir;

typedef struct {
	bool flag;
	string content;
}Msg;
/*
*	Super block
*/
struct filsys
{
	unsigned short s_num_inode;			//Total number of inodes.
	unsigned short s_num_finode;		//Total number of free inodes.
	unsigned short s_size_inode;		//Size of an inode.

	unsigned short s_num_block;			//Total number of blocks.
	unsigned short s_num_fblock;		//Total number of free blocks.
	unsigned short s_size_block;		//Size of a block.

	unsigned int special_stack[50];
	int special_free;
};

/*
*	Directory file(216B)
*/
struct directory
{
	char fileName[20][FILE_NAME_LENGTH];
	unsigned int inodeID[DIRECTORY_NUM];
};

/*
*	Accouting file(320B)
*/
struct userPsw
{
	unsigned short userID[ACCOUNT_NUM];
	char userName[ACCOUNT_NUM][USER_NAME_LENGTH];
	char password[ACCOUNT_NUM][USER_PASSWORD_LENGTH];
	unsigned short groupID[ACCOUNT_NUM];
};

extern FILE *fd ;
/*
*	Super block.
*/
extern filsys superBlock;
/*
*	Bitmaps for inodes and blocks. Element 1 stands for 'uesd', 0 for 'free'.
*/
extern unsigned short inode_bitmap[INODE_NUM];
/*
*	Accouting information.
*/
extern userPsw users;
/*
*	current user ID.
*/
extern unsigned short userID;
/*
*	current user name. used in command line.
*/
extern char userName[USER_NAME_LENGTH + 6];
/*
*	current directory.
*/
extern directory currentDirectory;

extern directory  rootDirectory;
/*
*	current directory stack.
*/
extern int bm[16][32];
extern char ab_dir[100][14];

extern unsigned short dir_pointer;

// find free block
void find_free_block(unsigned int &inode_number);

//recycle block
void recycle_block(unsigned int &inode_number);

/*
*	Formatting function of the file system, including the establishment
*	of superblock, inode chain, root directory, password file and so on.
*
*	return: the function return true only the file system is initialized
*			successfully.
*/
bool Format();

/*
*	Initialization function of the file system. Open an existing file system
*	from 'fs.han'.
*
*	return: the function return true only when the file system has been
*			formatted and is complete.
*/
bool Mount();




/*
*	Create a new empty file with the specific file name.
*
*	return: the function return true only when the new file is successfully
*			created.
*/
Msg CreateFile(const char* filename);

/*
*	Delete a file.
*
*	return: the function returns true only delete the file successfully.
*/
Msg DeleteFile(const char* filename);

/*
*	Open the specific file under current directory.
*
*	return: the function returns a pointer of the file's inode if the file is
*			successfully opened and NULL otherwise.
*/
inode  OpenFile(const char* filename);

/*
*	Append a string "content" to the specific file.
*
*	return: the function returns the number of bytes it has writen or -1 when
*			some error occur.
*/
Msg Write(inode& ifile, const char* content);

/*
*	Print the string "content" in the specific file.
*
*	return: none
*
*/
Msg PrintFile(inode& ifile);

/*
*	Create a new drirectory only with "." and ".." items.
*
*	return: the function returns true only when the new directory is
*			created successfully.
*/
Msg  MakeDir(const char* dirname);

/*
*	Delete a drirectory as well as all files and sub-directories in it.
*
*	return: the function returns true only when the directory as well
*			as all files and sub-directories in it is deleted successfully.
*/
Msg RemoveDir(const char* dirname);

/*
*	Open a directory.
*
*	return: the function returns true only when the directory is
*			opened successfully.
*/
Msg OpenDir(const char* dirname);


/*
*	Print absolute directory.
*/
string Ab_dir();

string Ab_arm_dir();

/*
*	Change file permission.
*/
Msg Chmod(char* filename, int tt, unsigned short premission);

/*
*	Change file' owner.
*/
Msg Chown(char* filename,int tt,char *username);

/*
*	Change file' group.
*/
Msg Chgrp(char* filename, int tt, int groupid);

/*
*	Change password.
*/
Msg Passwd(char *,char*);

string Access_1_Check(inode &inode,string op);
/*
*	User Management.
*/

Msg Copy(char* filename, inode & currentInode,char *aimpath);

bool ln(char* filename);

/*
*	Rename file/dir.
*/
int inti_main();

Msg  Rename(char* filename,char *newname,int tt);

int get_current_Inode(char *filename);
/*
*	Link.
*/
bool ln(char* filename);

bool ACCESS_check(string access, const unsigned short permission);//返回权限检查