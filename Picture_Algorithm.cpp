#include "Picture_Algorithms.h"
#include "BitmapInterface.h"


using namespace Kozy;

void Algorithm::changeAllPixel(ScanLine& line, unsigned long width, unsigned value){
	for (unsigned long pos(0); pos != width; ++pos)
		line[pos].setRGB(value);

	/*
	Previous Version:

	for (unsigned long pos(-1); pos != width; )
		line[++pos].setRGB(value);

	Why Does this lead to a heap crash??
	*/
}
void Algorithm::changeAllPixel(ScanLine& line, unsigned long width, unsigned(&arr)[3]){
	for (unsigned long pos(0); pos != width; ++pos)
		line[pos].setRGB(arr);
	
}

void Algorithm::changeToColorFlair(BitmapInterface& basePic, unsigned value) {
	for (unsigned long line(0); line < basePic.getRes().height; line += 2)
		changeAllPixel(basePic[line], basePic.getRes().width, value);

}
void Algorithm::changeToColorFlair(BitmapInterface& basePic, unsigned(&arr)[3]) {
	for (unsigned long line(0); line < basePic.getRes().height; line += 2)
		changeAllPixel(basePic[line], basePic.getRes().width, arr);
}