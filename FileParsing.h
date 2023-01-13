#ifndef FILEPARSING_H
#define FILEPARSING_H

#include <climits> // used for CHAR_BIT Macros
#include <cstdint> // used for size_t



#include "Exception_Handling.h"

namespace Kozy{


class Picture;
class PictureProject;
struct BitmapInterface;
struct Format;

namespace FileParsing{



/*
* right now it only knows bmp file format
*
*changes Picture::State if failed. Does only overwrite Picture if no error occurs.
*
*
* throws:
* invalid_argument -- file not found
* invalid_argument -- file unknown Format
* exception
*/
Picture* load_Picture(Picture* , const char* fileName);
/*NOT IMPLEMENTED YET*/
PictureProject* load_PictureProject(PictureProject* , const char* fileName);

/*
returns nullptr if failed
*/
BitmapInterface* load_Picture_BMP(BitmapInterface* , const char* fileName);


/*
choose a fileName without adding its type at the end of the name.
For more information, see the function it delegates to.

Does not change/overwrite any existing file

enables Picture::State::Failed_Operation, if an error occures. 
*/
const Picture* save_Picture(const Picture* , const char* fileName, const Format& );
const Picture* save_Picture(const Picture* , const char* fileName);

/*
>>>>>> OVERWRITES EXISTING FILES!!! <<<<<<<<<<

creates a new bmp file. If the fileName is already used _new is appended
appends .bmp to the fileName

Example_1:
fileName == folder/a.bmp
folder is empty
Result:

a.bmp.bmp

Example_2:
fileName == folder/a
folder has a.bmp
Result:

a_new.bmp


most important checks are done in save_picture. 
Only use delegating functions like these, if you are absolutely sure that your objects are valid!
Does overwrite existing files!!

Saves the picture as a BMP of Version 3. Uncompressed, 24 Bit channel
*/
const BitmapInterface* save_Picture_BMP(const BitmapInterface*, const char* fileName);

//BitmapInterface* save_PictureProject_BMP(Picture *pic, const char* fileName);

/*
Although modern computers use 8 Bits to denote a byte, there are architectures and OS that do not. -Graphical Calculators come to mind.
This library does not support these architectures yet

byte_8 guarantees that it can be converted to unsigned char.
*/

#if defined CHAR_BIT && CHAR_BIT == 8   // CHAR_BIT can be e.g.: 6 or 32, if it is defined at all. It is not defined, if there is no clear representation of that on the system
typedef unsigned char byte_8;


#else
    
#ifdef UINT8_C

typedef uint8_t byte_8;


#else

// byte is not 8 bit


#endif
#endif


/*
 assumes byte_8 is 8 bit
 for example getByte_16_Val assumes that the user passes an array of 2 byte_8 type memories

 Basics on binary calculations and storage (for readability, the examples are with 4-Bit bytes):

 A byte alone is read and written from right to left. So 1101 == 2^0 + 2^2 + 2^3 == 1 + 4 + 8 == 13

 there are two orders to read several bytes together:
    little Endian. _l_E       
    big Endian. _b_E

Visualization:
    Data in memory: 
        0000 0001 0010 0011 
        little Endian: 0011 0010 0001 0000 == 2^4 + 2^9 + 2^12 + 2^13 == 12.816
        big Endian: 0000 0001 0010 0011 == 2^0 + 2^1 + 2^5 + 2^8 == 291

  https://de.m.wikipedia.org/wiki/Byte-Reihenfolge I read the German version, but surely the english variant is good too


Endianness is only important when handling data. It does not apply to values or to be more precise, bit operations in your code. 
So for example: 
	int i =4; //0b4 == 0b0000'0000 0000'0100
	int l = <<1 //==0b0000'0000 0000'1000 left shifting . l will always become 8
	int l2 = >>1 // ==0b0000'0000 0000'0010 right shifting . l2 will always become 2


If you want to turn a value or an array of values into an array of byte_8 that are ordered in l_e or b_e, 
then please use the Basic_DynamucArray class.

THE USER IS RESPONSIBLE FOR PASSING CORRECT ARGUMENTS
*/


constexpr size_t getByte_8_Val(const byte_8* b){
	return static_cast<unsigned char>(*b);} // ensures that b is first converted to unsigned char. byte_8 is defined to be convertable to unsigned char.


constexpr size_t getByte_16_Val_l_E(const byte_8* b){
	const size_t convV = static_cast<size_t>(static_cast<unsigned char>(b[0])) + 
						(static_cast<size_t>(static_cast<unsigned char>(b[1])) << 8); // we do two casts here, the reason for the first can be seen at getByte_8_Val. 
	//The second casting is done, so that the value in bits is at least 4 bytes long. Otherwise doing the << 8 bitwise shift, we would lose all bits, because unsigned char is only 1 byte long
    return convV;}
constexpr size_t getByte_24_Val_l_E(const byte_8* b){
	const size_t convV = static_cast<size_t>(static_cast<unsigned char>(b[0])) + 
						(static_cast<size_t>(static_cast<unsigned char>(b[1])) << 8) +
						(static_cast<size_t>(static_cast<unsigned char>(b[2])) << 16);
	return convV;}
constexpr size_t getByte_32_Val_l_E(const byte_8* b){
	const size_t convV = static_cast<size_t>(static_cast<unsigned char>(b[0])) + 
						(static_cast<size_t>(static_cast<unsigned char>(b[1])) << 8) +
						(static_cast<size_t>(static_cast<unsigned char>(b[2])) << 16) +
						(static_cast<size_t>(static_cast<unsigned char>(b[3])) << 24);
	return convV;}


constexpr size_t getByte_16_Val_b_E(const byte_8* b){
	const size_t convV = static_cast<size_t>(static_cast<unsigned char>(b[1])) + 
						(static_cast<size_t>(static_cast<unsigned char>(b[0])) << 8); 
    return convV;}
constexpr size_t getByte_24_Val_b_E(const byte_8* b){
	const size_t convV = static_cast<size_t>(static_cast<unsigned char>(b[2])) + 
						(static_cast<size_t>(static_cast<unsigned char>(b[1])) << 8) +
						(static_cast<size_t>(static_cast<unsigned char>(b[0])) << 16);
	return convV;}
constexpr size_t getByte_32_Val_b_E(const byte_8* b){
	const size_t convV = static_cast<size_t>(static_cast<unsigned char>(b[3])) + 
						(static_cast<size_t>(static_cast<unsigned char>(b[2])) << 8) +
						(static_cast<size_t>(static_cast<unsigned char>(b[1])) << 16) +
						(static_cast<size_t>(static_cast<unsigned char>(b[0])) << 24);
	return convV;}


/*
the constructors and operators do only copy values. 
If you want DynamicArray to take over an array, you have to use the copyArray method and if necessary delete the old array. Up to C++20, there is no standard way to check if an array or pointer is static or dynamic. It is very unlikely, that this will change in future revisions.
New elements are added to the end of the array

It does not allow the user to directly change the order of elements.

Does not have a capacity-concept implemented yet. Many single item expansions are expensive
*/
class Basic_DynamicArray{
	public:
	Basic_DynamicArray()noexcept:data(nullptr),length(0)
		{}
	Basic_DynamicArray(const Basic_DynamicArray& cpy)noexcept:length(cpy.length){
		data=new byte_8[length];
		for(unsigned pos(0);pos!=length;++pos)
			data[pos]=cpy.data[pos];
	}
	Basic_DynamicArray(Basic_DynamicArray&& mv)noexcept:data(mv.data),length(mv.length){
		mv.length=0;
		mv.data=nullptr;
	}
	/*
	checks all pointer, throws runtime error, when ptr has a nul element

	*/
	Basic_DynamicArray(const byte_8* ptr, size_t len=1):length(len){
		data=new byte_8[length];
		for(;len!=0;)	// same as while(len!=0). Here, we iterate through the array. My thinking is, that I use for-loops when I iterate and while-loops when I do instructions as long as the situation is valid
			if(ptr+(--len)==nullptr) //starts at the end
				throw Kozy::Runtime_Error_obj(
					"Inavlid Array! You tried to copy an array, that has nul pointer!",
					"Basic_DynamicArray does not allow nul values." //We could have written 0 to the Array for when the pointer is nul, but this might have serious bad consequences later
				);
		for(size_t pos(0);pos!=length;++pos)
			data[pos]=ptr[pos];
	}
	Basic_DynamicArray(const byte_8& value)noexcept:Basic_DynamicArray(&value) //delegating constructor
		{}

	virtual ~Basic_DynamicArray() //made virtual, in case I inherit from this class
		{delete[] data;}

	Basic_DynamicArray& append(const byte_8& val)noexcept{ 
		expandArray(1);
		data[length-1]=val;

		
		return *this;
	}
	Basic_DynamicArray& copyArray(byte_8* arr,size_t len){
		size_t oldLength=length;
		expandArray(len);

		for(unsigned pos(0);pos!=len;++pos)
			data[pos+oldLength]=arr[pos];
		return *this;
	}

	Basic_DynamicArray& operator<<(const byte_8& val)
		{return append(val);}
	Basic_DynamicArray& operator+=(const byte_8& val)
		{return append(val);}
	Basic_DynamicArray& operator+=(const Basic_DynamicArray& cpy){
		const size_t oldLength=length;
		expandArray(length+cpy.length);

		for(unsigned pos(0);pos!=length;++pos)
			data[oldLength+pos]=cpy.data[pos];

		return *this;
	}	
	Basic_DynamicArray& operator+=(Basic_DynamicArray&& mv){
		//TEMP. This needs to be optimized, because it does not use the move-semantic efficiently yet.
		Basic_DynamicArray mvO(mv);
		(*this)+=mvO;
		return (*this);
	}
	Basic_DynamicArray operator+(const Basic_DynamicArray& val)
		{return Basic_DynamicArray(*this)+=val;}	
	Basic_DynamicArray operator+(const byte_8& val)
		{return Basic_DynamicArray(*this).append(val);}	
	byte_8& operator[](size_t pos){
		if(pos>=length){
			throw Out_Of_Range_obj(
				"You tried to access an element that is out of bounds",
				"Check the length before accessing DynamicArray."
			);
			return data[0];
		}
		return data[pos];
	}
	const byte_8& operator[](size_t pos)const
		{ return (*this)[pos];}
	operator bool()const noexcept
		{return isEmpty();}


	size_t getLength()const noexcept
		{return length;}
	bool isEmpty()const noexcept
		{return data;} //same as data != nullptr

	private:
	byte_8* data;
	size_t length;


	void expandArray(size_t len)noexcept{
		byte_8* newData=new byte_8[length+len];
		for(unsigned index(0);index!=length;++index) //I could have used something like copy_n from <algorithm>, but I try to use as little from the STL as possible for this project
			newData[index]==data[index];
		length+=len;
	}

};

/*
converts the parameter to an integral value and returns a DynamicArray containing a WORD
We define WORD to be 2 bytes long of which each is 8 bit long.

l_e == bytes are pushed in Little Endian order
b_e == bytes are pushed in Big Endian order

assumes:
 argument passed can be converted to unsigned int
 1 byte is 8 bit
*/
template<typename T>	//We could have named the unknown type something else, but if there is no good name, it is traditionally named "T"
Basic_DynamicArray castValueToWORD_l_e(const T& val){
	constexpr size_t maxOf16=0b1111'1111'1111'1111;
	const unsigned int Ival;
	if(val>maxOf16){
		throw Invalid_Argument_obj(
			"You entered a value that is bigger than 2^16 and the value will be clipped!",
			"You should either \nuse a smaller value, \ncast it to DWORD or \nsplit it into several WORDs"
		);
	Ival=maxOf16; 
	}else
	Ival=static_cast<unsigned int>(val); //we cast explicitly
	
	byte_8 casted[2];
	constexpr unsigned padding=(2-sizeof(unsigned)*8); //we need to do this, because unsigned int is on most machines either 2 or 4 bytes long. It is only defined to be at least 2 bytes long
	casted[0]=(Ival<<8+padding)>>(8+padding);	//+ has a higher precedence than <<,>>. We use parentheses to denote meaning.
	casted[1]=Ival>>8+padding;

	return Basic_DynamicArray(casted,2);
}
/*
cast to DWORD
*/
template<typename T>
Basic_DynamicArray castValueToDWORD_l_e(const T& val){
	constexpr size_t maxOf32=0b1111'1111'1111'1111'1111'1111'1111'1111;
	const unsigned long Ival;
	if(val>maxOf32){
		throw Invalid_Argument_obj(
			"You entered a value that is bigger than 2^16 and the value will be clipped!",
			"You should either \nuse a smaller value, \ncast it to DWORD or \nsplit it into several WORDs"
		);
	Ival=maxOf32; 
	}else
	Ival=val; //we cast implicitly. It is likely that there is no explicit cast to unsigned long
	
	byte_8 casted[4];

	//uses clipping on the right and left side to get the exact bytes
	constexpr unsigned padding=(4-sizeof(unsigned long)*8); //we need to do this, because unsigned long is only defined to be at least 4 bytes long
	casted[0]=(Ival<<24+padding)>>(24+padding);
	casted[1]=(Ival<<16+padding)>>(32+padding);
	casted[2]=(Ival<<8+padding)>>(40+padding);
	casted[3]=(Ival<<padding)>>(48+padding);

	return Basic_DynamicArray(casted,2);
}

/*
For more information look up the l_e variant

b_e == big Endian order
*/
template<typename T>
Basic_DynamicArray castValueToWORD_b_e(const T& val){
	constexpr size_t maxOf16=0b1111'1111'1111'1111;
	const unsigned int Ival;
	if(val>maxOf16){
		throw Invalid_Argument_obj(
			"You entered a value that is bigger than 2^16 and the value will be clipped!",
			"You should either \nuse a smaller value, \ncast it to DWORD or \nsplit it into several WORDs"
		);
	Ival=maxOf16; 
	}else
	Ival=static_cast<unsigned int>(val); //we cast explicitly
	
	byte_8 casted[2];
	constexpr unsigned padding=(2-sizeof(unsigned)*8); //we need to do this, because unsigned int is on most machines either 2 or 4 bytes long. It is only defined to be at least 2 bytes long
	casted[0]=Ival>>8+padding;
	casted[1]=Ival<<8+padding;

	return Basic_DynamicArray(casted,2);
}



}

}

#endif
