#ifndef __PROJECT_SERVER_H_
#define __PROJECT_SERVER_H_

#include <omnetpp.h>
#include <queue>
#include <stdlib.h>

class Server : public cSimpleModule
{
    private:
        //local variables
        double delay;
        double vacationMean;
        double serviceTime;
        double lastEventTime;           //keeps trace of the last event (either a push of a pop operation)
        double temp;
        double totalSum;                //used to compute the time weighted number of jobs in the system
        double waitingQueueTemp;        //keeps trace or the waited time of each job
        double vacationReturn;          //holds the time in which server returns back from vacation
        std::queue<double> *myqueue;    //queue in with iat are stored
        bool isOnService;               //when true the server is serving
        bool isOnVacation;              //when true the server is on vacation

        //messages
        cMessage *serviceExpired;
        cMessage *vacationExpired;


        //signals for statistics
        simsignal_t stateOfSystem;
        simsignal_t waitingQueue;
        simsignal_t respTime;
        simsignal_t idleT;
        simsignal_t jobInVacation;
        simsignal_t empty;
        simsignal_t weightedSum;

        //functions
        void onService();           //handles server's services
        void onVacation();          //handles vacation
        void checkVacation();       //check if the queue is empty as the server returns from a vacation
        void handleJob();           //serves the job

    protected:
        virtual void initialize();
        virtual void finish();                      //overwritten for statistics purposes
        virtual void handleMessage(cMessage *msg);
};

#endif
