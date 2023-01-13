#ifndef UTILITY_H
#define UTILITY_H

#include <iosfwd>


namespace Kozy{

namespace Utility{

/*
Global utility functions that are useful for Production and/or Debug

*/


/*
returns true if file with that exact name exists. Differentiates between for example "exampleFile.txt","example" and "exampleFile.png"
*/
bool fileExists(const char* fileName) noexcept;


/*
closes and resets correctly a stream object
returns the last state
*/
bool finishStream(std::ifstream& fs);
bool finishStream(std::ofstream& fs);



}


/*

used as a static class, although that concept is foreign to c,cpp
It is state-oriented and there will always only be one object of it.
The idea is that this is an Object that I and the user can ask for device information and possibly other useful methods.

One shall preferably access it by using the systemM variable and act like it is like any other OOP object 
*/
class SystemManager{
    public:
    enum class OS_VERSION: unsigned{
        win64=1, win32=2,
        apple=3, 
        linux=4, 
        android=5,
        UNKNOWN=0
    };

    /*
    Operating System MACROS
    */
    #ifdef _WIN64
    static constexpr OS_VERSION os=OS_VERSION::win64;
    #elif _WIN32    // defined for both 64 and 32
    static constexpr OS_VERSION os=OS_VERSION::win32;   
    #elif __APPLE__
    static constexpr OS_VERSION os=OS_VERSION::apple;
    #elif __linux__
    static constexpr OS_VERSION os=OS_VERSION::linux;
    #elif __ANDROID__
    static constexpr OS_VERSION os=OS_VERSION::android;
    #else
    static constexpr OS_VERSION os=OS_VERSION::UNKNOWN;
    #endif

    constexpr bool checkSystem()const noexcept
        {return systemHealth;}
    constexpr void printSystemInfo()const noexcept; 
    
    private:
    SystemManager();

    static bool systemHealth;
};
constexpr SystemManager systemM;


}


#endif