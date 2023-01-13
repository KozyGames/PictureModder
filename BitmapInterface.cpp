

#include "BitmapInterface.h"

using namespace Kozy;


ScanLine::ScanLine(unsigned lineWidth){
	sLine = new Pixel_24[width];
}

ScanLine::ScanLine(const ScanLine& cpy, unsigned width)
:sLine(new Pixel_24[width]){
for(unsigned index=0; index!=width;++index){
        Pixel_24& pix=sLine[index];
        pix.red=cpy.sLine[index].red;
        pix.green=cpy.sLine[index].green;
        pix.blue=cpy.sLine[index].blue;
    }
}

ScanLine::ScanLine(ScanLine&& mv)
:sLine(mv.sLine){
    mv.sLine=nullptr;
}

ScanLine::ScanLine()
:sLine(nullptr){
    
}

Pixel_24* ScanLine::operator[](unsigned index){
    return sLine+index;
}

ScanLine& ScanLine::operator=(ScanLine&& mv){
    delete[] sLine;
    sLine=mv.sLine;

    mv.sLine=nullptr;

    return *this;
}
ScanLine& ScanLine::copy(const ScanLine& cpy, unsigned width){
    delete[] sLine;
    sLine=new Pixel_24[width];
    for(unsigned index=0; index!=width;++index){
        Pixel_24& pix=sLine[index];
        pix.red=cpy.sLine[index].red;
        pix.green=cpy.sLine[index].green;
        pix.blue=cpy.sLine[index].blue;
    }
    return *this;
}


bool ScanLine::compare(const ScanLine& rhs, unsigned width)const{
    for(unsigned pos{0};pos!=width;++pos){
        if(sLine[pos]!=rhs.sLine[pos])
            return false;
    }
    return true;
}
BitmapInterface::BitmapInterface(){

}

BitmapInterface& BitmapInterface::operator=(const BitmapInterface& cpy){
    orientation=cpy.orientation;
    res = cpy.res;
    delete[] pixLines;
    pixLines = new ScanLine[res.height];
    for(unsigned index{0};index != res.height;++index)
        pixLines[index].copy(cpy.pixLines[index],res.width);
    
}
BitmapInterface& BitmapInterface::operator=(BitmapInterface&& mv){
    orientation=mv.orientation;

    res = mv.res;
    mv.res = {0,0};

    delete[] pixLines;
    pixLines = mv.pixLines;
    mv.pixLines = nullptr;
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


void BitmapInterface::setPictureOrientation(const bool& b) {
    orientation=!b;
}
