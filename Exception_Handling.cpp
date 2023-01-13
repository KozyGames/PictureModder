#include <string>

#include "Exception_Handling.h"

using std::string;
using namespace Kozy;

//Error_Interface

Error_Interface::Error_Interface() {

}

Error_Interface::~Error_Interface() {

}

//Exception_obj

Exception_obj::Exception_obj(const char* cause, const char* solutions): std::exception(
	(string(msg_MarkerLine) +
	"RUNTIME_ERROR: " + cause + "\n"
	"POSSIBLE_SOLUTIONS: " + ((!solutions)? "-":solutions) + '\n' +
	msg_MarkerLine).c_str()
){

}

//Runtime_Error_obj

Runtime_Error_obj::Runtime_Error_obj(const char* cause, const char* solutions): std::runtime_error(
	string(msg_MarkerLine) +
	"RUNTIME_ERROR: " + cause + "\n"
	"POSSIBLE_SOLUTIONS: " + ((!solutions)? "-":solutions) + '\n' +
	msg_MarkerLine
	) {

}

//Invalid_Argument_obj

Invalid_Argument_obj::Invalid_Argument_obj(const char* cause, const char* solutions): std::invalid_argument(
	string(msg_MarkerLine) +
	"INVALID_ARGUMENT: " + cause + "\n"
	"POSSIBLE_SOLUTIONS: " + ((!solutions)? "-":solutions) + '\n' +
	msg_MarkerLine
	) {

}

//Out_Of_Range_obj

Out_Of_Range_obj::Out_Of_Range_obj(const char* cause, const char* solutions): std::out_of_range(
	string(msg_MarkerLine) +
	"INVALID_ARGUMENT: " + cause + "\n"
	"POSSIBLE_SOLUTIONS: " + ((!solutions)? "-":solutions) + '\n' +
	msg_MarkerLine
	) {

}