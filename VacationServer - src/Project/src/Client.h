#ifndef __PROJECT_CLIENT_H_
#define __PROJECT_CLIENT_H_

#include <omnetpp.h>

class Client : public cSimpleModule
{
    private:
        double IAT;
        double delay;           //internal delay for spawning jobs
        cMessage *beep;

        //functions
        void generateJob();     //sends delayed self message

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
