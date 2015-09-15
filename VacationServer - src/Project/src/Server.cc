#include "Server.h"
#include <queue>
#include <stdlib.h>

Define_Module(Server);

using namespace std;

void Server::initialize(){
    //initialization
    totalSum = 0;
    myqueue = new queue<double>();
    lastEventTime = 0;
    isOnService = false;
    isOnVacation = false;
    serviceExpired = new cMessage("serviceExpired");
    vacationExpired = new cMessage("vacationExpired");

    //fetch parameters from ini
    vacationMean = par("vacationMean").doubleValue();
    serviceTime = par("servTimeMean").doubleValue();

    //statistics
    stateOfSystem = registerSignal("NumOfJobs");
    waitingQueue = registerSignal("WaitInQueue");
    respTime = registerSignal("ResponseTime");
    idleT = registerSignal("IdleTime");
    jobInVacation = registerSignal("jobDuringVacation");
    empty = registerSignal("emptyingTime");
    weightedSum = registerSignal("weightedNumJobs");
    emit(stateOfSystem, myqueue->size());
}

void Server::handleMessage(cMessage *msg){
    //incoming message is from itself it's either a vacation or a service
    if(msg->isSelfMessage())
        //when returning from a service, proceeds to handle the eventually remaining jobs
        if(msg->isName("serviceExpired")){
            isOnService = false;
            temp = simTime().dbl() - lastEventTime;             //store the interevent time
            totalSum += temp*(myqueue->size()+1);               //sum it to the totalSum after weighting it for the queue size N(t)
            lastEventTime = simTime().dbl();                    //refresh last event record
            emit(stateOfSystem, myqueue->size());
            emit(respTime, simTime().dbl() - waitingQueueTemp);
            handleJob();                                        //process eventual jobs
        }
        //when returning from a vacation, checks the queue with checkVacation
        else{
            isOnVacation = false;
            emit(jobInVacation, myqueue->size());
            vacationReturn = simTime().dbl();
            checkVacation();
        }
    //incoming message is from client
    else{
        temp = simTime().dbl() - lastEventTime;                 //store the interevent time
        totalSum += temp * myqueue->size();                     //sum it to the totalSum after weighting it for the queue size N(t)
        myqueue->push( simTime().dbl() );                       //push incoming job into the queue
        lastEventTime = simTime().dbl();                        //refresh last event record
        EV << "Server after PUSH: " << myqueue->size() << endl;
        emit(stateOfSystem, myqueue->size());
        handleJob();                                            //process enqueued jobs
        delete(msg);
    }
}

//handles the service event
void Server::onService(){
    isOnService = true;
    bubble("Service");
    if(par("constantServTime").boolValue())                     //check everytime to refresh RV
        delay = serviceTime;
    else
        delay = exponential(serviceTime);
    EV << "Server: service time = " << delay << endl;
    scheduleAt(simTime() + delay, serviceExpired);
}

//handles the vacation event
void Server::onVacation(){
    isOnVacation = true;
    bubble("Vacation");
    delay = exponential(vacationMean);
    emit(idleT, delay);
    scheduleAt(simTime() + delay, vacationExpired);
}

//handles the jobs
void Server::handleJob(){
    //if server is busy return
    if(isOnService || isOnVacation)
        return;
    //if queue is empty server goes on vacation
    if(myqueue->empty())
        onVacation();
    //if queue not empty pops the first job from the queue and serves it
    else{
        EV << "Server before POP: " << myqueue->size() << endl;
        waitingQueueTemp = myqueue->front();                    //saves the job's time before popping it from the queue
        myqueue->pop();
        emit(waitingQueue, simTime() - waitingQueueTemp);
        if(myqueue->size() == 0)
            emit(empty, simTime() - vacationReturn);            //emptying time = time queue is empty - time the server went on vacation
        onService();                                            //serves the job
    }
}

//checks if the queue is empty
void Server::checkVacation(){
    //if empty server goes on vacation
    if(myqueue->empty()){
        onVacation();
    }
    //if not serves first job in the queue
    else{
        handleJob();
    }
}

//overwritten to compute the weighted sum of the number of jobs
void Server::finish(){
    emit(weightedSum,totalSum/simTime().dbl());                 //total weight of events/total simulation time
}

