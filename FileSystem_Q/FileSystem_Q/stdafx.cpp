#include"stdafx.h"
// find free block
FILE *fd = NULL;
/*
*	Super block.
*/
filsys superBlock;
/*
*	Bitmaps for inodes and blocks. Element 1 stands for 'uesd', 0 for 'free'.
*/
unsigned short inode_bitmap[INODE_NUM];
/*
*	Accouting information.
*/
userPsw users;
/*
*	current user ID.
*/
unsigned short userID = ACCOUNT_NUM;
int bm[16][32];
/*
*	current user name. used in command line.
*/
char userName[USER_NAME_LENGTH + 6];
/*
*	current directory.
*/
directory currentDirectory;

directory  rootDirectory;
/*
*	current directory stack.
*/
char ab_dir[100][14];

unsigned short dir_pointer;

void find_free_block(unsigned int &inode_number)
{
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fread(&superBlock, sizeof(filsys), 1, fd);
	if (superBlock.special_free == 0)
	{
		if (superBlock.special_stack[0] == 0)
		{
			printf("No value block!\n");
			return;
		}
		unsigned int stack[51];

		for (int i = 0; i < 50; i++)
		{
			stack[i] = superBlock.special_stack[i];
		}
		stack[50] = superBlock.special_free;
		fseek(fd, DATA_START + (superBlock.special_stack[0] - 50) * BLOCK_SIZE, SEEK_SET);
		fwrite(stack, sizeof(stack), 1, fd);

		fseek(fd, DATA_START + superBlock.special_stack[0] * BLOCK_SIZE, SEEK_SET);
		fread(stack, sizeof(stack), 1, fd);
		for (int i = 0; i < 50; i++)
		{
			superBlock.special_stack[i] = stack[i];
		}
		superBlock.special_free = stack[50];
	}
	inode_number = superBlock.special_stack[superBlock.special_free];
	superBlock.special_free--;
	superBlock.s_num_fblock--;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);
	FILE* fbm = fopen("./blockmap.txt", "r+");
	if (fbm == NULL)
	{
		printf("Fail to open  the file blockmap!\n");
		return;
	}
	int b[16][32];
	fread(b, sizeof(int) * 512, 1, fbm);
	b[inode_number / 32][inode_number % 32] = 1;
	fseek(fbm, 0, SEEK_SET);
	fwrite(b, sizeof(int) * 512, 1, fbm);
	fclose(fbm);
}

//recycle block
void recycle_block(unsigned int &inode_number)
{
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fread(&superBlock, sizeof(filsys), 1, fd);
	if (superBlock.special_free == 49)
	{
		unsigned int block_num;
		unsigned int stack[51];
		if (superBlock.special_stack[0] == 0)
			block_num = 499;
		else
			block_num = superBlock.special_stack[0] - 50;
		for (int i = 0; i < 50; i++)
		{
			stack[i] = superBlock.special_stack[i];
		}
		stack[50] = superBlock.special_free;
		fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
		fwrite(stack, sizeof(stack), 1, fd);
		block_num -= 50;
		fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
		fread(stack, sizeof(stack), 1, fd);
		for (int i = 0; i < 50; i++)
		{
			superBlock.special_stack[i] = stack[i];
		}
		superBlock.special_free = stack[50];
	}
	superBlock.special_free++;
	superBlock.s_num_fblock++;
	superBlock.special_stack[superBlock.special_free] = inode_number;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);
	FILE* fbm = fopen("./blockmap.txt", "r+");
	if (fbm == NULL)
	{
		printf("Fail to open  the file blockmap!\n");
		return;
	}

	int b[16][32];
	fread(b, sizeof(int) * 512, 1, fbm);
	b[inode_number / 32][inode_number % 32] = 0;
	fseek(fbm, 0, SEEK_SET);
	fwrite(b, sizeof(int) * 512, 1, fbm);
	fclose(fbm);

}

/*
*	Formatting function of the file system, including the establishment
*	of superblock, inode chain, root directory, password file and so on.
*
*	return: the function return true only the file system is initialized
*			successfully.
*/
bool Format()
{
	/*
	*	1. Create a empty file to emulate the file system.
	*/
	cout << "format" << endl;
	FILE * fd = fopen("./fs.han", "wb+");
	if (fd == NULL)
	{
		printf("Fail to initialize the file system!\n");
		return false;
	}
	FILE* fbm = fopen("./blockmap.txt", "r+");
	if (fbm == NULL)
	{
		printf("Fail to open the block file !\n");
		return false;
	}
	int b[16][32] = { 0 };

	b[0][0] = 1;
	b[0][1] = 1;
	fwrite(b, sizeof(int) * 512, 1, fbm);
	fclose(fbm);
	

	/*
	*	2. Initialize super block.
	*/
	//filsys superBlock;
	superBlock.s_num_inode = INODE_NUM;
	superBlock.s_num_block = BLOCK_NUM + 3 + 64; //3代表0空闲块、1超级块、2Inode位示图表,64块存inode
	superBlock.s_size_inode = INODE_SIZE;
	superBlock.s_size_block = BLOCK_SIZE;
	//Root directory and accounting file will use some inodes and blocks.
	superBlock.s_num_fblock = BLOCK_NUM - 2;
	superBlock.s_num_finode = INODE_NUM - 2;
	superBlock.special_stack[0] = 99;
	for (int i = 1; i < 50; i++)
	{
		superBlock.special_stack[i] = 49 - i;
	}
	superBlock.special_free = 47;
	//Write super block into file.
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);

	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fread(&superBlock, sizeof(filsys), 1, fd);

	/*
	*	3. Initialize inode and block bitmaps.
	*/
	unsigned short inode_bitmap[INODE_NUM];
	//Root directory and accounting file will use some inodes and blocks.
	memset(inode_bitmap, 0, INODE_NUM);
	inode_bitmap[0] = 1;
	inode_bitmap[1] = 1;
	//Write bitmaps into file.
	fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);

	//成组连接
	unsigned int stack[51];
	for (int i = 0; i < BLOCK_NUM / 50; i++)
	{
		memset(stack, 0, sizeof(stack));
		for (unsigned int j = 0; j < 50; j++)
		{
			stack[j] = (49 + i * 50) - j;
		}
		stack[0] = 49 + (i + 1) * 50;
		stack[50] = 49;
		fseek(fd, DATA_START + (49 + i * 50)*BLOCK_SIZE, SEEK_SET);
		fwrite(stack, sizeof(unsigned int) * 51, 1, fd);
	}
	memset(stack, 0, sizeof(stack));
	for (int i = 0; i < 12; i++)
	{
		stack[i] = 511 - i;
	}
	stack[0] = 0;
	stack[50] = 11;
	fseek(fd, DATA_START + 511 * BLOCK_SIZE, SEEK_SET);
	fwrite(stack, sizeof(unsigned int) * 51, 1, fd);

	fseek(fd, DATA_START + 49 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 99 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 149 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 199 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 249 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 299 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 349 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 399 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 449 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 499 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);
	fseek(fd, DATA_START + 511 * BLOCK_SIZE, SEEK_SET);
	fread(stack, sizeof(unsigned int) * 51, 1, fd);


	/*
	*	4. Create root directory.
	*/
	//Create inode
	//Now root directory contain 1 accounting file.
	inode iroot_tmp;
	iroot_tmp.i_ino = 0;					//Identification
	iroot_tmp.di_number = 2;				//Associations: itself and accouting file
	iroot_tmp.di_mode = 0;					//0 stands for directory
	iroot_tmp.di_size = 0;					//"For directories, the value is 0."
	memset(iroot_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
	iroot_tmp.di_addr[0] = 0;				//Root directory is stored on 1st block. FFFFFF means empty.
	iroot_tmp.permission = MAX_OWNER_PERMISSION;
	iroot_tmp.di_grp = 1;
	iroot_tmp.di_uid = 0;					//Root user id.
	iroot_tmp.icount = 0;
	time_t t = time(0);
	strftime(iroot_tmp.time, sizeof(iroot_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
	iroot_tmp.time[64] = 0;
	fseek(fd, INODE_START, SEEK_SET);
	fwrite(&iroot_tmp, sizeof(inode), 1, fd);

	//Create directory file.
	directory droot_tmp;
	memset(droot_tmp.fileName, 0, sizeof(char) * DIRECTORY_NUM * FILE_NAME_LENGTH);
	memset(droot_tmp.inodeID, -1, sizeof(unsigned int) * DIRECTORY_NUM);
	strcpy(droot_tmp.fileName[0], ".");
	droot_tmp.inodeID[0] = 0;
	strcpy(droot_tmp.fileName[1], "..");
	droot_tmp.inodeID[1] = 0;
	//A sub directory for accounting files
	strcpy(droot_tmp.fileName[2], "pw");
	droot_tmp.inodeID[2] = 1;

	//Write
	fseek(fd, DATA_START, SEEK_SET);
	fwrite(&droot_tmp, sizeof(directory), 1, fd);

	/*
	*	5. Create accouting file.
	*/
	//Create inode
	inode iaccouting_tmp;
	iaccouting_tmp.i_ino = 1;					//Identification
	iaccouting_tmp.di_number = 1;				//Associations
	iaccouting_tmp.di_mode = 1;					//1 stands for file
	iaccouting_tmp.di_size = sizeof(userPsw);	//File size
	memset(iaccouting_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
	iaccouting_tmp.di_addr[0] = 1;				//Root directory is stored on 1st block.
	iaccouting_tmp.di_uid = 0;					//Root user id.
	iaccouting_tmp.di_grp = 1;
	iaccouting_tmp.permission = 320;
	iaccouting_tmp.icount = 0;
	t = time(0);
	strftime(iaccouting_tmp.time, sizeof(iaccouting_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
	iaccouting_tmp.time[64] = 0;
	fseek(fd, INODE_START + INODE_SIZE, SEEK_SET);
	fwrite(&iaccouting_tmp, sizeof(inode), 1, fd);

	//Create accouting file.
	userPsw paccouting_tmp;
	memset(paccouting_tmp.userName, 0, sizeof(char) * USER_NAME_LENGTH * ACCOUNT_NUM);
	memset(paccouting_tmp.password, 0, sizeof(char) * USER_PASSWORD_LENGTH * ACCOUNT_NUM);
	//Only default user 'admin' is registered. Default password is 'admin'.
	strcpy(paccouting_tmp.userName[0], "admin");
	strcpy(paccouting_tmp.userName[1], "guest");
	strcpy(paccouting_tmp.password[0], "admin");
	strcpy(paccouting_tmp.password[1], "123456");
	//0 stands for super user. Other IDs are only used to identify users.
	for (unsigned short i = 0; i < ACCOUNT_NUM; i++)
	{
		paccouting_tmp.userID[i] = i;
	}
	paccouting_tmp.groupID[0] = 1;
	paccouting_tmp.groupID[1] = 2;
	//Write
	fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
	fwrite(&paccouting_tmp, sizeof(userPsw), 1, fd);

	//Close file.
	fclose(fd);

	return true;
};

/*
*	Initialization function of the file system. Open an existing file system
*	from 'fs.han'.
*
*	return: the function return true only when the file system has been
*			formatted and is complete.
*/
bool Mount()
{
	/*
	*	1. Open the emulation file where the file system is installed.
	*/
	fd = fopen("./fs.han", "rb+");
	if (fd == NULL)
	{
		printf("Error: File system not found!\n");
		return false;
	}
	/*
	*	2. Read superblock, bitmaps, accouting file, current directory (root)
	*/
	//Read superblock
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fread(&superBlock, sizeof(superBlock), 1, fd);
	//Read inode bitmap
	fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	fread(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
	//Read current directory, namely root directory
	fseek(fd, DATA_START, SEEK_SET);
	fread(&currentDirectory, sizeof(directory), 1, fd);

	//Read accouting file
	fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
	fread(&users, sizeof(userPsw), 1, fd);

	return true;
};

/*
*	Create a new empty file with the specific file name.
*
*	return: the function return true only when the new file is successfully
*			created.
*/
Msg CreateFile(const char* filename)
{
	Msg msg;
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		msg.content = " Error: Illegal file name.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	1. Check whether free inodes and blocks are used up.
	*/
	if (superBlock.s_num_fblock <= 0 || superBlock.s_num_finode <= 0)
	{
		msg.content = "File creation error: No valid spaces.\n";
		msg.flag = false;
		return msg;
	}
	//Find new inode number and new block address
	int new_ino = 0;
	unsigned int new_block_addr = -1;
	for (; new_ino < INODE_NUM; new_ino++)
	{
		if (inode_bitmap[new_ino] == 0)
		{
			break;
		}
	}

	/*
	*	2. Check whether file name has been used in current directory.
	*/
	for (int i = 0; i < DIRECTORY_NUM; i++)
	{
		if (strcmp(currentDirectory.fileName[i], filename) == 0)
		{
			inode* tmp_file_inode = new inode;
			int tmp_file_ino = currentDirectory.inodeID[i];
			fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
			fread(tmp_file_inode, sizeof(inode), 1, fd);
			if (tmp_file_inode->di_mode == 0) continue;
			else {
				msg.content = "File creation error: File name '" + string(currentDirectory.fileName[i]) + "' has been used.\n";
				msg.flag = false;
				return msg;
			}
		}
	}

	/*
	*	3. Check whether current directory contains too many items already.
	*/
	int itemCounter = 0;
	for (int i = 0; i < DIRECTORY_NUM; i++)
	{
		if (strlen(currentDirectory.fileName[i]) > 0)
		{
			itemCounter++;
		}
	}
	if (itemCounter >= DIRECTORY_NUM)
	{
		msg.content = "File creation error : Too many files or directories in current path.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	4. Create new inode.
	*/
	//Create inode
	inode ifile_tmp;
	ifile_tmp.i_ino = new_ino;				//Identification
	ifile_tmp.di_number = 1;				//Associations
	ifile_tmp.di_mode = 1;					//1 stands for file
	ifile_tmp.di_size = 0;					//New file is empty
	memset(ifile_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
	ifile_tmp.di_uid = userID;				//Current user id.
	ifile_tmp.di_grp = users.groupID[userID];//Current user group id
	ifile_tmp.permission = MAX_PERMISSION;
	ifile_tmp.icount = 0;
	time_t t = time(0);
	strftime(ifile_tmp.time, sizeof(ifile_tmp.time), "%Y/%m/%d %X %A", localtime(&t));
	ifile_tmp.time[64];
	fseek(fd, INODE_START + new_ino * INODE_SIZE, SEEK_SET);
	fwrite(&ifile_tmp, sizeof(inode), 1, fd);

	/*
	*	5.  Update bitmaps.
	*/
	//Update bitmaps
	inode_bitmap[new_ino] = 1;
	fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);

	/*
	*	6. Update directory.
	*/
	//Fetch current directory's inode
	//Inode position of current directory


	int pos_directory_inode = 0;
	pos_directory_inode = currentDirectory.inodeID[0]; //"."
	inode tmp_directory_inode;
	fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
	fread(&tmp_directory_inode, sizeof(inode), 1, fd);

	//Add to current directory item
	for (int i = 2; i < DIRECTORY_NUM; i++)
	{
		if (strlen(currentDirectory.fileName[i]) == 0)
		{
			strcat(currentDirectory.fileName[i], filename);
			currentDirectory.inodeID[i] = new_ino;
			break;
		}
	}

	//write
	fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fwrite(&currentDirectory, sizeof(directory), 1, fd);

	//Update associations
	directory tmp_directory = currentDirectory;
	int tmp_pos_directory_inode = pos_directory_inode;
	while (true)
	{
		//Update association
		tmp_directory_inode.di_number++;
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
		//If reach the root directory, finish updating.
		if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
		{
			break;
		}
		//Fetch father directory
		tmp_pos_directory_inode = tmp_directory.inodeID[1];		//".."
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fread(&tmp_directory_inode, sizeof(inode), 1, fd);
		fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fread(&tmp_directory, sizeof(directory), 1, fd);
	}

	/*
	*	7. Update super block.
	*/
	//superBlock.s_num_fblock--;

	superBlock.s_num_finode--;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);
	msg.flag = true;
	msg.content = "OK";
	return msg;
};

/*
*	Delete a file.
*
*	return: the function returns true only delete the file successfully.
*/
Msg DeleteFile(const char* filename)
{
	Msg msg;
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		msg.content = "Error: Illegal file name.\n";
		cout << msg.content;
		msg.flag = false;
		return msg;
	}

	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1, tmp_file_ino;
	inode tmp_file_inode;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			msg.content = "Delete error: File not found.\n";
			cout << msg.content;
			msg.flag = false;
			return msg;
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode.di_mode == 0);	//is a directory, roll back and continue to search the file

	//Access check
	if (Access_1_Check(tmp_file_inode, "Delete") != "OK") {
		msg.content = "Delete error: No Access\n";
		cout << msg.content;
		msg.flag = false;
		return msg;
	}
		

	/*
	*	3. Start deleting. Fill the inode's original space with 0.
	*/
	if (tmp_file_inode.icount > 0) {
		tmp_file_inode.icount--;
		fseek(fd, INODE_START + tmp_file_inode.i_ino * INODE_SIZE, SEEK_SET);
		fwrite(&tmp_file_inode, sizeof(inode), 1, fd);
		/*
		*	Update directories
		*/
		//Fetch current directory inode
		int pos_directory_inode = currentDirectory.inodeID[0];	//"."
		inode tmp_directory_inode;
		fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
		fread(&tmp_directory_inode, sizeof(inode), 1, fd);

		//Update current directory item
		memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
		currentDirectory.inodeID[pos_in_directory] = -1;
		fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fwrite(&currentDirectory, sizeof(directory), 1, fd);

		//Update associations
		directory tmp_directory = currentDirectory;
		int tmp_pos_directory_inode = pos_directory_inode;
		while (true)
		{
			//Update association
			tmp_directory_inode.di_number--;
			fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
			fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
			//If reach the root directory, finish updating.
			if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
			{
				break;
			}
			//Fetch father directory
			tmp_pos_directory_inode = tmp_directory.inodeID[1];		//".."
			fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
			fread(&tmp_directory_inode, sizeof(inode), 1, fd);


			fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
			fread(&tmp_directory, sizeof(directory), 1, fd);
		}
		msg.flag = true;
		msg.content = "OK";
		return msg;
	}
	//Fill original space
	int tmp_fill[sizeof(inode)];
	memset(tmp_fill, 0, sizeof(inode));
	fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
	fwrite(&tmp_fill, sizeof(inode), 1, fd);

	/*
	*	4. Update bitmaps
	*/
	//inode bitmap
	inode_bitmap[tmp_file_ino] = 0;
	fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	fwrite(&inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
	//block bitmap

	for (int i = 0; i < NADDR - 2; i++)
	{
		if (tmp_file_inode.di_addr[i] != -1)
			recycle_block(tmp_file_inode.di_addr[i]);
		else break;
	}
	if (tmp_file_inode.di_addr[NADDR - 2] != -1) {
		unsigned int f1[128];
		fseek(fd, DATA_START + tmp_file_inode.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
		fread(f1, sizeof(f1), 1, fd);
		for (int k = 0; k < 128; k++) {
			recycle_block(f1[k]);
		}
		recycle_block(tmp_file_inode.di_addr[NADDR - 2]);
	}

	/*
	*	5. Update directories
	*/
	//Fetch current directory inode
	int pos_directory_inode = currentDirectory.inodeID[0];	//"."
	inode tmp_directory_inode;
	fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
	fread(&tmp_directory_inode, sizeof(inode), 1, fd);

	//Update current directory item
	memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
	currentDirectory.inodeID[pos_in_directory] = -1;
	fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fwrite(&currentDirectory, sizeof(directory), 1, fd);

	//Update associations
	directory tmp_directory = currentDirectory;
	int tmp_pos_directory_inode = pos_directory_inode;
	while (true)
	{
		//Update association
		tmp_directory_inode.di_number--;
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
		//If reach the root directory, finish updating.
		if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
		{
			break;
		}
		//Fetch father directory
		tmp_pos_directory_inode = tmp_directory.inodeID[1];		//".."
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fread(&tmp_directory_inode, sizeof(inode), 1, fd);
		fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fread(&tmp_directory, sizeof(directory), 1, fd);
	}

	/*
	*	6. Update super block
	*/
	//superBlock.s_num_fblock += tmp_file_inode.di_size / BLOCK_SIZE + 1;
	superBlock.s_num_finode++;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);

	msg.flag = true;
	msg.content = "OK";
	return msg;
}

/*
*	Open the specific file under current directory.
*
*	return: the function returns a pointer of the file's inode if the file is
*			successfully opened and NULL otherwise.
*/
inode  OpenFile(const char* filename)
{
	inode tmp_file_inode;
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		printf("Error: Illegal file name.\n");
		return tmp_file_inode;
	}

	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1;
	
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			printf("Open file error: File not found.\n");
			return tmp_file_inode;
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode.di_mode == 0);
	cout << "Openfile finished" << endl;
	return tmp_file_inode;
};

/*
*	Append a string "content" to the specific file.
*
*	return: the function returns the number of bytes it has writen or -1 when
*			some error occur.
*/
Msg Write(inode& ifile, const char* content)
{
	Msg msg;
	//parameter check
	if (content == NULL)
	{
		msg.content = "Error: Illegal file name.\n";
		msg.flag = false;
		printf("Error: Illegal file name.\n");
		return msg;
	}
	//Access check
	msg.content = Access_1_Check(ifile, "Write");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}

	/*
	*	1. Check whether the expected file will be out of length.
	*/
	int len_content = strlen(content);
	unsigned int new_file_length = len_content + ifile.di_size;
	if (new_file_length >= FILE_SIZE_MAX)
	{
		msg.content = "Write error: File over length.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	2. Get the number of needed blocks and check is there any enough free spaces.
	*/
	//Get the number of needed blocks
	unsigned int block_num;
	if (ifile.di_addr[0] == -1)	block_num = -1;
	else
	{
		for (int i = 0; i < NADDR - 2; i++)
		{
			if (ifile.di_addr[i] != -1)
				block_num = ifile.di_addr[i];
			else break;
		}
		int f1[128];
		fseek(fd, DATA_START + ifile.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
		int num;
		if (ifile.di_size%BLOCK_SIZE == 0)
			num = ifile.di_size / BLOCK_SIZE;
		else num = ifile.di_size / BLOCK_SIZE + 1;
		if (num > 4 && num <= 132)
		{
			fseek(fd, DATA_START + ifile.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
			fread(f1, sizeof(f1), 1, fd);
			block_num = f1[num - 4];
		}

	}
	int free_space_firstBlock = BLOCK_SIZE - ifile.di_size % BLOCK_SIZE;
	unsigned int num_block_needed;
	if (len_content - free_space_firstBlock > 0)
	{
		num_block_needed = (len_content - free_space_firstBlock) / BLOCK_SIZE + 1;
	}
	else
	{
		num_block_needed = 0;
	}
	//Check is there any enough free spaces
	if (num_block_needed > superBlock.s_num_fblock)
	{
		msg.content = "Write error: No enough space available.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	3. Write first block.
	*/
	if (ifile.di_addr[0] == -1)
	{
		find_free_block(block_num);
		ifile.di_addr[0] = block_num;
		fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
	}
	else
		fseek(fd, DATA_START + block_num * BLOCK_SIZE + ifile.di_size % BLOCK_SIZE, SEEK_SET);
	char data[BLOCK_SIZE];
	if (num_block_needed == 0)
	{
		fwrite(content, len_content, 1, fd);
		fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
		fread(data, sizeof(data), 1, fd);
		ifile.di_size += len_content;
	}
	else
	{
		fwrite(content, free_space_firstBlock, 1, fd);
		fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
		fread(data, sizeof(data), 1, fd);
		ifile.di_size += free_space_firstBlock;
	}

	/*
	*	4. Write the other blocks. Update file information in inode and block bitmap in the meanwhile.
	*/
	char write_buf[BLOCK_SIZE];
	unsigned int new_block_addr = -1;
	unsigned int content_write_pos = free_space_firstBlock;
	//Loop and write each blocks
	if ((len_content + ifile.di_size) / BLOCK_SIZE + ((len_content + ifile.di_size) % BLOCK_SIZE == 0 ? 0 : 1) <= NADDR - 2) {
		//direct addressing
		for (int i = 0; i < num_block_needed; i++)
		{
			find_free_block(new_block_addr);
			if (new_block_addr == -1) {
				msg.content = "no enough space";
				msg.flag = false;
				return msg ;
			}
			for (int j = 0; j < NADDR - 2; j++)
			{
				if (ifile.di_addr[j] == -1)
				{
					ifile.di_addr[j] = new_block_addr;
					break;
				}
			}
			memset(write_buf, 0, BLOCK_SIZE);
			//Copy from content to write buffer
			unsigned int tmp_counter = 0;
			for (; tmp_counter < BLOCK_SIZE; tmp_counter++)
			{
				if (content[content_write_pos + tmp_counter] == '\0')
					break;
				write_buf[tmp_counter] = content[content_write_pos + tmp_counter];
			}
			content_write_pos += tmp_counter;
			//Write
			fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
			fwrite(write_buf, tmp_counter, 1, fd);
			fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
			fread(data, sizeof(data), 1, fd);
			//Update inode information: blocks address and file size
			ifile.di_size += tmp_counter;
		}
	}
	else if ((len_content + ifile.di_size) / BLOCK_SIZE + ((len_content + ifile.di_size) % BLOCK_SIZE == 0 ? 0 : 1) > NADDR - 2) {
		//direct addressing
		for (int i = 0; i < NADDR - 2; i++)
		{
			if (ifile.di_addr[i] != -1)continue;

			memset(write_buf, 0, BLOCK_SIZE);
			new_block_addr = -1;

			find_free_block(new_block_addr);
			if (new_block_addr == -1) {
				msg.content = "no enough space";
				msg.flag = false;
				return msg;
			}
			ifile.di_addr[i] = new_block_addr;
			//Copy from content to write buffer
			unsigned int tmp_counter = 0;
			for (; tmp_counter < BLOCK_SIZE; tmp_counter++)
			{
				if (content[content_write_pos + tmp_counter] == '\0') {
					break;
				}
				write_buf[tmp_counter] = content[content_write_pos + tmp_counter];
			}
			content_write_pos += tmp_counter;
			//Write
			fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
			fwrite(write_buf, tmp_counter, 1, fd);

			//Update inode information: blocks address and file size
			ifile.di_size += tmp_counter;
		}
		//first indirect addressing
		int cnt = 0;
		unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = { 0 };

		new_block_addr = -1;
		find_free_block(new_block_addr);
		if (new_block_addr == -1) {
			msg.content = "no enough space";
			msg.flag = false;
			return msg;
		}
		ifile.di_addr[NADDR - 2] = new_block_addr;
		for (int i = 0; i < BLOCK_SIZE / sizeof(unsigned int); i++)
		{
			new_block_addr = -1;
			find_free_block(new_block_addr);
			if (new_block_addr == -1) {
				msg.content = "no enough space";
				msg.flag = false;
				return msg;
			}
			else
				f1[i] = new_block_addr;
		}
		fseek(fd, DATA_START + ifile.di_addr[4] * BLOCK_SIZE, SEEK_SET);
		fwrite(f1, sizeof(f1), 1, fd);
		bool flag = 0;
		for (int j = 0; j < BLOCK_SIZE / sizeof(int); j++) {
			fseek(fd, DATA_START + f1[j] * BLOCK_SIZE, SEEK_SET);
			//Copy from content to write buffer
			unsigned int tmp_counter = 0;
			for (; tmp_counter < BLOCK_SIZE; tmp_counter++)
			{
				if (content[content_write_pos + tmp_counter] == '\0') {
					//tmp_counter--;
					flag = 1;
					break;
				}
				write_buf[tmp_counter] = content[content_write_pos + tmp_counter];
			}
			content_write_pos += tmp_counter;
			fwrite(write_buf, tmp_counter, 1, fd);
			ifile.di_size += tmp_counter;
			if (flag == 1) break;
		}
	}
	time_t t = time(0);
	strftime(ifile.time, sizeof(ifile.time), "%Y/%m/%d %X %A", localtime(&t));
	ifile.time[64] = 0;
	//Write inode
	fseek(fd, INODE_START + ifile.i_ino * INODE_SIZE, SEEK_SET);
	fwrite(&ifile, sizeof(inode), 1, fd);

	/*
	*	5. Update super block.
	*/
	//superBlock.s_num_fblock -= num_block_needed;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(superBlock), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
};

/*
*	Print the string "content" in the specific file.
*
*	return: none
*
*/
Msg PrintFile(inode& ifile)
{
	Msg msg;
	//Access check
	msg.content = Access_1_Check(ifile, "Read");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
	msg.content.clear();
	int block_num = ifile.di_size / BLOCK_SIZE + 1;
	int print_line_num = 0;		//16 bytes per line.
	//Read file from data blocks
	char stack[BLOCK_SIZE];
	if (block_num <= NADDR - 2)
	{
		for (int i = 0; i < block_num; i++)
		{
			fseek(fd, DATA_START + ifile.di_addr[i] * BLOCK_SIZE, SEEK_SET);
			fread(stack, sizeof(stack), 1, fd);
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				if (stack[j] == '\0')break;
				if (j % 16 == 0)
				{
					printf("\n");
					printf("%d\t", ++print_line_num);
				}
				printf("%c", stack[j]);	
				msg.content.push_back(stack[j]);
			}
		}
		//int i = 0;
	}
	else if (block_num > NADDR - 2) {
		//direct addressing
		for (int i = 0; i < NADDR - 2; i++)
		{
			fseek(fd, DATA_START + ifile.di_addr[i] * BLOCK_SIZE, SEEK_SET);
			fread(stack, sizeof(stack), 1, fd);
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				
				if (stack[j] == '\0')break;
				if (j % 16 == 0)
				{
					printf("\n");
					printf("%d\t", ++print_line_num);
				}
				printf("%c", stack[j]);
				msg.content.push_back(stack[j]);
			}
		}

		//first indirect addressing
		unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = { 0 };
		fseek(fd, DATA_START + ifile.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
		fread(f1, sizeof(f1), 1, fd);
		for (int i = 0; i < block_num - (NADDR - 2); i++) {
			fseek(fd, DATA_START + f1[i] * BLOCK_SIZE, SEEK_SET);
			fread(stack, sizeof(stack), 1, fd);
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				if (stack[j] == '\0')  break;
				if (j % 16 == 0)
				{
					printf("\n");
					printf("%d\t", ++print_line_num);
				}
				printf("%c", stack[j]);
				msg.content.push_back(stack[j]);
			}
		}
	}
	msg.flag = true;
	return msg;
};

/*
*	Create a new drirectory only with "." and ".." items.
*
*	return: the function returns true only when the new directory is
*			created successfully.
*/
Msg MakeDir(const char* dirname)
{
	cout << "MakeDir:" << endl;
	Msg msg;
	//parameter check
	if (dirname == NULL || strlen(dirname) > FILE_NAME_LENGTH)
	{
		msg.content="Error: Illegal directory name.\n";
		msg.flag = false;
		return msg;
	}
	/*
	*	1. Check whether free inodes and blocks are used up.
	*/
	if (superBlock.s_num_fblock <= 0 || superBlock.s_num_finode <= 0)
	{
		msg.content = "File creation error: No valid spaces.\n";
		msg.flag = false;
		return msg;
	}
	//Find new inode number and new block address
	cout << "Find new inode number and new block address" << endl;
	int new_ino = 0;
	unsigned int new_block_addr = 0;
	for (; new_ino < INODE_NUM; new_ino++)
	{
		if (inode_bitmap[new_ino] == 0)
		{
			break;
		}
	}
	find_free_block(new_block_addr);
	if (new_block_addr == -1) {
		msg.content = "no enough space\n";
		msg.flag = false;
		return msg;
	}
	if (new_ino == INODE_NUM || new_block_addr == BLOCK_NUM)
	{
		msg.content = "File creation error: No valid spaces.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	2. Check whether directory name has been used in current directory.
	*/
	for (int i = 0; i < DIRECTORY_NUM; i++)
	{
		if (strcmp(currentDirectory.fileName[i], dirname) == 0)
		{
			inode* tmp_file_inode = new inode;
			int tmp_file_ino = currentDirectory.inodeID[i];
			fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
			fread(tmp_file_inode, sizeof(inode), 1, fd);
			if (tmp_file_inode->di_mode == 1) continue;
			else {
				msg.content = "File creation error: Directory name '" + string(currentDirectory.fileName[i]) + "' has been used.\n";
				msg.flag = false;
				return msg;
			}
		}
	}

	/*
	*	3. Check whether current directory contains too many items already.
	*/
	int itemCounter = 0;
	for (int i = 0; i < DIRECTORY_NUM; i++)
	{
		if (strlen(currentDirectory.fileName[i]) > 0)
		{
			itemCounter++;
		}
	}
	if (itemCounter >= DIRECTORY_NUM)
	{
		msg.content = "File creation error: Too many files or directories in current path.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	4. Create new inode.
	*/
	//Create inode
	inode idir_tmp;
	idir_tmp.i_ino = new_ino;				//Identification
	idir_tmp.di_number = 1;					//Associations
	idir_tmp.di_mode = 0;					//0 stands for directory
	idir_tmp.di_size = sizeof(directory);	//"For directories, the value is 0."
	memset(idir_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
	idir_tmp.di_addr[0] = new_block_addr;
	idir_tmp.di_uid = userID;				//Current user id.
	idir_tmp.di_grp = users.groupID[userID];
	time_t t = time(0);
	strftime(idir_tmp.time, sizeof(idir_tmp.time), "%Y/%m/%d %X %A", localtime(&t));
	idir_tmp.time[64] = 0;
	idir_tmp.icount = 0;
	idir_tmp.permission = MAX_PERMISSION;
	fseek(fd, INODE_START + new_ino * INODE_SIZE, SEEK_SET);
	fwrite(&idir_tmp, sizeof(inode), 1, fd);

	/*
	*	5. Create directory file.
	*/
	directory tmp_dir;
	memset(tmp_dir.fileName, 0, sizeof(char) * DIRECTORY_NUM * FILE_NAME_LENGTH);
	memset(tmp_dir.inodeID, -1, sizeof(unsigned int) * DIRECTORY_NUM);
	strcpy(tmp_dir.fileName[0], ".");
	tmp_dir.inodeID[0] = new_ino;
	strcpy(tmp_dir.fileName[1], "..");
	tmp_dir.inodeID[1] = currentDirectory.inodeID[0];
	fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
	fwrite(&tmp_dir, sizeof(directory), 1, fd);

	/*
	*	6.  Update bitmaps.
	*/
	//Update bitmaps
	inode_bitmap[new_ino] = 1;
	fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);

	/*
	*	7. Update directory.
	*/
	//Fetch current directory's inode
	//Inode position of current directory
	int pos_directory_inode = 0;
	pos_directory_inode = currentDirectory.inodeID[0]; //"."
	inode tmp_directory_inode;
	fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
	fread(&tmp_directory_inode, sizeof(inode), 1, fd);

	//Add to current directory item
	for (int i = 2; i < DIRECTORY_NUM; i++)
	{
		if (strlen(currentDirectory.fileName[i]) == 0)
		{
			strcat(currentDirectory.fileName[i], dirname);
			currentDirectory.inodeID[i] = new_ino;
			break;
		}
	}
	//write
	fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fwrite(&currentDirectory, sizeof(directory), 1, fd);

	//Update associations
	directory tmp_directory = currentDirectory;
	int tmp_pos_directory_inode = pos_directory_inode;
	while (true)
	{
		//Update association
		tmp_directory_inode.di_number++;
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
		//If reach the root directory, finish updating.
		if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
		{
			break;
		}
		//Fetch father directory
		tmp_pos_directory_inode = tmp_directory.inodeID[1];		//".."
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fread(&tmp_directory_inode, sizeof(inode), 1, fd);
		fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fread(&tmp_directory, sizeof(directory), 1, fd);
	}

	/*
	*	8. Update super block.
	*/
	//superBlock.s_num_fblock--;
	superBlock.s_num_finode--;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
};

/*
*	Delete a drirectory as well as all files and sub-directories in it.
*
*	return: the function returns true only when the directory as well
*			as all files and sub-directories in it is deleted successfully.
*/
Msg RemoveDir(const char* dirname)
{
	Msg msg;
	//parameter check
	if (dirname == NULL || strlen(dirname) > FILE_NAME_LENGTH)
	{
		msg.content = "Error: Illegal directory name.\n";
		msg.flag = false;
		return msg;
	}

	/*
	*	1. Check whether the directory exists in current directory.
	*/
	int pos_in_directory = 0;
	int tmp_dir_ino;
	inode tmp_dir_inode;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], dirname) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			msg.content = "Delete error: Directory not found.\n";
			msg.flag = false;
			return msg;
		}

		/*
		*	2. Fetch inode and check whether it's a file.
		*/
		//Fetch inode
		tmp_dir_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_dir_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_dir_inode.di_mode == 1);

	/*
	*	3. Access check.
	*/
	msg.content = Access_1_Check(tmp_dir_inode, "Delete");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
		
	/*
	*	4. Start deleting. Delete all sub-directories and files first.
	*/
	if (tmp_dir_inode.icount > 0) {
		tmp_dir_inode.icount--;
		fseek(fd, INODE_START + tmp_dir_inode.i_ino * INODE_SIZE, SEEK_SET);
		fwrite(&tmp_dir_inode, sizeof(inode), 1, fd);
		/*
		*	Update directories
		*/
		//Fetch current directory inode
		int pos_directory_inode = currentDirectory.inodeID[0];	//"."
		inode tmp_directory_inode;
		fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
		fread(&tmp_directory_inode, sizeof(inode), 1, fd);

		//Update current directory item
		memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
		currentDirectory.inodeID[pos_in_directory] = -1;
		fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fwrite(&currentDirectory, sizeof(directory), 1, fd);

		//Update associations
		directory tmp_directory = currentDirectory;
		int tmp_pos_directory_inode = pos_directory_inode;
		while (true)
		{
			//Update association
			tmp_directory_inode.di_number--;
			fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
			fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
			//If reach the root directory, finish updating.
			if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
			{
				break;
			}
			//Fetch father directory
			tmp_pos_directory_inode = tmp_directory.inodeID[1];		//".."
			fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
			fread(&tmp_directory_inode, sizeof(inode), 1, fd);
			fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
			fread(&tmp_directory, sizeof(directory), 1, fd);
		}
		msg.content = "OK";
		msg.flag = true;
		return msg;
	}

	directory tmp_dir;
	fseek(fd, DATA_START + tmp_dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fread(&tmp_dir, sizeof(directory), 1, fd);

	//Search all sub files and directories and delete them.
	inode tmp_sub_inode;
	char tmp_sub_filename[FILE_NAME_LENGTH];
	memset(tmp_sub_filename, 0, FILE_NAME_LENGTH);
	for (int i = 2; i < DIRECTORY_NUM; i++)
	{
		if (strlen(tmp_dir.fileName[i]) > 0)
		{
			strcpy(tmp_sub_filename, tmp_dir.fileName[i]);
			//Determine whether it's a file or a directory.
			fseek(fd, INODE_START + tmp_dir.inodeID[i] * INODE_SIZE, SEEK_SET);
			fread(&tmp_sub_inode, sizeof(inode), 1, fd);
			//Before delete sub files and directories, change current directory first and recover after deleting.
			directory tmp_swp;
			tmp_swp = currentDirectory;
			currentDirectory = tmp_dir;
			tmp_dir = tmp_swp;
			//Is a file.
			if (tmp_sub_inode.di_mode == 1)
			{
				DeleteFile(tmp_sub_filename);
			}
			//Is a directory.
			else if (tmp_sub_inode.di_mode == 0)
			{
				RemoveDir(tmp_sub_filename);
			}
			tmp_swp = currentDirectory;
			currentDirectory = tmp_dir;
			tmp_dir = tmp_swp;
		}
	}

	/*
	*	5. Start deleting itself. Fill the inode's original space with 0s.
	*/
	//Fill original space
	int tmp_fill[sizeof(inode)];
	memset(tmp_fill, 0, sizeof(inode));
	fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
	fwrite(&tmp_fill, sizeof(inode), 1, fd);

	/*
	*	6. Update bitmaps
	*/
	//inode bitmap
	inode_bitmap[tmp_dir_ino] = 0;
	fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	fwrite(&inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
	//block bitmap
	for (int i = 0; i < (tmp_dir_inode.di_size / BLOCK_SIZE + 1); i++)
	{
		recycle_block(tmp_dir_inode.di_addr[i]);
	}

	/*
	*	7. Update directories
	*/
	//Fetch current directory inode
	int pos_directory_inode = currentDirectory.inodeID[0];	//"."
	inode tmp_directory_inode;
	fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
	fread(&tmp_directory_inode, sizeof(inode), 1, fd);
	//Update current directory item
	memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
	currentDirectory.inodeID[pos_in_directory] = -1;
	fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * INODE_SIZE, SEEK_SET);
	fwrite(&currentDirectory, sizeof(directory), 1, fd);
	//Update associations
	directory tmp_directory = currentDirectory;
	int tmp_pos_directory_inode = pos_directory_inode;
	while (true)
	{
		//Update association
		tmp_directory_inode.di_number--;
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
		//If reach the root directory, finish updating.
		if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
		{
			break;
		}
		//Fetch father directory
		tmp_pos_directory_inode = tmp_directory.inodeID[1];		//".."
		fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
		fread(&tmp_directory_inode, sizeof(inode), 1, fd);
		fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
		fread(&tmp_directory, sizeof(directory), 1, fd);
	}
	/*
	*	78 Update super block
	*/
	//superBlock.s_num_fblock += tmp_dir_inode.di_size / BLOCK_SIZE + 1;
	superBlock.s_num_finode++;
	fseek(fd, BLOCK_SIZE, SEEK_SET);
	fwrite(&superBlock, sizeof(filsys), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
};

/*
*	Open a directory.
*
*	return: the function returns true only when the directory is
*			opened successfully.
*/
Msg OpenDir(const char* dirname)
{
	Msg msg;
	//parameter check
	if (dirname == NULL || strlen(dirname) > FILE_NAME_LENGTH)
	{
		msg.content = "Error: Illegal directory name.\n";
		msg.flag = false;
		return msg;
	}
	/*
	*	1. Check whether the directory exists in current directory.
	*/
	int pos_in_directory = 0;
	inode tmp_dir_inode;
	int tmp_dir_ino;
	do {
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], dirname) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			msg.content = "Open error : Directory not found.\n";
			msg.flag = false;
			return msg;
		}

		/*
		*	2. Fetch inode and check whether it's a file.
		*/
		//Fetch inode
		tmp_dir_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_dir_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_dir_inode.di_mode == 1);
	//ACCESS CHECK
	msg.content = Access_1_Check(tmp_dir_inode, "Open dir");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
	/*
	*	3. Update current directory.
	*/
	directory new_current_dir;
	fseek(fd, DATA_START + tmp_dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fread(&new_current_dir, sizeof(directory), 1, fd);
	currentDirectory = new_current_dir;
	
	if (dirname[0] == '.' && dirname[1] == 0) {
		dir_pointer;
	}
	else if (dirname[0] == '.' && dirname[1] == '.' && dirname[2] == 0) {
		if (dir_pointer != 0) dir_pointer--;
	}
	else {
		for (int i = 0; i < 14; i++) {
			ab_dir[dir_pointer][i] = dirname[i];
		}
		dir_pointer++;
	}
	//show_info();
	msg.content = "OK";
	msg.flag = true;
	return msg;
};

/*
*	Print absolute directory.
*/
string Ab_dir() {
	string ab_dir_path = "";
	for (int i = 0; i < dir_pointer; i++) {
		printf("%s/", ab_dir[i]);
		ab_dir_path += ">";
		ab_dir_path += ab_dir[i];
	}
	printf("\n");
	return ab_dir_path;
}
string Ab_arm_dir() {
	string ab_dir_path = "";
	for (int i = 0; i < dir_pointer; i++) {
		printf("%s/", ab_dir[i]);
		ab_dir_path += ab_dir[i];
		ab_dir_path += "/";
	}
	cout<<"当前文件夹" << ab_dir_path << endl;
	return ab_dir_path;
}

/*
*	Change file permission.
*/
Msg Chmod(char* filename,int tt,unsigned short premission) {
	printf("File or Dir?(0 stands for file, 1 for dir):");
	//parameter check
	Msg msg;
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		msg.content = "Error: Illegal file name.\n";
		msg.flag = false;
		printf("Error: Illegal file name.\n");
		return msg;
	}

	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1;
	inode  tmp_file_inode ;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			msg.content = "Not found.\n";
			msg.flag = false;
			return msg;
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode.di_mode == tt);

	printf("Please input 0&1 string(1 stands for having, 0 for not)\n");
	printf("Format: rwerwerwe\n");
	msg.content = Access_1_Check(tmp_file_inode, "Change permission");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
	cout << "change permission: " << premission << endl;
	tmp_file_inode.permission = premission;
	fseek(fd, INODE_START + tmp_file_inode.i_ino * INODE_SIZE, SEEK_SET);
	fwrite(&tmp_file_inode, sizeof(inode), 1, fd);
	msg.flag = true;
	msg.content = "OK";
	return msg;
}

/*
*	Change file' owner.
*/
Msg Chown(char* filename,int tt,char *username) {
	Msg msg;
	printf("File or Dir?(0 stands for file, 1 for dir):");
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		msg.content = "Error: Illegal file name.\n";
		msg.flag = false;
		printf("Error: Illegal file name.\n");
		return msg;
	}

	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1;
	inode tmp_file_inode ;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			msg.content = "Not found.\n";
			msg.flag = false;

			printf("Not found.\n");
			return msg;
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode.di_mode == tt);
	msg.content = Access_1_Check(tmp_file_inode, "Change file' owner");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
		

	int i = 0;
	printf("Please input user name:");
	for (i = 0; i < ACCOUNT_NUM; i++) {
		if (strcmp(users.userName[i], username) == 0) break;
	}
	if (i == ACCOUNT_NUM) {
		msg.content="Error user!\n";
		msg.flag = false;
		return msg;
	}
	tmp_file_inode.di_uid = i;
	fseek(fd, INODE_START + tmp_file_inode.i_ino * INODE_SIZE, SEEK_SET);
	fwrite(&tmp_file_inode, sizeof(inode), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
}

/*
*	Change file' group.
*/
Msg Chgrp(char* filename,int tt,int groupid) {
	printf("File or Dir?(0 stands for file, 1 for dir):");
	Msg msg;
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		Msg msg;
		msg.content="Error: Illegal file name.\n";
		return msg;
	}

	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1;
	inode tmp_file_inode ;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			Msg msg;
			msg.content = "Not found.\n";
			printf("Not found.\n");
			return msg;
			
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode.di_mode == tt);
	msg.content = Access_1_Check(tmp_file_inode, "Change file' group");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
		

	printf("Please input group number:");
	int i = 0;
	for (i = 0; i < ACCOUNT_NUM; i++) {
		if (users.groupID[i] == groupid) break;
	}
	if (i == ACCOUNT_NUM) {
		msg.content = "Error user!\n";
		msg.flag = false;
		return msg;
	}
	tmp_file_inode.di_grp = groupid;
	fseek(fd, INODE_START + tmp_file_inode.i_ino * INODE_SIZE, SEEK_SET);
	fwrite(&tmp_file_inode, sizeof(inode), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
}

/*
*	Change password.
*/
Msg Passwd(char * oldpass,char *newpass) {
	Msg msg;
	printf("Please input old password:");
	if (strcmp(users.password[userID], oldpass) == 0) {
		printf("Please input a new password:");
		if (strcmp(newpass, oldpass) == 0) {
			msg.content = "Password doesn't change!\n";
			msg.flag = false;
			return msg;
		}
		else {
			strcpy(users.password[userID], newpass);
			//Write
			fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
			fwrite(&users, sizeof(users), 1, fd);
			msg.content = "Success\n";
			msg.flag = true;
			return msg;
		}
	}
	else {
		msg.content = "Password Error!!!\n";
		msg.flag = false;	
		return msg;
	}
	return msg;
}


string Access_1_Check(inode & inode,string operate)
{
	string tag = "Other Error";
	if (operate == "Read") {
		tag = "OK";
		cout << "Access _1_Check read" << endl;
		if (userID == inode.di_uid)
		{
			if (!(inode.permission & OWN_R)) {
				printf("Read error: Access deny.\n");
				tag = "Read error: Access deny.\n";
				return tag;
			}
		}
		else if (users.groupID[userID] == inode.di_grp) {
			if (!(inode.permission & GRP_R)) {
				printf("Read error: Access deny.\n");
				tag = "Read error: Access deny.\n";
				return tag;
			}
		}
		else {
			if (!(inode.permission & ELSE_R)) {
				printf("Read error: Access deny.\n");
				tag = "Read error: Access deny.\n";
				return tag;
			}
		}
	}
	if (operate == "Write") {
		cout << "Access _1_Check Write" << endl;
		tag = "OK";
		if (userID == inode.di_uid)
		{
			if (!(inode.permission & OWN_W)) {
				tag="Write error: Access deny.\n";
				return tag;
			}
		}
		else if (users.groupID[userID] == inode.di_grp) {
			if (!(inode.permission & GRP_W)) {
				tag="Write error: Access deny.\n";
				return tag;
				
			}
		}
		else {
			if (!(inode.permission & ELSE_W)) {
				tag="Write error: Access deny.\n";
				return tag;
			}
		}
	}
	cout << "Access _1_Check " << operate << endl;

	if (userID == inode.di_uid)
	{
		if (inode.permission & OWN_E != OWN_E) {
			tag = operate + " error: Access deny.\n";
			return tag;
		}
	}
	else if (users.groupID[userID] == inode.di_grp) {
		if (inode.permission & GRP_E != GRP_E) {
			tag = operate + " error: Access deny.\n";
			return tag;
		}
	}
	else {
		if (inode.permission & ELSE_E != ELSE_E) {
			tag = operate + "  error: Access deny.\n";
			return tag;
		}
	}
	tag = "OK";
	return tag;
}

/*
*	Rename file/dir.
*/
Msg Rename(char* filename,char *newname,int tt) 
{
	Msg msg;
	printf("File or Dir?(0 stands for file, 1 for dir):");
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		msg.content = "Error: Illegal file name.\n";
		msg.flag = false;
		printf("Error: Illegal file name.\n");
		return msg;
	}
	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1;
	inode* tmp_file_inode = new inode;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			msg.content = "Not found.\n";
			msg.flag = false;
			return msg;
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode->di_mode == tt);

	printf("Please input new file name:");
	int i = 0,k=0;
	for (; i < DIRECTORY_NUM; i++)
	{
		if (currentDirectory.inodeID[i] == tmp_file_inode->i_ino){
			k = i;
			continue;
		}
		else if (strcmp(currentDirectory.fileName[i], newname) == 0) {
			msg.content = "The currentDirectory already have this name ,please rename again.\n";
			msg.flag = false;
			return msg;
		}
	}
	strcpy(currentDirectory.fileName[k], newname);
	//write
	fseek(fd, DATA_START + tmp_file_inode->di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fwrite(&currentDirectory, sizeof(directory), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
}
/*
*	Link.
*/
bool ln(char* filename)
{
	printf("File or Dir?(0 stands for file, 1 for dir):");
	int tt;
	scanf("%d", &tt);
	//parameter check
	if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
	{
		printf("Error: Illegal file name.\n");
		return false;
	}

	/*
	*	1. Check whether the file exists in current directory.
	*/
	int pos_in_directory = -1;
	inode tmp_file_inode;
	do {
		pos_in_directory++;
		for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		{
			if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			{
				break;
			}
		}
		if (pos_in_directory == DIRECTORY_NUM)
		{
			printf("Not found.\n");
			return false;
		}

		/*
		*	2. Fetch inode and check whether it's a directory.
		*/
		//Fetch inode
		int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
		fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
		fread(&tmp_file_inode, sizeof(inode), 1, fd);
		//Directory check
	} while (tmp_file_inode.di_mode == tt);

	//Access check
	if (Access_1_Check(tmp_file_inode, "Delete") != "OK")
		return false;

	//get absolute path
	char absolute[1024];
	int path_pos = 0;
	printf("Input absolute path(end with '#'):");
	scanf("%s", absolute);
	//get directory inode
	char dirname[14];
	int pos_dir = 0;
	bool root = false;
	inode dir_inode;
	directory cur_dir;
	int i;
	for (i = 0; i < 5; i++)
	{
		dirname[i] = absolute[i];
	}
	dirname[i] = 0;
	if (strcmp("root/", dirname) != 0)
	{
		printf("path error!\n");
		return false;
	}
	fseek(fd, INODE_START, SEEK_SET);
	fread(&dir_inode, sizeof(inode), 1, fd);
	for (int i = 5; absolute[i] != '#'; i++)
	{
		if (absolute[i] == '/')
		{
			dirname[pos_dir++] = 0;
			pos_dir = 0;
			fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
			fread(&cur_dir, sizeof(directory), 1, fd);
			int i;
			for (i = 0; i < DIRECTORY_NUM; i++)
			{
				if (strcmp(cur_dir.fileName[i], dirname) == 0)
				{
					fseek(fd, INODE_START + cur_dir.inodeID[i] * INODE_SIZE, SEEK_SET);
					fread(&dir_inode, sizeof(inode), 1, fd);
					if (dir_inode.di_mode == 0)break;
				}
			}
			if (i == DIRECTORY_NUM)
			{
				printf("path error!\n");
				return false;
			}
		}
		else
			dirname[pos_dir++] = absolute[i];
	}
	//update this directory
	fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fread(&cur_dir, sizeof(directory), 1, fd);
	for (i = 0; i < DIRECTORY_NUM; i++)
	{
		if (strlen(cur_dir.fileName[i]) == 0)
		{
			strcat(cur_dir.fileName[i], filename);
			cur_dir.inodeID[i] = tmp_file_inode.i_ino;
			break;
		}
	}
	if (i == DIRECTORY_NUM)
	{
		printf("No value iterms!\n");
		return false;
	}
	fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fwrite(&cur_dir, sizeof(directory), 1, fd);
	dir_inode.di_number++;
	tmp_file_inode.icount++;
	fseek(fd, INODE_START + tmp_file_inode.i_ino*INODE_SIZE, SEEK_SET);
	fwrite(&tmp_file_inode, sizeof(inode), 1, fd);
	return true;
}

/*
*	File copy.
*/
Msg Copy(char* filename, inode & currentInode,char *aimpath="") {
	//currentInode = OpenFile(filename);
	Msg msg;
	int block_num = currentInode.di_size / BLOCK_SIZE + 1;
	//Read file from data blocks
	char stack[BLOCK_SIZE];
	char str[100000];
	int cnt = 0;
	if (block_num <= NADDR - 2)
	{
		for (int i = 0; i < block_num; i++)
		{
			if (currentInode.di_addr[i] == -1) break;
			fseek(fd, DATA_START + currentInode.di_addr[i] * BLOCK_SIZE, SEEK_SET);
			fread(stack, sizeof(stack), 1, fd);
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				if (stack[j] == '\0') {
					str[cnt] = 0;
					break;
				}
				str[cnt++] = stack[j];
			}
		}
		//int i = 0;
	}
	else if (block_num > NADDR - 2) {
		//direct addressing
		for (int i = 0; i < NADDR - 2; i++)
		{
			fseek(fd, DATA_START + currentInode.di_addr[i] * BLOCK_SIZE, SEEK_SET);
			fread(stack, sizeof(stack), 1, fd);
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				if (stack[j] == '\0') {
					str[cnt] = 0;
					break;
				}
				str[cnt++] = stack[j];
			}
		}

		//first indirect addressing
		unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = { 0 };
		fseek(fd, DATA_START + currentInode.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
		fread(f1, sizeof(f1), 1, fd);
		for (int i = 0; i < block_num - (NADDR - 2); i++) {
			fseek(fd, DATA_START + f1[i] * BLOCK_SIZE, SEEK_SET);
			fread(stack, sizeof(stack), 1, fd);
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				if (stack[j] == '\0') {
					str[cnt] = 0;
					break;
				}
				str[cnt++] = stack[j];
			}
		}
	}
	//Access check
	msg.content = Access_1_Check(currentInode, "Copy");
	if (msg.content != "OK") {
		msg.flag = false;
		return msg;
	}
		

	//get absolute path
	char absolute[1024];
	int path_pos = 0;
	printf("Input absolute path(end with '#'):");
	strcpy(absolute, aimpath);
	//get directory inode
	char dirname[14];
	int pos_dir = 0;
	bool root = false;
	inode dir_inode;
	int i;
	for (i = 0; i < 5; i++)
	{
		dirname[i] = absolute[i];
	}
	dirname[i] = 0;
	if (strcmp("root/", dirname) != 0)
	{
		msg.content="path error!\n";
		msg.flag = false;
		return msg;
	}
	fseek(fd, INODE_START, SEEK_SET);
	fread(&dir_inode, sizeof(inode), 1, fd);
	for (int i = 5; absolute[i] != '#'; i++)
	{
		if (absolute[i] == '/')
		{
			dirname[pos_dir++] = 0;
			pos_dir = 0;
			fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
			fread(&currentDirectory, sizeof(directory), 1, fd);
			int i;
			for (i = 0; i < DIRECTORY_NUM; i++)
			{
				if (strcmp(currentDirectory.fileName[i], dirname) == 0)
				{
					fseek(fd, INODE_START + currentDirectory.inodeID[i] * INODE_SIZE, SEEK_SET);
					fread(&dir_inode, sizeof(inode), 1, fd);
					if (dir_inode.di_mode == 0)break;
				}
			}
			if (i == DIRECTORY_NUM)
			{
				msg.content = "path error!\n";
				msg.flag = false;
				return msg;
			}
		}
		else
			dirname[pos_dir++] = absolute[i];
	}
	//update this directory
	fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fread(&currentDirectory, sizeof(directory), 1, fd);
	for (i = 0; i < DIRECTORY_NUM; i++)
	{
		if (strlen(currentDirectory.fileName[i]) == 0)
		{
			strcat(currentDirectory.fileName[i], filename);
			int new_ino = 0;
			for (; new_ino < INODE_NUM; new_ino++)
			{
				if (inode_bitmap[new_ino] == 0)
				{
					break;
				}
			}
			inode ifile_tmp;
			ifile_tmp.i_ino = new_ino;				//Identification
			ifile_tmp.icount = 0;
			ifile_tmp.di_uid = currentInode.di_uid;
			ifile_tmp.di_grp = currentInode.di_grp;
			ifile_tmp.di_mode = currentInode.di_mode;
			memset(ifile_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
			ifile_tmp.di_size = 0;
			ifile_tmp.permission = currentInode.permission;
			time_t t = time(0);
			strftime(ifile_tmp.time, sizeof(ifile_tmp.time), "%Y/%m/%d %X %A", localtime(&t));
			currentDirectory.inodeID[i] = new_ino;
			Write(ifile_tmp, str);
			//Update bitmaps
			inode_bitmap[new_ino] = 1;
			fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
			fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
			superBlock.s_num_finode--;
			fseek(fd, BLOCK_SIZE, SEEK_SET);
			fwrite(&superBlock, sizeof(filsys), 1, fd);
			break;
		}
	}
	if (i == DIRECTORY_NUM)
	{
		msg.content = "No value iterms!\n";
		msg.flag = false;
		return msg;
	}
	fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
	fwrite(&currentDirectory, sizeof(directory), 1, fd);
	dir_inode.di_number++;
	fseek(fd, INODE_START + currentInode.i_ino*INODE_SIZE, SEEK_SET);
	fwrite(&currentInode, sizeof(inode), 1, fd);
	msg.content = "OK";
	msg.flag = true;
	return msg;
}

int inti_main()
{
	memset(ab_dir, 0, sizeof(ab_dir));
	dir_pointer = 0;
	//Check file system has been formatted or not.
	FILE * fs_test = fopen("fs.han", "r");
	if (fs_test == NULL)
	{
		printf("File system not found... Format file system first...\n\n");
		//第一次初始化
		Format();
	}
	Mount();
	strcpy(ab_dir[dir_pointer], "root");
	dir_pointer++;
	return 0;
}


int  get_current_Inode(char *filename) {
	int pos_in_directory = 0;
	for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
		if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
			break;
	if (pos_in_directory == DIRECTORY_NUM) {
		return -1;
	}
	return currentDirectory.inodeID[pos_in_directory];

}
/*
*	Print all commands help information when 'help' is
*	called or input error occurs.
*/

bool ACCESS_check(string access, const unsigned short permission) {
	if (access == "OWN_R") {
		if (permission &(1 << 8)) return true;
	}
	if (access == "OWN_W") {
		if (permission &(1 << 7)) return true;
	}
	if (access == "OWN_E") {
		if (permission &(1 << 6)) return true;
	}
	if (access == "GRP_R") {
		if (permission &(1 << 5)) return true;
	}
	if (access == "GRP_W") {
		if (permission &(1 << 4)) return true;
	}
	if (access == "GRP_E") {
		if (permission &(1 << 3)) return true;
	}
	if (access == "ELSE_R") {
		if (permission &(1 << 2)) return true;
	}
	if (access == "ELSE_W") {
		if (permission &(1 << 1)) return true;
	}
	if (access == "ELSE_E") {
		if (permission &(1)) return true;
	}
	return false;
}