

#include "Format.h"
#include "Exception_Handling.h"
using namespace Kozy;



const Format Kozy::getFormatByFileName(const char* fileName){
	const char* iter{fileName};

	//the following two loops could have been done using the STL, but 
	//1) it is actually faster to just write simple loops than to look for the right header and function
	//2) c functions that deal with c-Strings are not always safe and one has to carefully read about their side effects and how they are intended to be used
	//3) it is fun to write for loops
	for(;*iter!='\0';++iter) ; //get the last element

	for(--iter;*iter!='.'&& *iter!='\0';--iter) ; //get the last '.'

	if(*iter=='\0'){
		throw Invalid_Argument_obj((
			std::string(fileName) + " does not have a dot in its name.").c_str(), 
			"Make sure your pictures name ends correctly with its type.\n"
			"For example: YOUR_PICTURE_NAME.png"
		);
		return Format(BLANKFORMAT);
	}

	return Format(++iter);	// iter should be the name of the data type of the file. a pointer to a char can be converted to a c-string and std::string, if it ends with the nul symbol \0
							// It is okay to pass a c-style pointer. Format constructor copies the c-string.
}


bool Format::isValid() const noexcept{
	return name!=NO_FORMAT;	// in C or using C-Style Strings, we would do the following: strcmp(ending,NO_FORMAT)==0
}