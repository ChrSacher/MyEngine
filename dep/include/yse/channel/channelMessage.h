/*
  ==============================================================================

    channelMessage.h
    Created: 23 Mar 2014 12:14:47pm
    Author:  yvan

  ==============================================================================
*/

#ifndef CHANNELMESSAGE_H_INCLUDED
#define CHANNELMESSAGE_H_INCLUDED

#include "channel.hpp"

namespace YSE {
  namespace CHANNEL {
    
    /*
       Message objects are used to send messages from interface to implementation. In this
       case, a message will be sent from a channelInterfaceObject to a
       channelImplementationObject. They are a way to ensure threadsafe and lockfree communication
       between the two.
    */
    class messageObject {
    public:
      /** The ID of a message defines how it will be stored in the implementation
      */
      MESSAGE ID;

      /** The data is stored in a union, so to not use more data as needed. Other types
      could be added to this union if needed, but they should not exeed the current
      length (Flt[3]). (Doing so won't be unsafe, but it will affect every message
      used in by all subSystems. So don't.)
      */
      union {
        Bool   boolValue;
        Flt    vecValue[3];
        Flt    floatValue;
        UInt   uintValue;
        void * ptrValue;
      };
    };
  }
}


#endif  // CHANNELMESSAGE_H_INCLUDED
