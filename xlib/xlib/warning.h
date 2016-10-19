#ifndef _WARNING_H
#define _WARNING_H

//#define _STRINGIFY(_x) #_x
//#define WARNING( _message ) __FILE__"(" _STRINGIFY(__LINE__) ") : warning: " _message

#define STRINGIFY(_text) #_text
#define TOSTRING(_text) STRINGIFY(_text)
#define WARNING( _message ) __FILE__"(" TOSTRING(__LINE__) ") : warning: " _message

#endif //_WARNING_H
