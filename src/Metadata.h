#ifndef METADATA_H
#define METADATA_H

#include <string.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <time.h>

class Metadata
{
	public:
		Metadata();
		Metadata(const std::string& token);
		Metadata(const char* token);
		std::string getFilename()const;
		int getSize()const;
		std::string getLastModftime()const;
		std::string toString()const;
		void setFilename(const std::string& fname);
		void setFilename(const char* fname);
		void setSize(int size);
		void updateModftime();
		void initWithToken(const char* token);
		void initWithToken(const std::string& token);
	private:
		void currentDateTime();
		char filename[15];
		char last_modftime[30];
		int size;
};


#endif