#include "Metadata.h"

Metadata::Metadata()
{	
	strcpy(this->filename, "null");
	this->size = 0;
	strcpy(this->last_modftime,"null");
}

// error check
Metadata::Metadata(const std::string& token)
{
	this->initWithToken(token);
}
Metadata::Metadata(const char* token)
{
	this->initWithToken(token);
}
void Metadata::initWithToken(const char* token)
{
	char *str = new char[80];
	char* bckupstr = str;
	strcpy(str,token);
	int len = strlen(str);

	str = &str[1];
	str[len-2] = '\0';

	char tokenizer[80];
	strcpy(tokenizer,str);
	delete[] bckupstr;

	char* tokenize = strtok(tokenizer,",");
	if(strlen(tokenize) > 14){
		std::cerr << "Max filename size is 14 ! Exit" << std::endl;
		exit(EXIT_FAILURE);
	}
	strcpy(this->filename,tokenize);
	tokenize = strtok(NULL,",");
	this->size = atoi(tokenize);
	tokenize = strtok(NULL,",");
	strcpy(this->last_modftime,tokenize);
}
void Metadata::initWithToken(const std::string& token)
{
	char *str = new char[80];
	char* bckupstr = str;
	strcpy(str,token.c_str());
	int len = strlen(str);

	str = &str[1];
	str[len-2] = '\0';

	char tokenizer[80];
	strcpy(tokenizer,str);
	delete[] bckupstr;

	char* tokenize = strtok(tokenizer,",");
	strcpy(this->filename,tokenize);
	tokenize = strtok(NULL,",");
	this->size = atoi(tokenize);
	tokenize = strtok(NULL,",");
	strcpy(this->last_modftime,tokenize);
}
void Metadata::setFilename(const std::string& fname)
{	
	strcpy(this->filename,fname.c_str());
}
void Metadata::setFilename(const char* fname)
{
	strcpy(this->filename,fname);
}
void Metadata::setSize(int size)
{
	this->size = size;
}
void Metadata::updateModftime()
{
	this->currentDateTime();
}
std::string Metadata::getFilename()const
{
	return this->filename;
}
int Metadata::getSize()const
{
	return this->size;
}
std::string Metadata::getLastModftime()const
{
	return this->last_modftime;
}

std::string Metadata::toString()const
{
	std::ostringstream out;
	out << "<" << filename << "," << size << "," << this->last_modftime << ">";
	return out.str();
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
void Metadata::currentDateTime(){
    time_t     now = time(0);
    struct tm  tstruct;
    tstruct = *localtime(&now);
    strftime(last_modftime, sizeof(last_modftime), "%Y-%m-%d.%X", &tstruct);
}