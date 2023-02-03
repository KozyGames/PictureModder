

#include "BitmapInterface.h"
#include "Exception_Handling.h"

using namespace Kozy;


ScanLine::ScanLine(unsigned lineWidth)noexcept: 
sLine(new Pixel_24[lineWidth]){
	
}
ScanLine::ScanLine(unsigned lineWidth, unsigned value)noexcept:
sLine(new Pixel_24[lineWidth]) {

    // there is a c-function that creates a memory block with an initialization value, but I do not remember the name right now and also this is possibly easier to read
    for (unsigned long index = 0; index != lineWidth; ++index)
        sLine[index].setRGB(value);
}
ScanLine::ScanLine(unsigned lineWidth, const unsigned(&arr)[3])noexcept:
sLine(new Pixel_24[lineWidth]) {
   
    for (unsigned long index = 0; index != lineWidth; ++index)
        sLine[index].setRGB(arr);
}
ScanLine::ScanLine(const ScanLine& cpy, unsigned width)
:sLine(new Pixel_24[width]){

    for(unsigned index=0; index!=width;++index)
        sLine[index] = cpy.sLine[index];
}

ScanLine::ScanLine(ScanLine&& mv)noexcept
:sLine(mv.sLine){
    mv.sLine=nullptr;
}

ScanLine::ScanLine()noexcept
:sLine(nullptr){
    
}

Pixel_24& ScanLine::operator[](unsigned long long index){
    return sLine[index];
}
const Pixel_24& ScanLine::operator[](unsigned long long index)const{
    return sLine[index];
}
ScanLine& ScanLine::operator=(ScanLine&& mv)noexcept{
    if(sLine!=nullptr)
        delete[] sLine;
    sLine=mv.sLine;

    mv.sLine=nullptr;

    return *this;
}
ScanLine& ScanLine::copy(const ScanLine& cpy, unsigned long width){
    delete[] sLine;
    sLine=new Pixel_24[width];
    for (unsigned long index = 0; index != width; ++index)
        sLine[index] = cpy.sLine[index];

    return *this;
}


bool ScanLine::compare(const ScanLine& rhs, unsigned width)const noexcept{
    for(unsigned pos{0};pos!=width;++pos){
        if(sLine[pos]!=rhs.sLine[pos])
            return false;
    }
    return true;
}


//BitmapInterface

BitmapInterface::BitmapInterface()noexcept : res(0,0), pixLines(nullptr){

}
BitmapInterface::BitmapInterface(const Resolution& r, unsigned value)noexcept: 
    res(r),
    pixLines(new ScanLine[r.height]){

    const ScanLine pL(r.width, value);
    for (unsigned long line(0); line != r.height; ++line)
        pixLines[line].copy(pL, r.width);
}
BitmapInterface::BitmapInterface(const Resolution& r,const unsigned (&arr)[3])noexcept: 
    res(r),
    pixLines(new ScanLine[r.height]) {

    const ScanLine pL(r.width, arr);
    for (unsigned long line(0); line != r.height; ++line)
        pixLines[line].copy(pL, r.width);
}


BitmapInterface& BitmapInterface::operator=(const BitmapInterface& cpy){
    orientation=cpy.orientation;
    res = cpy.res;
    delete[] pixLines;
    pixLines = new ScanLine[res.height];
    for(unsigned index{0};index != res.height;++index)
        pixLines[index].copy(cpy.pixLines[index],res.width);
    return *this;
}
BitmapInterface& BitmapInterface::operator=(BitmapInterface&& mv)noexcept{
    orientation=mv.orientation;

    res = mv.res;
    mv.res = {0,0};

    delete[] pixLines;
    pixLines = mv.pixLines;
    mv.pixLines = nullptr;

    return *this;
}

ScanLine& BitmapInterface::operator[](unsigned long index){
    if(index>=res.height){
        throw Out_Of_Range_obj(
            "Out of Bounds Error. \nYou tried to access a Scanline that does not exist!",
            "Check the height of your picture before accessing a Scanline."
        );

        if(res.height==0)
            throw Runtime_Error_obj(
                "Bad Access! The picture is Empty!",
                ""
            );
        return pixLines[0];
    }
    return pixLines[index];
}
const ScanLine& BitmapInterface::operator[](unsigned long index)const{
    if(index>=res.height){
        throw Out_Of_Range_obj(
            "Out of Bounds Error. \nYou tried to access a Scanline that does not exist!",
            "Check the height of your picture before accessing a Scanline."
        );

        if(res.height==0)
            throw Runtime_Error_obj(
                "Bad Access! The picture is Empty!",
                ""
            );
        return pixLines[0];
    }
    return pixLines[index];

}

bool BitmapInterface::operator==(const BitmapInterface& rhs)const{
    if(res.width==rhs.res.width) {
        if(res.height==0&&rhs.res.height==0||res.width==0) return true;
        const unsigned long smaller = (res.height < rhs.res.height) ? (res.height) : (rhs.res.height);

        if(orientation==rhs.orientation){
                for(unsigned pos{0}; pos!=smaller;++pos)
                    if(!pixLines[pos].compare(rhs.pixLines[pos],res.width))
                        return false;

    }else{

        for(unsigned pos1{0},pos2{rhs.res.height-1}; pos1!=smaller;++pos1,--pos2)
            if(!pixLines[pos1].compare(rhs.pixLines[pos2],res.width))
                return false;
        
    }

    } else
        return false;
    return true;
}
bool BitmapInterface::exactlySame(const BitmapInterface& rhs)const{
    if(res.width==rhs.res.width&&res.height==rhs.res.height){
        if(res.height==0||res.width==0) return true;

        if(orientation==rhs.orientation){
            for(unsigned pos{0}; pos!=res.height;++pos)
                if(!pixLines[pos].compare(rhs.pixLines[pos],res.width))
                    return false;

    }else{

        for(unsigned pos1{0},pos2{res.height-1}; pos1!=res.height;++pos1,--pos2)
            if(!pixLines[pos1].compare(rhs.pixLines[pos2],res.width))
                return false;
    }

    } else
        return false;
    return true;
}


void BitmapInterface::setPictureOrientation(bool b)noexcept {
    orientation = b;

}
