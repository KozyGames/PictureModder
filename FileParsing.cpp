#include <string>
#include <fstream>
#include <iostream>


#include "FileParsing.h"
#include "Picture.h"
#include "PictureProject.h"
#include "Exception_Handling.h"
#include "Utility.h"

using namespace Kozy;
using namespace Kozy::FileParsing;
using namespace Kozy::Utility;

using std::string; using std::ifstream; using std::ofstream;



Picture* FileParsing::load_Picture(Picture* pic, const char* fileName){
	using namespace std;

	if (!fileExists(fileName)) {
		throw Invalid_Argument_obj((
			string(fileName) + " can not be found.").c_str(), 
			"Make sure you spelled the name of the file correctly and also that the path is correct."
		);
		pic->enableState(Picture::State::Failed_Loading);

		return pic; 
	} else {
		if(getFormatByFileName(fileName)== Format("bmp")||getFormatByFileName(fileName)==Format("dib")){
			try{
				load_Picture_BMP(pic, fileName);
			}catch(const Exception_obj& e){
				throw e;
				pic->enableState(Picture::State::Failed_Loading);
				
			}

			return pic;		
		}else{
		pic->enableState(Picture::State::Failed_Loading);

		throw Invalid_Argument_obj((
			string(fileName) + " can not be loaded.").c_str(), 
			"Unfortunately the format of the file is not known or has no conversion yet.");

		return pic; 
		}
	}
}

BitmapInterface* Kozy::FileParsing::load_Picture_BMP(BitmapInterface* basePic, const char* fileName){
	/*
	credits to: Martin Reddy(1), Microsoft(2) and others for their technical articles of the bmp format. The following are the ones containing all information and more to this topic. Unfortunately, there is not one reference containing all information 
	1) http://www.martinreddy.net/gfx/2d/BMP.txt
	2) https://learn.microsoft.com/en-us/windows/win32/gdi/bitmap-structures		//overview
	2) https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types 	//Most(if not even all) types used by microsoft (file-)software
	3) https://www.fileformat.info/format/bmp/egff.htm								// although this was last updated in 2005 and references only sources from the 1990s, it is in my opinion the most detailed article and easiest to understand.
	4) http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm						

	
	Microsoft defines specific ranges for its types used in files. The ones we need to know:
	WORD						16-Bit unsigned, "The range is 0 through 65535 decimal."
	DWORD						32-Bit unsigned, "The range is 0 through 4294967295 decimal."
	LONG						32-Bit signed, "The range is -2147483648 through 2147483647 decimal.". 

	

	__HEADER - always 14 Bytes 
	-- a file Header and a cpp header are slightly different. A file header is the entry of a file and explains how the data is structured and to be interpreted
	-- Bytes are ordered in 'Little Endian' convention. Search for "Little Endian". Byte_8.h contains a short summary and examples
	-- Microsoft and POSIX define a Byte to be exactly 8 bit. Important for whenever dealing with raw data and/or Electronics. 
	-- Do not assume, that a byte is always 8 bit. C/C++ only guarantees for example char to be a byte long, but not how many bits there are in a byte.  
	
	TYPE 	ELEMENT_NAME	POS.(IN DEC.BYTE)	SIZE_IN_BYTE
	WORD 	Signature 		0					2	== 'B', 'M'

	DWORD 	FileSize		2					4	

	WORD 	Reserved_1 		6					2	== 0 
	WORD	Reserved_2		8					2 	== 0 
		We do not use them and I did not find out what they are reserved for. Could be a legacy thing. 
		In some texts, they are combined as one DWORD. According to Microsoft, we must set both to 0.
	
	DWORD 	DataOffset 		10					4
		tells us the address(position) of the first image data byte  


	__BITMAPINFOHEADER 
	-- as of today[21.12.2022] there are three official standard versions called Version 3, Version 4 and Version 5. 
	There are other Version variants, which we ignore and will likely not encounter. Nowadays, mostly Version 3 in 24 Bit without compression are used. If compression is important, one should use another format for storage anyway.
	-- all versions after 3 contain the preceding versions. 
	-- !!! We only use features of version 3. Newer Versions might work, but they are not guaranteed by me!!!

	VERSION 3. header, without any additional features, is 40 Bytes long
	-- all official succeeding versions are backwards compatible to Version 3. and are thus guaranteed to be longer than 40 Bytes
	
	TYPE ELEMENT_NAME		POS.(IN DEC.BYTE)	SIZE_IN_BYTE
	DWORD InfoSize			14					4 == 40 (standard)
		size of info header. Does not include color, mask nor other tables	

	LONG ImageWidth			18					4
		excluding padding.

	LONG ImageHeight		22					4 
		positive height == image starts at bottom left 
		Example 
			height == 12 == 0...0 0...0 0...0 00001100 | in little Endian == 00001100 0...0 0...0 0...0
		negative height == image starts at top left
		
		I did not find any clear information on what to do with negative width. For now, I ignore the signed bit. Keep in mind, there is actually no official specifications sheet made by Microsoft!

	WORD  Planes			26					2 == is always 1. BMP only has one color plane
	WORD  BitCount			28					2 == 
		In how many bits one pixel is stored. 
		Only 1,4,8 and 24 are valid values for Version 3. 
		Since Version 4: 16 and 32 are valid too. Might later be added to this program 

  	DWORD CompressionType	30					4 == Type of encoding. 
		0 == no compression. 
		1 == 8-Bit Run-length Encoding 
		2 == 4-Bit Run-length Encoding
		3 == bit field
		As far as I can tell, any other value might have a meaning for videos but not for pictures.

 	DWORD ImageSize			34		4 == size of the BitMap. 
		Uncompressed, it is equal to the dimensions of a picture in bytes including padding. Padding is defined to be bytes that are 0. Padding is a reoccuring concept and can be different in other contexts!
		A Scanline is defined to be one row of pixels. So a screen with 1920x1080 dimensions, has 1920 scanlines of which each is 1080 pixels wide.
		A Scanline in bytes has to be divisable by 4. So, for example, if the width of an 24-Bit-channel image (meaning, each pixel gets 3 bytes allocated) is 9, then ImageWidth is 9 and 27 in bytes but the Scanline image data width is 28 (27+1). 
			Examples for ImageSize values:
			a 1920x1080 24-Bit picture results in 1920*(1080*3+0) == 6.220.800
			a 9x6 24-Bit picture results in 9*(6*3+2) == 180
			For uncompressed images it is usually set to 0
		It is important for when the BitMap is compressed.
		When uncompressed, it can be set to 0

  	LONG  HoPelsPerMeter	38					4 == can be 0
  	LONG  VePelsPerMeter	42					4
		I did not find any information about what a negative value means for PixelPerMeter 
		0 is a valid value and uses the target devices native resolution
		more on DPI and PPI : https://en.wikipedia.org/wiki/Dots_per_inch
		We do not use this

  	DWORD ColorsUsed		46					4
		only for when there is a color table and also BitCount is less than 16. Otherwise it is 0.
		indicates how many unique colors there are inside of the color table
		
  	DWORD ColorsImportant	50					4
		it is 0 if there is no color table "palette"
		indicates the most frequently used colors. Useful for devices that have a limited color palette. 

		The color table is mainly used for if one wants to display an image that has a greater range of colors than the target device can display
	
	Implementation:
		I wrote the infrastructure for several features, but because of time restrictions, we only use 24 Bit channels. The vast majority of bmp files are of this variant anyway.
		If you have a native bmp file that is of different Bit channel, then you can do a work-around for now: 
			use an online converter to convert it to png and then convert png to bmp. It will be very very likely 24 Bit Channel and uncompressed.

		In theory, it can read compressed files of type 1(8-bit RLE ) and 2 (4-Bit RLE). I did not find a compressed bmp file to test it, yet.
	*/


	/*
	unsigned long is defined to be at least 4 bytes long >= 32 Bit for if 1 Byte == 8 Bit
	unsigned long long is defined to be at least 8 bytes long >= 64 Bit

	Technically, this is an overkill, but is easier to read (WORD == 1 Long, DWORD/LONG == 2 Longs)
	and thus also easier to maintain/modify.
	*/
	typedef unsigned long long DWORD;
	typedef unsigned long WORD;



	// Reading bitmap file into Picture
	
	ifstream ifStr(fileName, std::ios_base::binary); // we are not parsing text, we parse data. otherwise char that represeent escape sequences will lead to errors


	constexpr unsigned BMP_HEADER_LENGTH_IN_BYTE = 14;
	byte_8 headerStr[BMP_HEADER_LENGTH_IN_BYTE];

	
	ifStr.read(static_cast<char*>(static_cast<void*>(headerStr)),BMP_HEADER_LENGTH_IN_BYTE); // works only if a byte is 8 bit

	if(ifStr.good()&&headerStr[0]=='B'&&headerStr[1]=='M'){	//Signature
		
		

		const DWORD fileSize = getByte_32_Val_l_E(headerStr+2);	//FileSize 4 Bytes
		const WORD reserved_1 = getByte_16_Val_l_E(headerStr+6);
		const WORD reserved_2 = getByte_16_Val_l_E(headerStr+8);
		//reserved 2+2 Bytes is unused
        const DWORD dataOffset = getByte_32_Val_l_E(headerStr+10); //DataOffset 4 Bytes
			

		byte_8 headerInfoSizeByte[4];
		ifStr.read(static_cast<char*>(static_cast<void*>(headerInfoSizeByte)),4);
		const DWORD headerInfoSize=getByte_32_Val_l_E(headerInfoSizeByte);	
		if(headerInfoSize<40){
			throw Exception_obj(
				(string(fileName) + " is corrupt!\nHeaderInfo can not be safely read.").c_str(),
				"The file seems to be bad.\nTry a backup or use a different file.");
			return nullptr;}


		byte_8* headerInfo=new byte_8[headerInfoSize-4];
		ifStr.read(static_cast<char*>(static_cast<void*>(headerInfo)),headerInfoSize-4);
		//const DWORD width = getByte_32_Val_l_E(headerInfo);	
			/*
			I did not find any information on what to do with the signed value of width.
			We could use it to define a wider picture, but it is more likely that we mistakenly misinterpret that value, than use it correctly. 
			3 Bits represent a variation of 2^3 == 8 : 000, 001, 010, 011, 100, 101, 110, 111
			So if we were to remove the first bit of that we would have only 2^2 == 4 variations : X00, X01, X10, X11
			It is more likely that someone needs the first bit to have a specific meaning, like mirroring the picture, than that they need 2^32(== 4.294.967.296) instead of "just" 2^31(==2.147.483.648) pixels in width
			*/  
		const DWORD width = getByte_32_Val_l_E(headerInfo)<<1;	// discarding the first bit. Signed types use the first bit of a byte to indicate whether it is a positive or negative number
		const DWORD height = getByte_32_Val_l_E(headerInfo+4)<<1;

		const bool orientation = height>>31; // bitwise shift by 31, so that only the last bit of the 32 bit value is left.
		
		const WORD planes = getByte_16_Val_l_E(headerInfo+8); //should be 1, but we do not check it, because it would not change our usage. Technically, we would have to check it and throw an exception if it is not 1
		//planes 2 Byte is unused. 
    	
		const WORD bitCount=getByte_16_Val_l_E(headerInfo+10);

		const DWORD compressionType = getByte_32_Val_l_E(headerInfo+12);
		const DWORD ImageSize = getByte_32_Val_l_E(headerInfo+16);
		
		const DWORD horPelsPerMeter = getByte_32_Val_l_E(headerInfo+20);
		const DWORD verPelsPerMeter = getByte_32_Val_l_E(headerInfo+24);
		// Horizontal and Vertical PelsPerMeter, each 4 Byte, are not used. We do not have a meaning to it right now.

		const DWORD colorsUsed = getByte_32_Val_l_E(headerInfo+28); //not used atm
		const DWORD importantColors = getByte_32_Val_l_E(headerInfo+32); // not used atm
			

		delete[] headerInfo;
		finishStream(ifStr);


		if (bitCount !=1 && bitCount !=4 && bitCount !=8 &&bitCount !=24 || compressionType >=3){ //compressionType 3 is only allowed for 16 and 32 Bits
		    throw Exception_obj(
						(string(fileName) + " Only 1,4,8 and 24 Bit-Channels are allowed.").c_str(),
						"Please use another file or convert it to Version 3."
						);
			return nullptr;	
		}

		if (bitCount ==1 || bitCount ==4 || bitCount ==8){ //TEMP. I plan to add these bit channels at some point
		    throw Exception_obj(
						(string(fileName) + " Only 24 Bit-Channels are implemented right now.").c_str(),
						"Please use another file or convert it to Version 3 with 24 Bit.");
			return nullptr;	}



		ifStr.open(fileName); //we read the file again and use the offset provided in the header. This is safer, because that is exactly what DataOffset is designed for.  
		byte_8* completeFile = new byte_8[fileSize];
	    ifStr.read(static_cast<char*>(static_cast<void*>(completeFile)),fileSize); 


		//modifying basePic
		/*
		right now, it assumes that each pixel in the bitmap data is saved in 24 Bits. Meaning that every 24 Bits a new pixel is defined

		*/
		switch(compressionType){
			case 0:

			delete[] basePic->pixLines;
			basePic->pixLines=new ScanLine[height];

            
			/*
			one pixel consists of 3 bytes, each representing one color value. RGB == RED, GREEN, BLUE
			bytes inside of a pixel are ordered in "little Endian".
			Meaning, the data in the bitmap looks like this: Pixel_1: BLUE_BYTE, GREEN_BYTE, RED_BYTE, Pixel_2: BLUE_BYTE, GREEN_BYTE, RED_BYTE,....
			*/
			
			byte_8* bitmapData = completeFile+dataOffset; //pixel data array 
			const WORD scanlinePadding = 4 - (width*3)%4; // padding
			const DWORD scanlineWidthInBytes = width*3 + scanlinePadding; //bytes per scanline + padding

			/*
			we loop through the bitmap 
			we copy each pixel of each scanline and skip any padding

			we make a reference to increase readability
			otherwise the statement for the blue part of a pixel inside the nested loop would look like this:
				basePic->pixLines[hgh][pos-hgh*scanlineWidthInBytes]->blue;
	
			*/
			for(DWORD pos{0},hgh{0};hgh<height;++hgh){	// we go through each scanline of the picture. a 1920x1080 Picture has 1920 scanlines
				ScanLine& curLine = basePic->pixLines[hgh]; 
				for(;pos!=scanlineWidthInBytes*(hgh+1)-scanlinePadding;pos+=3){
					Pixel_24& curPixel = *curLine[pos-hgh*scanlineWidthInBytes];

					curPixel.blue = getByte_8_Val(bitmapData+pos);
					curPixel.green = getByte_8_Val(bitmapData+pos+1);
					curPixel.red = getByte_8_Val(bitmapData+pos+2);
           		}
				pos += scanlinePadding;	//skip padding
			}


			break;

			case 1:
			

			// TODO 24 Bit channel that uses 8-Bit RLE 
			/* 
			as described in: https://users.cs.fiu.edu/~czhang/teaching/cop4225/project_files/bitmap_format.htm
			*/
			
			break;

			case 2:
			

			throw Exception_obj(
						(string(fileName) + " This program does not do any compression yet.").c_str(),
					    "Please use another file or convert it to Version 3 without compression."
			);
			finishStream(ifStr);
			delete[] completeFile;

			return nullptr;
				//TODO 4-Bit RLE

			break;

			case 3:

			throw Exception_obj(
						(string(fileName) + " This program does not do any compression yet.").c_str(),
				    	"Please use another file or convert it to Version 3 without compression."
			);
			finishStream(ifStr);
			delete[] completeFile;

			return nullptr;

				//Here could be code for 16 and 32 Bit channels that use Bitfield compression. 
			break;
			}

				
		basePic->res.width= width;
		basePic->res.height= height;
		basePic->setPictureOrientation(orientation);

		delete[] completeFile;
		finishStream(ifStr);

		return basePic;
	} 
	throw Exception_obj((string(fileName) + " is corrupt!\nIt can not be safely read.").c_str(),
						"The file seems to be bad.\nTry a backup or use a different file.");
	

	finishStream(ifStr);
	return nullptr;
}

const Picture* Kozy::FileParsing::save_Picture(const Picture* pic, const char* fileName){
	return save_Picture(pic,fileName,pic->getFormat());
}
const Picture* Kozy::FileParsing::save_Picture(const Picture* pic, const char* fileName, const Format& f){	
	if (pic->doesStateContain(Picture::State::Fatal_Error)){
		throw Exception_obj((string(fileName) + " is corrupt!\nIt can not be safely read.").c_str(),
						"The file seems to be bad.\nTry a backup or use a different file.");
		pic->enableState(Picture::State::Failed_Operation);
	}else{
		const string fileType=string(".")+ static_cast<const char *>(f); //example: ".bmp"
		const string fullFileName = string(fileName) + fileType;	// example: "EXAMPLE.bmp"

		if (fileExists(fullFileName.c_str())){
			throw Invalid_Argument_obj((fullFileName + " does already exist!").c_str(),
									   "\"_new\" will be added to the filename.");

			pic->enableState(Picture::State::Failed_Operation);
			return save_Picture(pic,(string(fileName)+"_new" + fileType).c_str(),f);	//although in most cases I prefer to use loops instead of recursion or goto's, this is one of the rare cases, where I think recursion is the better approach
		}
		if (f == BLANKFORMAT){
			throw Invalid_Argument_obj((string(f) + " has no type!").c_str(),
									   "The picture will be saved as a bmp file.\nNext time, please set a specific type for your picture.");

			pic->enableState(Picture::State::Failed_Operation);
		}
		else if (f != Format("bmp")){
			throw Invalid_Argument_obj((string(f) + " has not been implemented yet!").c_str(),
									   "The picture will be saved as a bmp file.");

			pic->enableState(Picture::State::Failed_Operation);
		}

		if (pic->hasPictureData()){
			
			save_Picture_BMP(pic, fullFileName.c_str());
		}
	}
	return pic;
}

const BitmapInterface* Kozy::FileParsing::save_Picture_BMP(const BitmapInterface* basePic, const char* fileName){
	using namespace std;
	//>>>>>>>> OVERWRITES EXISTING FILES!!! <<<<<<<<<< 
	/*
	See load_Picture_BMP for more detailed information

	unsigned long is defined to be at least 4 bytes long >= 32 Bit for if 1 Byte == 8 Bit
	unsigned long long is defined to be at least 8 bytes long >= 64 Bit

	Technically, this is an overkill, but is easier to read:
		WORD == 1 Long, DWORD/LONG == 2 Longs)


	We save the picture as a BMP of Version 3. Uncompressed, 24 Bit channel
	*/
	typedef unsigned long long DWORD;
	typedef unsigned long WORD;

	//utility variables
	const DWORD width = basePic->getRes().width;
	const DWORD height = basePic->getRes().height;
	const WORD scanlinePadding = 4 - (width*3)%4; // padding
	const DWORD scanlineWidthInBytes = width*3 + scanlinePadding; //bytes per scanline + padding

	/*
	variables are ordered in the way they will be written to the stream
	*/
	//header
	constexpr unsigned char signature[] = {'B','M'};

	

	const DWORD fileSize=
							14+		//header
							40+ 	//infoHeader
							scanlineWidthInBytes*height; //bitmap data with optional padding

	const DWORD reserved_1_And_2 = 0;
	constexpr DWORD dataOffset = 64; //header + infoHeader

	//infoHeader
	constexpr DWORD infoHeaderSize = 40;
	//width
	//height
	constexpr WORD planes = 1;
	constexpr WORD bitChannel = 24;
	constexpr DWORD compressionType =0;
	constexpr DWORD imageSize = 0;
	constexpr DWORD hoPelsPerMeter =0;
	constexpr DWORD vePelsPerMeter =0;
	constexpr DWORD colorsUsed =0;
	constexpr DWORD colorsImportant =0;


	/*
	we do not do checks, because we assume that the user used the delegator function save_picture. 
	Otherwise, they have been warned in the header! 
	If they use this function directly, they are responsible for error handling. 
	Also they have been informed, that this function >>>>>> OVERWRITES EXISTING FILES!!! <<<<<<<<<< 
	*/
	ofstream ostr(fileName); 

	//ostr
	//	<< 
	return basePic;
}

Kozy::PictureProject* FileParsing::load_PictureProject(PictureProject* proj, const char* fileName){

	return nullptr;
}
