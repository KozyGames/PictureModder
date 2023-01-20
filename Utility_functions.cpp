#include <fstream>

#include "Utility.h"


using namespace Kozy;
using namespace Kozy::Utility;

const SystemManager Kozy::initialize() {
	return SystemManager{};
}
const SystemManager systemM = initialize();

bool Kozy::Utility::fileExists(const char* fileName)noexcept {
	std::ifstream ifStr(fileName);
	if(ifStr.is_open()){
		finishStream(ifStr);
		return true;
	}
	else {
		finishStream(ifStr);
		return false;
	}
}

bool Kozy::Utility::finishStream(std::ifstream& fs){
	bool wasState = fs.rdstate() == std::ios_base::goodbit;
	fs.close();
	fs.setstate(std::ios_base::goodbit);
	return wasState;
}
bool Kozy::Utility::finishStream(std::ofstream& fs){
	bool wasState = fs.rdstate() == std::ios_base::goodbit;
	fs.close();
	fs.setstate(std::ios_base::goodbit);
	return wasState;
}
