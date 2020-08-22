#include "FileSystem.h"

FileSystem::FileSystem()
{

}
FileSystem::~FileSystem()
{	
	for(auto ptr : blocks)
		delete ptr;
}

bool FileSystem::isValidFilename(const char* filename)
{	
	/* some characters BANNED for filenames for parsing the disk file */
	std::string chars = BANNED_CHARACTERS;
	const char* invalid_chars = chars.c_str();
	
	for (unsigned int i = 0; i < strlen(invalid_chars); ++i)
		for (unsigned int j = 0; j < strlen(filename); ++j)
			if(filename[j] == invalid_chars[i])
				return false;
	
	return true;
}

void FileSystem::createFileSystem(int block_size,int free_inodes,const std::string& filename)
{
	int fd = open(filename.c_str(),O_CREAT | O_RDWR , S_IRWXU);
	if(fd == -1){
		std::cerr << "File cannot be opened : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
 	}

 	if(block_size < 0){
 		std::cerr << "Block size must be positive as KB" << std::endl;
 		exit(EXIT_FAILURE);
 	}

 	if(free_inodes < 0){
 		std::cerr << "Inode size must be positive as KB" << std::endl;
 		exit(EXIT_FAILURE);
 	}

 	if(free_inodes*INODE_SIZE >= 1024*1024){
 		std::cerr << "This inode size too much. exit." << std::endl;
 		exit(EXIT_FAILURE);
 	}

 	/* initialize super block */
 	this->filename = filename;
 	this->super_block.setBlocksize(block_size);
 	this->super_block.setFreeInodes(free_inodes);
 	this->super_block.setInitInodes(free_inodes);
 	this->super_block.setInodesPerBlock((block_size*1024) / INODE_SIZE);
 	this->super_block.setDirentryPerBlock((block_size*1024) / DIRECTORY_ENTRY_SIZE);

 	/* inode1,contains root directory */
 	Inode inode1;
 	Metadata mdata;	
 	mdata.updateModftime();
 	mdata.setFilename("root");
 	mdata.setSize(block_size*1024);
 	inode1.setMetadata(mdata);
 	
 	/* initialize direct pointers for root inode */
 	int direct_ptrs[10];
 	for (int i = 0; i < 10; ++i)
 		direct_ptrs[i] = -1;
 	direct_ptrs[0] = 1; // point to root directory

 	std::ostringstream out;
 	out <<  INODE_BLOCK <<  DIRECTORY_BLOCK;
 	blockData = out.str();
 	
 	inode1.setDirectPointers(direct_ptrs);
 	
 	InodeBlock* ind = new InodeBlock(super_block.getInodesPerBlock());
 	ind->fillNextInode(inode1);
 	this->decreaseFreeInode();
 	
 	this->blocks.push_back(ind);
 	this->blocks.push_back(new DirectoryBlock(super_block.getDirentryPerBlock()));
 	close(fd);
 	this->updateDisk();

}
void FileSystem::createFileSystem(int block_size,int free_inodes,const char* filename)
{
	std::string s = filename;
	this->createFileSystem(block_size,free_inodes,s);
}

void FileSystem::initSystem(const char* filename)
{
	std::string s = filename;
	this->initSystem(s);
}
void FileSystem::initSystem(const std::string& filename)
{
	int fd = open(filename.c_str(), O_RDWR, S_IRWXU);
	if(fd == -1){
		std::cerr << "File cannot be opened : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
 	}
 	
 	this->filename = filename;

 	size_t file_size = lseek(fd,0,SEEK_END);
 	char* contents = new char[file_size + 1];
 	char* bckp_content = contents;

 	lseek(fd,0,SEEK_SET);
 	read(fd,contents,file_size); // err check

 	int total_size = 0;

 	/**
 	* get first 3 lines to construct file system.
 	* 1st line contains super block.
 	* 2nd line contains blockData that represents the block types per their numbers.
 	* 3nd line contains all blocks seperated with ~
 	*/
 	char* tokenize = strtok_r(contents,"\n",&contents);
 	total_size += strlen(tokenize);

 	this->super_block.initWithToken(tokenize);
 	tokenize = strtok_r(contents,"\n",&contents);

 	total_size += strlen(tokenize);
 	this->blockData = tokenize;

 	char* tokenize2 = strtok_r(contents,"\n",&contents);
 	total_size += strlen(tokenize2);

 	for (unsigned int i = 0; i < blockData.size(); ++i)
 	{
 		tokenize = strtok_r(tokenize2,"~",&tokenize2);
 		if(blockData[i] == '0'){
 			InodeBlock* iblock = new InodeBlock(super_block.getInodesPerBlock(),tokenize);
 			this->blocks.push_back(iblock);
 			this->inode_blocks.push_back(iblock);
 		}
 		else if(blockData[i] == '1')
 			this->blocks.push_back(new DirectoryBlock(super_block.getDirentryPerBlock(),tokenize));
 		else if(blockData[i] == '2')
 			this->blocks.push_back(new DataBlock(tokenize));
 		else if(blockData[i] == '3')
 			this->blocks.push_back(new PointerBlock(tokenize));	
 	}

 	this->used_disk = total_size + 2;
 	close(fd);
 	delete[] bckp_content;
}


void FileSystem::updateDisk()
{	
	int disk_fd = open(this->filename.c_str(),O_RDWR | O_TRUNC);

	int total_size = 0;

	// write superblock
	total_size += write(disk_fd,super_block.toString().c_str(),super_block.toString().size());

	// write block data
	total_size += write(disk_fd,blockData.c_str(),blockData.size());
	total_size += write(disk_fd,"\n",1);

	for (unsigned int i = 0; i < blocks.size(); ++i)
	{
		total_size += write(disk_fd,blocks[i]->toString().c_str(),blocks[i]->toString().size());
		total_size += write(disk_fd,"~",1);
	}

	this->used_disk = total_size;
	off_t size = 1024*1024;
	ftruncate(disk_fd,size);
	close(disk_fd);
}

void FileSystem::decreaseFreeInode()
{
	this->super_block.setFreeInodes(this->super_block.getFreeInodes()-1);
}

void FileSystem::sys_mkdir(const char* dirname)
{

	if(!this->isValidFilename(dirname)){
		std::string banned_chars = BANNED_CHARACTERS;
		std::cerr << "Filename MUST NOT contain these characters : \"" << banned_chars << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(this->super_block.getFreeInodes() < 1){
		std::cerr << "Storage is not enough ! exit..\n";
		exit(EXIT_FAILURE);
	}

	char* cpy_dirname = new char[strlen(dirname) + 1];
	char* bckp_dirname = cpy_dirname;
	strcpy(cpy_dirname,dirname);
	
	std::vector<char*> path_tokens;

	char* tokenize = strtok_r(cpy_dirname,"/",&cpy_dirname);
	while(tokenize != NULL)
	{
		path_tokens.push_back(tokenize);
		tokenize = strtok_r(cpy_dirname,"/",&cpy_dirname);
	}

	if(path_tokens.size() == 0){
		std::cerr << "Invalid path given to mkdir" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* go to the target path directory */
	int inode;
	DirectoryBlock* current_dir = (DirectoryBlock*)this->blocks[1];
	for (unsigned int i = 0; i < path_tokens.size() - 1; ++i)
	{
		inode = current_dir->getInodeByFilename(path_tokens[i]);
		if(inode == -1){
			std::cerr << "Invalid path given to mkdir" << std::endl;
			delete[] bckp_dirname;
			exit(EXIT_FAILURE);
		}
		
		/* find inode index */
		int nth_block = inode / (this->super_block.getInodesPerBlock());
		int nth_inode = inode % (this->super_block.getInodesPerBlock());
		
		Inode dir_inode = this->inode_blocks[nth_block]->getNthInode(nth_inode);
		current_dir = dynamic_cast<DirectoryBlock*>(this->blocks[dir_inode.getNthPointer(0)]);

		/* check if it is really directory */
		if(current_dir == nullptr){
			std::cerr << "Given path is not valid path (is not directory) !" << std::endl;
			delete[] bckp_dirname;
			exit(EXIT_FAILURE);
		}

	}

	/* error check */
	if(current_dir->containsDirectory(path_tokens[path_tokens.size()-1])){
		std::cerr << "Directory already exist.!" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* find available inode block for new directory */
	bool directory_added = false;
	for (unsigned int i = 0; i < this->blocks.size(); ++i)
	{
		InodeBlock* iblock = dynamic_cast<InodeBlock*>(this->blocks[i]);

		if(iblock != nullptr){
			if(!iblock->isFull()){
				Inode inode1;
			 	Metadata mdata;	
			 	mdata.updateModftime();
			 	mdata.setFilename(path_tokens[path_tokens.size()-1]);
			 	mdata.setSize(this->super_block.getBlocksize()*1024);
			 	inode1.setMetadata(mdata);

			 	// initialize pointers to data blocks
			 	int direct_ptrs[10];
			 	for (int i = 0; i < 10; ++i)
			 		direct_ptrs[i] = -1;
			 	direct_ptrs[0] = this->blocks.size();

			 	inode1.setDirectPointers(direct_ptrs); 
				iblock->fillNextInode(inode1);
				directory_added = true;
				break;
			}
		}
	}

	/* If all inode blocks are full, create new one */
	if(!directory_added){
		InodeBlock* newBlock = new InodeBlock(super_block.getInodesPerBlock());
		Inode newInode;
		Metadata mdata;
		mdata.updateModftime();
		mdata.setFilename(path_tokens[path_tokens.size()-1]);
		mdata.setSize(this->super_block.getBlocksize()*1024);
		newInode.setMetadata(mdata);
		
		// initialize pointers to data blocks
		int direct_ptrs[10];
		for (int i = 0; i < 10; ++i)
			direct_ptrs[i] = -1;
		direct_ptrs[0] = this->blocks.size() + 1;
		newInode.setDirectPointers(direct_ptrs);
		newBlock->fillNextInode(newInode);
		this->blocks.push_back(newBlock);
		this->inode_blocks.push_back(newBlock);
		std::string bdata = "0";
		this->blockData = this->blockData + bdata;
	}
	
	/* get inode number */
	int inode_num = super_block.getInitInodes() - super_block.getFreeInodes();

	/* add new directory entry to directory block, if it fails, exit */
	DirectoryEntry ent(inode_num,path_tokens[path_tokens.size()-1]);
	if(!current_dir->fillNextDirectory(ent)){
		std::cerr << "Directory is full." << std::endl;
		exit(EXIT_FAILURE);
	}

	/* add new directory block */
	this->blocks.push_back(new DirectoryBlock((this->super_block.getBlocksize()*1024) / DIRECTORY_ENTRY_SIZE));

	std::string bdata = "1";
	this->blockData = this->blockData + bdata;

	/* decrease free inodes and update the disk file */
	this->decreaseFreeInode();
	this->updateDisk();

	delete[] bckp_dirname;
}

void FileSystem::sys_mkdir(const std::string& dirname)
{
	this->sys_mkdir(dirname.c_str());
}

void FileSystem::sys_write(const char* filename,const char* source_file)
{

	/* check validity of filename */
	if(!this->isValidFilename(filename)){
		std::string banned_chars = BANNED_CHARACTERS;
		std::cerr << "Filename MUST NOT contain these characters : \"" << banned_chars << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	int local_block_size = this->super_block.getBlocksize()*1024;
	//local_block_size = 2; // FOR DEBUG

	// read source file (linux file)
	int fd = open(source_file,O_RDONLY);
	if(fd == -1){
		std::cerr << "File cannot opened !" << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	/* get source file size */
	size_t srcfile_size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);

	if(srcfile_size <= 0){
		std::string strsrc = source_file;
		std::cerr << "Source file " << source_file << " is not valid..exit." << std::endl;
		exit(EXIT_FAILURE);
	}

	/* block need for storing the file */
	int inode_addr_need =  srcfile_size / local_block_size;
	if(srcfile_size % local_block_size != 0)
		++inode_addr_need;

	if(used_disk + srcfile_size >= 1024*1024 || inode_addr_need > this->super_block.getFreeInodes()){
		std::cerr << "Given file is too big for store" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	/* Parse the given path into tokens */
	char* cpy_filename = new char[strlen(filename) + 1];
	char* bckp_filename = cpy_filename;
	strcpy(cpy_filename,filename);
	
	std::vector<char*> path_tokens;
	char* tokenize = strtok_r(cpy_filename,"/",&cpy_filename);
	while(tokenize != NULL)
	{
		path_tokens.push_back(tokenize);
		tokenize = strtok_r(cpy_filename,"/",&cpy_filename);
	}
	
	if(path_tokens.size() == 0){
		std::cerr << "Invalid path given to write" << std::endl;
		exit(EXIT_FAILURE);
	}


	/* Go to the target directory.. */
	/* Iterate all over the blocks. */

	int inode;
	DirectoryBlock* current_dir = dynamic_cast<DirectoryBlock*>(this->blocks[1]);
	if(current_dir == nullptr){
		std::cerr << "FATAL ERROR. EXIT" << std::endl;
		exit(EXIT_FAILURE);
	}

	for (unsigned int i = 0; i < path_tokens.size() - 1; ++i)
	{
		inode = current_dir->getInodeByFilename(path_tokens[i]);

		if(inode == -1){
			std::cerr << "Invalid path given to write" << std::endl;
			delete[] bckp_filename;
			exit(EXIT_FAILURE);
		}

		/* find inode index */
		int nth_block = inode / (this->super_block.getInodesPerBlock());
		int nth_inode = inode % (this->super_block.getInodesPerBlock());

		Inode dir_inode = this->inode_blocks[nth_block]->getNthInode(nth_inode);
		
		current_dir = dynamic_cast<DirectoryBlock*>(this->blocks[dir_inode.getNthPointer(0)]);

		/* check if it is really directory */
		if(current_dir == nullptr){
			std::cerr << "Given path is not valid path (is not directory) !" << std::endl;
			delete[] bckp_filename;
			exit(EXIT_FAILURE);
		}
	}
	if(current_dir->containsDirectory(path_tokens[path_tokens.size()-1])){
		std::cerr << "File or directory already exist.!" << std::endl;
		exit(EXIT_FAILURE);
	}


	/** 
	* initialize inode for file, direct pointers,single indirect pointer if necessary.
	* initialize metadata and etc.
	*/
	bool file_added = false;
	for (unsigned int i = 0; i < this->blocks.size(); ++i)
	{
		InodeBlock* iblock = dynamic_cast<InodeBlock*>(this->blocks[i]);
		if(iblock != nullptr){
			if(!iblock->isFull()){
				Inode inode1;
			 	Metadata mdata;	
			 	mdata.updateModftime();
			 	mdata.setFilename(path_tokens[path_tokens.size()-1]);
			 	mdata.setSize(srcfile_size);
			 	inode1.setMetadata(mdata);

			 	// initialize pointers to data blocks
			 	int direct_ptrs[10];
			 	for (int i = 0; i < 10; ++i)
			 		direct_ptrs[i] = -1;
			 	
			 	int last_block_addr=0;
			 	if(inode_addr_need <= 10)
			 	{
				 	for (int i = 0; i < inode_addr_need; ++i){
				 		last_block_addr = this->blocks.size() + i;
				 		direct_ptrs[i] = last_block_addr;
				 	}
			 	}
			 	else	
			 	{	
			 		/* If direct pointers not enough, use single indirection */

			 		for (int i = 0; i < 10; ++i){
				 		last_block_addr = this->blocks.size() + i + 1;
				 		direct_ptrs[i] = last_block_addr;
				 	}
				 	inode1.setSingleIndirect(this->blocks.size());
				 	last_block_addr++;

				 	PointerBlock* ptrblock = new PointerBlock();

				 	int remaining_ptrs = inode_addr_need - 10;
				 	for (int i = 0; i < remaining_ptrs; ++i)
				 		ptrblock->push_pointer(i+last_block_addr);

				 	this->blocks.push_back(ptrblock);
				 	std::string bdata = "3";
				 	this->blockData += bdata;	
			 	}
			 	
			 	inode1.setDirectPointers(direct_ptrs); 
				iblock->fillNextInode(inode1);
				file_added = true;
				break;
			}
		}
	}

	/* If all inode blocks are full, create new inode block. */

	if(!file_added){
		InodeBlock* newBlock = new InodeBlock(super_block.getInodesPerBlock());
		Inode inode1;
		Metadata mdata;
		mdata.updateModftime();
		mdata.setFilename(path_tokens[path_tokens.size()-1]);
		mdata.setSize(srcfile_size);
		inode1.setMetadata(mdata);
		
		// initialize pointers to data blocks
	 	int direct_ptrs[10];
	 	for (int i = 0; i < 10; ++i)
	 		direct_ptrs[i] = -1;
	 	
	 	int last_block_addr=0;
	 	if(inode_addr_need <= 10)
	 	{
		 	for (int i = 0; i < inode_addr_need; ++i){
		 		last_block_addr = this->blocks.size() + i + 1;
		 		direct_ptrs[i] = last_block_addr;
		 	}
	 	}
	 	else	
	 	{	
	 		/* If direct pointers not enough, use single indirection */

	 		for (int i = 0; i < 10; ++i){
		 		last_block_addr = this->blocks.size() + i + 2;
		 		direct_ptrs[i] = last_block_addr;
		 	}
		 	inode1.setSingleIndirect(this->blocks.size());
		 	last_block_addr++;

		 	PointerBlock* ptrblock = new PointerBlock();

		 	int remaining_ptrs = inode_addr_need - 10;
		 	for (int i = 0; i < remaining_ptrs; ++i)
		 		ptrblock->push_pointer(i+last_block_addr);

		 	this->blocks.push_back(ptrblock);
		 	std::string bdata = "3";
		 	this->blockData += bdata;	
	 	}
	 	
	 	inode1.setDirectPointers(direct_ptrs); 

		newBlock->fillNextInode(inode1);
		this->blocks.push_back(newBlock);
		this->inode_blocks.push_back(newBlock);
		std::string bdata = "0";
		this->blockData = this->blockData + bdata;
	}

	
	/* add new directory entry */
	int inode_num = super_block.getInitInodes() - super_block.getFreeInodes();
	DirectoryEntry ent(inode_num,path_tokens[path_tokens.size()-1]);
	current_dir->fillNextDirectory(ent);


	/* read the source file and write data to data blocks */

	char* buffer = new char[local_block_size + 1];
	std::string bdata = "2";
	int total_readed = 0;
	for (int i = 0; i < inode_addr_need-1; ++i)
	{	
		total_readed += read(fd,buffer,local_block_size);
		buffer[local_block_size] = '\0';
		DataBlock* nblock = new DataBlock();
		nblock->setContent(buffer);
		this->blocks.push_back(nblock);
		this->blockData = this->blockData + bdata;
	}

	if(srcfile_size - total_readed > 0){
		read(fd,buffer,srcfile_size - total_readed);
		buffer[srcfile_size - total_readed] = '\0';
		DataBlock* nblock = new DataBlock();
		nblock->setContent(buffer);
		this->blocks.push_back(nblock);
		this->blockData = this->blockData + bdata;
	}

	/* decrease free inodes and update the disk file */
	this->decreaseFreeInode();
	this->updateDisk();

	close(fd);
	delete[] buffer;
	delete[] bckp_filename;

}
void FileSystem::sys_write(const std::string& filename,const char* source_file)
{
	this->sys_write(filename.c_str(),source_file);
}

void FileSystem::sys_list(const char* path)const
{
	char* cpy_pathname = new char[strlen(path) + 1];
	char* bckp_pathname = cpy_pathname;
	strcpy(cpy_pathname,path);
	
	/* get path tokens */
	std::vector<char*> path_tokens;
	char* tokenize = strtok_r(cpy_pathname,"/",&cpy_pathname);
	while(tokenize != NULL)
	{
		path_tokens.push_back(tokenize);
		tokenize = strtok_r(cpy_pathname,"/",&cpy_pathname);
	}

	/* go to target path, directory */
	int inode;
	DirectoryBlock* current_dir = (DirectoryBlock*)this->blocks[1];
	for (unsigned int i = 0; i < path_tokens.size(); ++i)
	{
		inode = current_dir->getInodeByFilename(path_tokens[i]);
		if(inode == -1){
			std::cerr << "Invalid path given to list" << std::endl;
			delete[] bckp_pathname;
			exit(EXIT_FAILURE);
		}
		
		/* find inode index */
		int nth_block = inode / (this->super_block.getInodesPerBlock());
		int nth_inode = inode % (this->super_block.getInodesPerBlock());
		
		Inode dir_inode = this->inode_blocks[nth_block]->getNthInode(nth_inode);
		current_dir = dynamic_cast<DirectoryBlock*>(this->blocks[dir_inode.getNthPointer(0)]);

		/* chech if it is really directory */
		if(current_dir == nullptr){
			std::cerr << "Given path is not valid path (is not directory) !" << std::endl;
			delete[] bckp_pathname;
			exit(EXIT_FAILURE);
		}
	}

	std::vector<DirectoryEntry> contents = current_dir->getContents();

	/* iterate over the all directory entries */
	for(auto dir_entry : contents)
	{
		int inode = dir_entry.getInode();

		int nth_block = inode / (this->super_block.getInodesPerBlock());
		int nth_inode = inode % (this->super_block.getInodesPerBlock());
		
		Inode dir_inode = this->inode_blocks[nth_block]->getNthInode(nth_inode);

		int ptr = dir_inode.getNthPointer(0);

		std::string modftime = dir_inode.getMetadata().getLastModftime();
		std::string entry_name = dir_inode.getMetadata().getFilename();

		/* f means file, d means directory for readability */
		if(blockData[ptr] == '2')
			printf("<f> %-5d %-5s %-5s\n", dir_inode.getMetadata().getSize(),modftime.c_str(),entry_name.c_str());
		else if(blockData[ptr] == '1')
			printf("<d> %-5d %-5s %-5s\n", dir_inode.getMetadata().getSize(),modftime.c_str(),entry_name.c_str());

	}

	delete[] bckp_pathname;
}
void FileSystem::sys_list(const std::string& path)const
{
	this->sys_list(path.c_str());
}

void FileSystem::sys_read(const char* filename,const char* target_file)const
{
	int fd = open(target_file,O_CREAT | O_RDWR | O_TRUNC,S_IRWXU);
	if(fd == -1){
		std::cerr << "File cannot opened !" << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	char* cpy_filename = new char[strlen(filename)+1];
	char* bckp_filename = cpy_filename;
	strcpy(cpy_filename,filename);
	
	/* get path tokens */
	std::vector<char*> path_tokens;
	char* tokenize = strtok_r(cpy_filename,"/",&cpy_filename);
	while(tokenize != NULL)
	{
		path_tokens.push_back(tokenize);
		tokenize = strtok_r(cpy_filename,"/",&cpy_filename);
	}

	if(path_tokens.size() == 0){
		std::cerr << "Invalid path given to read" << std::endl;
		delete[] bckp_filename;
		exit(EXIT_FAILURE);
	}

	/* Search target path, find the directory that contains the file */
	int inode;
	DirectoryBlock* current_dir = (DirectoryBlock*)this->blocks[1];
	for (unsigned int i = 0; i < path_tokens.size() - 1; ++i)
	{		
		inode = current_dir->getInodeByFilename(path_tokens[i]);

		if(inode == -1){
			std::cerr << "Invalid path given to read" << std::endl;
			delete[] bckp_filename;
			exit(EXIT_FAILURE);
		}

		int nth_block = inode / (this->super_block.getInodesPerBlock());
		int nth_inode = inode % (this->super_block.getInodesPerBlock());

		Inode dir_inode = this->inode_blocks[nth_block]->getNthInode(nth_inode);
		
		current_dir = dynamic_cast<DirectoryBlock*>(this->blocks[dir_inode.getNthPointer(0)]);

		/* check if its really directory */
		if(current_dir == nullptr){
			std::cerr << "Given path is not valid path (is not directory) !" << std::endl;
			delete[] bckp_filename;
			exit(EXIT_FAILURE);
		}
	}

	/* check file is exist or not */
	if(!(current_dir->containsDirectory(path_tokens[path_tokens.size()-1]))){
		std::cerr << "File or directory not exist.!" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* get inode number of the searched file from directory entry */
	inode = current_dir->getInodeByFilename(path_tokens[path_tokens.size()-1]);

	/* find inode index */
	int nth_block = inode / (this->super_block.getInodesPerBlock());
	int nth_inode = inode % (this->super_block.getInodesPerBlock());

	Inode dir_inode = this->inode_blocks[nth_block]->getNthInode(nth_inode);

	DataBlock* current_file = dynamic_cast<DataBlock*>(this->blocks[dir_inode.getNthPointer(0)]);
	if(current_file == nullptr){
		std::cerr << "Given filename is not a regular file !" << std::endl;
		delete[] bckp_filename;
		exit(EXIT_FAILURE);
	}

	/* get direct pointer block adresses */
	std::vector<int> occupied_areas;
	for (int i = 0; i < 10; ++i)
	{
		if(dir_inode.getNthPointer(i) == -1)
			break;
		occupied_areas.push_back(dir_inode.getNthPointer(i));
	}

	/* get indirect pointer block adresses */
	if(dir_inode.getSingleIndirect() != -1){

		PointerBlock* ptrblock = dynamic_cast<PointerBlock*>(this->blocks[dir_inode.getSingleIndirect()]);
		if(ptrblock == nullptr){
			std::cerr << "FATAL ERROR occured. exiting." << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < ptrblock->getSize(); ++i)
			occupied_areas.push_back(ptrblock->getNthPointer(i));
	}

	/* get file contents from data blocks */
	std::vector<std::string> file_contents;
	for(auto v : occupied_areas)
	{
		DataBlock* dblock = dynamic_cast<DataBlock*>(this->blocks[v]);
		if(dblock == nullptr){
			std::cerr << "FATAL ERROR occured. exiting." << std::endl;
			exit(EXIT_FAILURE);
		}
		file_contents.push_back(dblock->getContent());
	}

	/* write file contents into target file */
	for(auto content : file_contents)
		write(fd,content.c_str(),content.size()); // err check
	

	close(fd);
	delete[] bckp_filename;

}
void FileSystem::sys_read(const std::string filename,const char* target_file)const
{
	this->sys_read(filename.c_str(),target_file);
}

void FileSystem::sys_dumpe2fs()const
{
	size_t disk_used_size = this->used_disk;

	unsigned int used_blocks = this->blockData.size();

	int init_block_size = 1024 / this->super_block.getBlocksize();

	std::cout << "Disk usage : " << disk_used_size << " bytes." <<  std::endl;
	std::cout << "# of used blocks : " << used_blocks << std::endl;
	std::cout << "# of free blocks : " << init_block_size - used_blocks << std::endl;
	std::cout << "# of free inodes : " << this->super_block.getFreeInodes() << std::endl;

	int used_inodes = this->super_block.getInitInodes() - this->super_block.getFreeInodes();

	size_t total_size = 0;

	std::cout << "\nOccupied inodes : " << std::endl <<"format : (Number,inode)\n" << "---------------------------" << std::endl;
	int ind_number = 0;
	for(InodeBlock* iblock : this->inode_blocks)
	{
		std::vector<Inode> inodes = iblock->getAllInodes();
		for(Inode ind : inodes){
			total_size += ind.getMetadata().getSize();
			std::cout << ind_number++ << " " << ind.getMetadata().toString() << std::endl;
		}
	}

	std::cout <<  "---------------------------\n" << std::endl;

	std::cout << "Available space : " << 1024*1024 - total_size << " bytes." << std::endl;
	std::cout << "# of directories : " << DirectoryBlock::getTotalDirectories() << std::endl;
	std::cout << "# of files : " << used_inodes - DirectoryBlock::getTotalDirectories() << std::endl;
}

void FileSystem::sys_fsck()const
{
	std::cout << "Used Blocks : ";

	for (unsigned int i = 0; i < blockData.size(); ++i)
		std::cout << i << " ";
	std::cout << std::endl;


	int num_block = (1024 / this->super_block.getBlocksize()) - 2;
	std::cout << "Free blocks : (" << blockData.size() << " to " << num_block << ")" << std::endl;

	int ind_number = 0;
	std::cout << "Used inodes : ";
	for(InodeBlock* iblock : this->inode_blocks)
	{
		std::vector<Inode> inodes = iblock->getAllInodes();
		for (unsigned int i = 0; i < inodes.size(); ++i)
			std::cout << ind_number++ << " ";	
	}
	std::cout << std::endl;

	std::cout << "Free inodes : (" << ind_number << " to " << this->super_block.getFreeInodes() << ")";
	std::cout << std::endl;
}