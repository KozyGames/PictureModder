#ifndef PICTURE_ALGORITHM_H
#define PICTURE_ALGORITHM_H



namespace Kozy {

struct ScanLine;
struct BitmapInterface;


namespace Algorithm {

void changeAllPixel(ScanLine& line, unsigned long width, unsigned value);
void changeAllPixel(ScanLine& line, unsigned long width, unsigned (&arr)[3]);

void changeToColorFlair(BitmapInterface& basePic, unsigned value);
void changeToColorFlair(BitmapInterface& basePic, unsigned(&arr)[3]);

}
}





#endif