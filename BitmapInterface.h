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
BitmapInterface* load_Picture_BMP(BitmapInterface*, const char* fileName);
BitmapInterface* save_Picture_BMP(BitmapInterface*, const char* fileName);
}

struct Resolution{
	Resolution(unsigned long w, unsigned long h) : width(w), height(h) 
		{}
	Resolution() : width(0), height(0) 
		{}
	Resolution(const Resolution& cpy) : width(cpy.width), height(cpy.height) // this constructor is implicitly created, but for completeness sake, I added it
		{}

	unsigned long width;	// 4 byte.
	unsigned long height;	
};


struct Pixel_24{
	friend Kozy::BitmapInterface* Kozy::FileParsing::load_Picture_BMP(Kozy::BitmapInterface* pic, const char* fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture
	friend Kozy::BitmapInterface* Kozy::FileParsing::save_Picture_BMP(Kozy::BitmapInterface* pic, const char* fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture

	public:
	Pixel_24(unsigned argRed, unsigned argGreen, unsigned argBlue) noexcept : red(argRed), green(argGreen), blue(argBlue)
		{}
	Pixel_24(const Pixel_24& cpy) noexcept : red(cpy.red), green(cpy.green), blue(cpy.blue) 
		{}
	Pixel_24() noexcept : red(0), green(0), blue(0) 
		{}
	/*
	This is a primitive data structure, thus there is no real need to add a move constructor.
	Pixel_24(Pixel_24&& mv) noexcept : red(mv.red), green(mv.green), blue(mv.blue) { 
		mv.red = -1;
		mv.green = -1;
		mv.blue = -1;
	}*/

	unsigned getRed()const noexcept
		{return red;}
	unsigned getGreen()const noexcept
		{return green;}
	unsigned getBlue()const noexcept
		{return blue;}
	void getRGB(unsigned(&arr)[3])const noexcept {  // reference to a static array of 3 elements
		/*
		this is a really cool thing I learned from "Modern Effective C++"
		Passing an array by reference does actually pass a reference of the array.

		Whereas, passing an array by value does NOT pass a copy of the array. It would pass a copy of the first pointer, which has a slightly different meaning.

		The code below would still be valid if we were to pass a pointer, but the meaning would be different and also the array could be of a different size, thus being less safe than the reference.

		A reference parameter forces the passed object to be exactly the length declared in the parameter. For example this function does not allow the user to pass an array with not exactly 3 elements!
		Another benefit is, that we can safely add the "noexcept" qualifier to this function. This can improve compiled code, thus performance.
		*/
		arr[0] = red;
		arr[1] = green;
		arr[2] = blue;
	}

	Pixel_24& setRed(unsigned value) noexcept {
		red=value;
		return *this;}
	Pixel_24& setGreen(unsigned value) noexcept {
		green=value;
		return *this;}
	Pixel_24& setBlue(unsigned value) noexcept {
		blue=value;
		return *this;}
	Pixel_24& setRGB(unsigned argRed, unsigned argGreen, unsigned argBlue)noexcept {
		red = argRed;
		green = argGreen;
		blue = argBlue;

		return *this;
	}
	Pixel_24& setRGB(const unsigned(&arr)[3]) noexcept{  // reference to a static array of 3 elements
		red = arr[0];
		green = arr[1];
		blue = arr[2];

		return *this;
	}
	Pixel_24& setRGB(const Pixel_24& cpy)noexcept {
		red = cpy.red;
		green = cpy.green;
		blue = cpy.blue;

		return *this;
	}
	Pixel_24& setRGB(unsigned value)noexcept {
		red = green = blue = value;

		return *this;
	}
	Pixel_24& operator=(const Pixel_24& cpy) noexcept 
		{ return setRGB(cpy); }
	Pixel_24& operator=(const unsigned(&arr)[3]) noexcept 
		{  return setRGB(arr);}
	Pixel_24& operator=(Pixel_24&& mv) noexcept {
		red=mv.red;
		green = mv.green;
		blue = mv.blue;

		mv.red = mv.green = mv.blue = -1; // same as = 255. 
		return *this; 
	}

	/* 
	the member fields are bit fields. These are not allowed to return pointers nor references!
	We change the function to only return values, but that would break the meaning of the subscript operator.
	*/
	unsigned& operator[](unsigned pos) = delete; 

	bool operator==(const Pixel_24& rhs)const noexcept 
		{return red == rhs.red && green == rhs.green && blue == rhs.blue;}
	bool operator!=(const Pixel_24& rhs)const noexcept 
		{return *this==rhs;}


	private:
	unsigned red:8;
	unsigned green:8;
	unsigned blue:8;
};


struct ScanLine{
	friend Kozy::BitmapInterface* Kozy::FileParsing::load_Picture_BMP(Kozy::BitmapInterface* pic, const char* fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture
	friend Kozy::BitmapInterface* Kozy::FileParsing::save_Picture_BMP(Kozy::BitmapInterface* pic, const char* fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture
	friend struct BitmapInterface;

    public:
	ScanLine(unsigned lineWidth)noexcept;
	ScanLine(unsigned lineWidth, unsigned value)noexcept;
	ScanLine(unsigned lineWidth, const unsigned (&arr)[3])noexcept;

	ScanLine(const ScanLine&, unsigned width);
	ScanLine(ScanLine&&)noexcept;
	ScanLine(const ScanLine&)=delete; // ScanLine does not save its width, so it is not possible to copy the array correctly
	ScanLine()noexcept;
		

	virtual ~ScanLine(){
		if(sLine != nullptr) // delete can be used on pointer that are nullptr. This check is only important for moved objects
			delete[] sLine;
	}

	ScanLine& operator=(const ScanLine&)=delete;
	ScanLine& operator=(ScanLine&&)noexcept;
	ScanLine& operator=(unsigned)=delete;

	ScanLine& copy(const ScanLine&, unsigned long width);

	bool operator==(const ScanLine&)=delete;
	bool operator!=(const ScanLine&)=delete;

	bool compare(const ScanLine&, unsigned width)const noexcept;

	// user is responsible for correct access
	const Pixel_24& operator[](unsigned long long)const;
	Pixel_24& operator[](unsigned long long);
	
		
	private:
	Pixel_24* sLine{nullptr};

};


enum class Bit_Channel:unsigned{
	Bit_1 = 1, Bit_4 = 4, Bit_8 = 8, Bit_24 = 24,	//Version 3
	Bit_16 = 16, Bit_32 = 32						//Version 4
};


struct BitmapInterface{ 
	friend Kozy::BitmapInterface* Kozy::FileParsing::load_Picture_BMP(Kozy::BitmapInterface *pic, const char* fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture
	friend Kozy::BitmapInterface* Kozy::FileParsing::save_Picture_BMP(Kozy::BitmapInterface *pic, const char* fileName); // this function does the heavy in-depth construction of a new Bitmap/Picture

    public:
    virtual ~BitmapInterface(){
			if(pixLines != nullptr) // delete can be used on pointer that are nullptr. This check is only important for moved objects
				delete[] pixLines;
		}
	
	BitmapInterface& operator=(const BitmapInterface&);
	BitmapInterface& operator=(BitmapInterface&&)noexcept;

	ScanLine& operator[](unsigned long index);
	const ScanLine& operator[](unsigned long index)const;

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
	void setPictureOrientation(bool) noexcept;

	constexpr bool getOrientation() const noexcept 
		{return orientation;}
	constexpr const Resolution& getRes() const noexcept
		{ return res;}
	const Bit_Channel& getBit_Channel()const noexcept
		{ return bitChannel;}




    private:



    protected:
	//Constructors are hidden for the user. They are supposed to use the Picture class
    BitmapInterface()noexcept;
	BitmapInterface(const Resolution&, unsigned value)noexcept;
	BitmapInterface(const Resolution& r)noexcept : BitmapInterface(r, 0)
		{}

	BitmapInterface(const Resolution&, const unsigned (&arr)[3])noexcept;


	Resolution res;
	Bit_Channel bitChannel{Bit_Channel::Bit_24};	// right now, we only allow exactly 24 bits per pixel

	/* 
	res.height == length of ScanLine Array
	res.width  == length of pixels per Scanline 
	*/
	ScanLine* pixLines; 

	

	/*
	false -- top-down
	true -- bottom-up
	*/
	bool orientation{ true };
};


}


#endif