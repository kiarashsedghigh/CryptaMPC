//ICommChannel.h
#ifndef I_COMM_CHANNEL_H
#define I_COMM_CHANNEL_H
#include "config.h"

#include <string>

class ICommChannel {
public:
    virtual ~ICommChannel() = default; 
    // Send a msg to a specific party ID
    virtual void send(TYPE_PARTY_ID toPartyId, const TYPE_MESSAGE& msg) = 0;
    // Receive a msg from a specific party ID
    virtual void receive(TYPE_PARTY_ID fromPartyId, TYPE_PARTY_ID myPartyId, TYPE_MESSAGE& msg) = 0;
}

#endif // I_COMM_CHANNEL_H