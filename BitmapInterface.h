#ifndef BITMAPINTERFACE_H
#define BITMAPINTERFACE_H


/*
Important, we only deal with 24 Bit_channels. Other Channels might be added at some point.
The user is not able to change the bit channel and for now, it is assumed that all BitMapInterface and ScanLine save pixels in 24 Bits 
*/

namespace Kozy{

//forward declarations
struct BitmapInterface;

namespace FileParsing{
BitmapInterface* load_Picture_BMP(BitmapInterface*, const char *const fileName);
BitmapInterface* save_Picture_BMP(BitmapInterface*, const char *const fileName);
}

struct Resolution{
	unsigned long width {0};	// 4 byte.
	unsigned long height{0};	
};


struct Pixel_24{
	unsigned red:8;
	unsigned green:8;
	unsigned blue:8;
};


struct ScanLine{
    public:
	ScanLine(unsigned lineWidth)noexcept;
	ScanLine(const ScanLine&, unsigned width);
	ScanLine(ScanLine&&);
	ScanLine(ScanLine&)=delete; // ScanLine does not save its width, so it is not possible to copy the array correctly
	ScanLine()noexcept;
		

	virtual ~ScanLine(){
		if(sLine != nullptr) // delete can be used on pointer that are nullptr. This check is only important for moved objects
			delete[] sLine;
	}

	ScanLine& operator=(const ScanLine&)=delete;
	ScanLine& operator=(ScanLine&&);
	ScanLine& operator=(unsigned)=delete;

	ScanLine& copy(const ScanLine&, unsigned width);

	bool operator==(const ScanLine&)=delete;
	bool operator!=(const ScanLine&)=delete;

	bool compare(const ScanLine&, unsigned width)const noexcept;

	// user is responsible for correct access
	Pixel_24* operator[](unsigned);
	
		
	private:
	Pixel_24* sLine{nullptr};

};


enum class Bit_Channel:unsigned{
	Bit_1 = 1, Bit_4 = 4, Bit_8 = 8, Bit_24 = 24,	//Version 3
	Bit_16 = 16, Bit_32 = 32						//Version 4
};


struct BitmapInterface{ 
	friend Kozy::BitmapInterface* Kozy::FileParsing::load_Picture_BMP(Kozy::BitmapInterface *pic, const char *const fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture
	friend Kozy::BitmapInterface* Kozy::FileParsing::save_Picture_BMP(Kozy::BitmapInterface *pic, const char *const fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture

    public:
    virtual ~BitmapInterface(){
			if(pixLines != nullptr) // delete can be used on pointer that are nullptr. This check is only important for moved objects
				delete[] pixLines;
		}
	
	virtual BitmapInterface& operator=(const BitmapInterface&);
	virtual BitmapInterface& operator=(BitmapInterface&&);

	/*
	compares each scanline
	returns true even if one is shorter than the other, as long as the shorter is equal to the part of the picture of the longer one.
	returns false if they have not the same width
	orientation may influence the result. Two black pictures return true, no matter the orientation. E.g. two pictures of the same dog result to false, if they have a different orientation
	an empty picture is only equal to another empty picture
	
	*/
	virtual bool operator==(const BitmapInterface&)const;

	/*
	same as == , but takes height also in account
	*/
	virtual bool exactlySame(const BitmapInterface&)const;

	/*
	
	*/
	bool hasPictureData()const noexcept
		{ return pixLines;}			//true, when the pointer is not nul
	
	/*
	false -- top-down
	true -- bottom-up
	*/
	void setPictureOrientation(const bool&) noexcept;

	constexpr bool getOrientation() const noexcept 
		{return orientation;}
	constexpr const Resolution& getRes() const noexcept
		{ return res;}
	const Bit_Channel& getBit_Channel()const noexcept
		{ return bitChannel;}




    private:



    protected:
    BitmapInterface();
	Resolution res;
	Bit_Channel bitChannel{Bit_24};	// right now, we only allow exactly 24 bits per pixel

	/* 
	res.height == length of ScanLine Array
	res.width  == length of pixels per Scanline 
	*/
	ScanLine* pixLines{nullptr}; 

	

	/*
	false -- top-down
	true -- bottom-up
	*/
	bool orientation{true}; 
};


}


#endif