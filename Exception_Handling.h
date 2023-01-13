#ifndef EXCEPTION_HANDLING_H
#define EXCEPTION_HANDLING_H

#include <stdexcept>

/*
 The LWG states in Issue 471, 
 that one has to at least implicitly define a copy assignment operator (=) and that the "what"-member of both become the same, to conform to the standard.
 We do that. 
*/

namespace Kozy {

struct Error_Interface {
	public:
	virtual ~Error_Interface();


	private:

	
	protected:
	static constexpr const char* const msg_MarkerLine{ "\n///\n\n" }; 
	/*
	Modern C++ tries to decrease the amount of Macros that are used. 
	This is the modern approach for a value, that is known at compile time and does not change. 
	It has several advantages over a Macro and over other styles.*/
	
	Error_Interface();

};

struct Exception_obj: public Error_Interface, public std::exception {
	public:
	Exception_obj(const char* cause, const char* solutions);


	private:

};

struct Runtime_Error_obj: public Error_Interface, public std::runtime_error {
	public:
	Runtime_Error_obj(const char* cause, const char* solutions);


	private:

};

struct Invalid_Argument_obj: public Error_Interface, public std::invalid_argument {
	public:
	Invalid_Argument_obj(const char* cause, const char* solutions);


	private:

};
struct Out_Of_Range_obj: public Error_Interface, public std::out_of_range {
	public:
	Out_Of_Range_obj(const char* cause, const char* solutions);


	private:

};

}

#endif