#ifndef PICTURE_H
#define PICTURE_H

#include "Format.h"
#include "BitmapInterface.h"

/*
- state-system has not yet been really implemented and will surely change. So even if isValid returns true, Picture might not be safe to use. I have not yet decided how I want it to be

*/
namespace Kozy{

struct Format;


/*
* internally, always saves picture data in a bmp manner
* 
*/
class Picture : public BitmapInterface{
	public:
	Picture()noexcept;
	Picture(const char* fileName);
	Picture(const Picture&);
	Picture(Picture&&)noexcept;
	Picture(const Picture* const ptr)	//this is simply added, so that the user does not need to dereference their pointer
		:Picture(*ptr){}

	virtual ~Picture();

	Picture& operator=(const Picture&);
	Picture& operator=(Picture&&)noexcept;
	Picture& operator=(const char*)=delete; //deleted for preference. I do not want the user to do something like this: pic = "String"; .

	//bool operator== does not exist yet. This class uses its parents equality operator



	
	const Format& getFormat() const noexcept;

	operator bool()const noexcept
		{return isValid();}

	bool isValid()const noexcept;
	//can be assigned to a bool function so to have additional checks in the isValid function. At least for now, it must not throw any exceptions
	bool (*extraCheckForIsValid)()noexcept{nullptr};

	enum class State:unsigned{
		Good=0,
		Fatal_Error = 0b1,

		Failed_Loading = 0b10,
		Failed_Loading_AND_Fatal_Error = 0b11,

		Failed_Operation = 0b100,
		Failed_Operation_AND_Failed_Loading = 0b110,
		Failed_Operation_Fatal_Error = 0b101,
		Failed_Operation_AND_Failed_Loading_AND_Fatal_Error = 0b111

	};

	unsigned getState()const noexcept
		{ return state;}
	unsigned setState(unsigned st)const noexcept
		{ return state = st;}
	unsigned enableState(unsigned st)const noexcept
		{ return state = st|state; }
	unsigned disableState(unsigned st)const noexcept
		{ return state = ~st&state; } //inverts the byte, then AND each bit of st and state
	bool isState(unsigned st)const noexcept
		{ return st==state;}
	bool doesStateContain(unsigned st) const noexcept
		{return (st&state) == st;} // == has a higher precedence than &
	bool doesStateContainAny(unsigned st) const noexcept
		{return (~st&state) != state;} 

	
	unsigned setState(const State& st)const noexcept
		{ return setState(static_cast<unsigned>(st));}
	unsigned enableState(const State& st)const noexcept
		{ return enableState(static_cast<unsigned>(st));}
	unsigned disableState(const State& st)const noexcept
		{ return disableState(static_cast<unsigned>(st));}
	/*
	returns true if the state is exactly st
	*/
	bool isState(const State& st) const noexcept
		{ return isState(static_cast<unsigned>(st));}
	/*
	returns true if the state contains exactly st 
	*/
	bool doesStateContain(State st) const noexcept
		{return doesStateContain(static_cast<unsigned>(st));}
	/*
	returns true if any State of st is enabled
	*/
	bool doesStateContainAny(State st) const noexcept
		{return doesStateContainAny(static_cast<unsigned>(st));}

	const Picture& save(const char* fileName, const Format& )const;
	const Picture& save(const char* fileName)const;

	
	private:
	
	/*
	This is a typical C manner to store several values in one integral type. Meaning, we use efficiently the whole range of the bytes of a memory address.

	Nowadays, Modern C++ tries to avoid such verbose concepts. 
	Nevertheless, I wanted to add this for simply showing this concept, binary literals and the ' literal seperator

	There are several alternatives, that are easier to read and in addition to that are also very efficient:
	enum, enum class, std::vector<bool>, std::bitset, bitfields, writing a State data structure


	0b0			== Good
	0b1			== Fatal Failure	
		it is no longer valid and might have bad data
	0b10 		== Failed to load/change
	0b100 		== Failed operation
	0b1000 		== 
	*/
	mutable unsigned state{0b0000'0001};	//used mutable specifier, so that the state of a const Picture can also be changed.

	protected:
	Format format;
	
	
};


	
}


#endif

