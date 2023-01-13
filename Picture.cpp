

#include "Picture.h"
#include "Format.h"
#include "FileParsing.h"
#include "Exception_Handling.h"


using namespace Kozy;


Picture::Picture(): BitmapInterface(), format(BLANKFORMAT) {
	setState(State::Good); 
}
Picture::Picture(const char* fileName): BitmapInterface(), format(getFormatByFileName(fileName)) {
    setState(State::Good); 

   FileParsing::load_Picture_BMP(this,fileName);
}

Picture::Picture(const Picture& cpy){
    if(!doesStateContain(State::Fatal_Error)&&!cpy.doesStateContainAny(State::Failed_Loading_AND_Fatal_Error)){
        BitmapInterface::operator=(cpy);
        extraCheckForIsValid=cpy.extraCheckForIsValid;
        state=cpy.state;
        format=cpy.format;
    }

    else{
       if(!isValid()&&!cpy.isValid())
        throw Runtime_Error_obj(
            "Both pictures experienced a fatal error and are unsafe to work with!",
            "Check both pictures for any big mistakes. This state indicates fatal behavior!"
        );
       else
            throw Runtime_Error_obj(
            (!isValid()) ? "The to-be-copied-to-picture is in a fatal error state!":"The to-be-copied-from-picture is in a fatal state!",
            "Check the picture for any big mistakes. This state indicates fatal behavior!"
        );
       
    }
    
}
Picture::Picture(Picture&& mv){
     if(!doesStateContain(State::Fatal_Error)&&!mv.doesStateContainAny(State::Failed_Loading_AND_Fatal_Error)){
        BitmapInterface::operator=(std::move(mv));
        extraCheckForIsValid=mv.extraCheckForIsValid;
        state=mv.state;
        format=std::move(mv.format);
        mv.enableState(State::Fatal_Error);
    }

    else{
       if(!isValid()&&!mv.isValid())
        throw Runtime_Error_obj(
            "Both pictures experienced a fatal error and are unsafe to work with!",
            "Check both pictures for any big mistakes. This state indicates fatal behavior!"
        );
       else
            throw Runtime_Error_obj(
            (!isValid()) ? "The to-be-copied-to-picture is in a fatal error state!":"The to-be-copied-from-picture is in a fatal state!",
            "Check the picture for any big mistakes. This state indicates fatal behavior!"
        );
       
    }
    
}


Picture::~Picture(){

}

Picture& Picture::operator=(const Picture& cpy){
     if(!doesStateContain(State::Fatal_Error)&&!cpy.doesStateContainAny(State::Failed_Loading_AND_Fatal_Error)){
        BitmapInterface::operator=(cpy);
        extraCheckForIsValid=cpy.extraCheckForIsValid;
        state=cpy.state;
        format=cpy.format;
    }

    else{
       if(!isValid()&&!cpy.isValid())
        throw Runtime_Error_obj(
            "Both pictures experienced a fatal error and are unsafe to work with!",
            "Check both pictures for any big mistakes. This state indicates fatal behavior!"
        );
       else
            throw Runtime_Error_obj(
            (!isValid()) ? "The to-be-copied-to-picture is in a fatal error state!":"The to-be-copied-from-picture is in a fatal state!",
            "Check the picture for any big mistakes. This state indicates fatal behavior!"
        );
       
    }
}
Picture& Picture::operator=(Picture&& mv){
     if(!doesStateContain(State::Fatal_Error)&&!mv.doesStateContainAny(State::Failed_Loading_AND_Fatal_Error)){
        BitmapInterface::operator=(std::move(mv));
        extraCheckForIsValid=mv.extraCheckForIsValid;
        state=mv.state;
        format=std::move(mv.format);
        mv.enableState(State::Fatal_Error);
    }

    else{
       if(!isValid()&&!mv.isValid())
        throw Runtime_Error_obj(
            "Both pictures experienced a fatal error and are unsafe to work with!",
            "Check both pictures for any big mistakes. This state indicates fatal behavior!"
        );
       else
            throw Runtime_Error_obj(
            (!isValid()) ? "The to-be-copied-to-picture is in a fatal error state!":"The to-be-copied-from-picture is in a fatal state!",
            "Check the picture for any big mistakes. This state indicates fatal behavior!"
        );
       
    }
    
}


bool Picture::isValid()const {
    if(extraCheckForIsValid && !extraCheckForIsValid()) 
        return false;

    return 
        format.isValid() 
        && state == static_cast<unsigned>(State::Good) //we could have just checked for state == 0, but this is as efficient(done at compile time) and easier to understand, as in we do not need to look at the header to understand that line
;}


const Format& Picture::getFormat() const {
    return format;
}
