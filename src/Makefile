all: makeFileSystem
	@echo "\n> part2 created : makeFileSystem"

makeFileSystem : makeFileSystem.o Metadata.o Inode.o Block.o InodeBlock.o DirectoryEntry.o DirectoryBlock.o SuperBlock.o FileSystem.o DataBlock.o PointerBlock.o
	g++ -Wall makeFileSystem.o Metadata.o Inode.o Block.o InodeBlock.o DirectoryEntry.o DirectoryBlock.o SuperBlock.o DataBlock.o PointerBlock.o FileSystem.o -o makeFileSystem

makeFileSystem.o : makeFileSystem.cpp
	g++ -c -Wall makeFileSystem.cpp -std=c++11 -g

FileSystem.o : FileSystem.cpp FileSystem.h
	g++ -c -Wall FileSystem.cpp -std=c++11 -g

PointerBlock.o : PointerBlock.cpp PointerBlock.h
	g++ -c -Wall PointerBlock.cpp -std=c++11 -g

DataBlock.o : DataBlock.cpp DataBlock.h
	g++ -c -Wall DataBlock.cpp -std=c++11 -g

SuperBlock.o : SuperBlock.cpp SuperBlock.h
	g++ -c -Wall SuperBlock.cpp -std=c++11 -g

DirectoryBlock.o : DirectoryBlock.cpp DirectoryBlock.h
	g++ -c -Wall DirectoryBlock.cpp -std=c++11 -g

InodeBlock.o : InodeBlock.cpp InodeBlock.h
	g++ -c -Wall InodeBlock.cpp -std=c++11 -g 

Block.o : Block.cpp Block.h
	g++ -c -Wall Block.cpp -std=c++11 -g 

DirectoryEntry.o : DirectoryEntry.cpp DirectoryEntry.h
	g++ -c -Wall DirectoryEntry.cpp -std=c++11 -g

Inode.o: Inode.cpp Inode.h
	g++ -c -Wall Inode.cpp -std=c++11 -g 

Metadata.o : Metadata.cpp Metadata.h
	g++ -c -Wall Metadata.cpp -std=c++11 -g

clean:
	rm -rf *o
