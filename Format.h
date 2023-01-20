#ifndef FORMAT_H
#define FORMAT_H

#include <functional> //used for std::hash. Not guaranteed to always have size_t definition!
#include <cstring> //guaranteed to have the definition for size_t 
#include <string> //should have a definition of size_t in it, but I did not find a conclusive answer to it

namespace Kozy {


struct Format {
private:
	static constexpr std::hash<std::string> hasher{};

public:
	Format(const char* const str) :name(str), cacheHsh(hasher(std::string(str))) {
	}
	Format(const Format& f) :name(f.name), cacheHsh(f.cacheHsh) {
	}
	Format(Format&& f)noexcept :name(std::move(f.name)), cacheHsh(f.cacheHsh) {
	}
	Format(const Format* const f) :Format(*f) {
	}
	Format() : Format(NO_FORMAT) {
	}
	virtual ~Format() {
	}

	Format& operator=(const Format& rhs) noexcept {
		name = rhs.name;
		cacheHsh = rhs.cacheHsh;
		return *this;
	}
	Format& operator=(Format&& rhs) noexcept {
		name = std::move(rhs.name);
		cacheHsh = rhs.cacheHsh;
		return *this;
	}
	Format& operator=(const char* const str) noexcept {
		name = str;
		cacheHsh = hasher(std::string(name));
		return *this;
	}

	bool operator==(const Format& rhs) const noexcept {
		return cacheHsh == rhs.cacheHsh;
	}
	bool operator!=(const Format& rhs) const noexcept {
		return cacheHsh != rhs.cacheHsh;
	}


	explicit operator size_t() const noexcept //only gets casted to size_t, when explicitly asked to
		{ return cacheHsh;}
	operator const char* () const noexcept {
		return name.c_str();
	}
	operator bool() const noexcept {
		return isValid();
	}

	virtual bool isValid() const noexcept;	//made virtual so that if one inherits of this datastructure, one can use their own isValid function 


private:
	static constexpr const char* const NO_FORMAT{ "NO_FORMAT" };	//The const after the * is redundant. This was done to make it clear, that this is a pointer that can not be changed.

	std::string name; //Denotes the type of a file. E.g.: 'png'
	size_t cacheHsh; //for efficiency and easier operations, the string is hashed
};
const Format BLANKFORMAT{};	//object of Format that represents a bad Format


/*
 * expects '\0' to be the last element
 *
 * throws:
 * invalid_argument -- fileName either does not contain '.' or has other problems. returns BLANKFORMAT.
 */
const Format getFormatByFileName(const char* fileName);

}


#endif