//Client Module
//spawns jobs

#include "Client.h"

Define_Module(Client);

//handles the first "beep"
void Client::initialize()
{
    IAT = par("IATmean").doubleValue();
    beep = new cMessage("beep");
    if(par("isConstant").boolValue())
        delay = IAT;
    else
        delay = exponential(IAT);
    scheduleAt(simTime() + delay, beep);
}

//each time a msg is received (from itself) it sends out a message
void Client::handleMessage(cMessage *msg)
{
    send(new cMessage("request"), "out");
    generateJob();
}

//schedules a self message so simulate iat
void Client::generateJob()
{
    if(!par("isConstant").boolValue())
            delay = exponential(IAT);
    EV << "Client extracts: " << delay << endl;
    scheduleAt(simTime() + delay, beep);             //self message the allows looping
}
