#ifndef PICTUREPROJECT_H
#define PICTUREPROJECT_H

/*
collects Pictures and hints on how to use them. Like transparancy and transformations.

Can create a new picture. This is done in a new thread each time; locks a picture while doing so.
*/

/*
this is a stud
*/
#include "Exception_Handling.h"
namespace Kozy{
class PictureProject{
    public:
    //TEMP
    PictureProject(){
        throw Runtime_Error_obj(
            "Pictureproject has been tried to instanciate.",
            "Please do not use this object.\nPictureProjects are not implemented yet."
        );
    }
    bool isWorking()const noexcept
        {return working;}
    private:
    bool working{false};
    

};

}
#endif