package project;

//Module for the server
simple Server
{
    parameters:
        double vacationMean;							//mean of exponentially distributed vacation time
        bool constantServTime = default(true);			//if true, constant
        double servTimeMean; 							//mean of constant or exponential service time
        @statistic[NumOfJobs](source="NumOfJobs"; record=mean;);					//statistics over the number of jobs in the system (each single event, not time weighted)
        @statistic[weightedNumJobs](source="weightedNumJobs"; record=sum;);			//single mean number of jobs (weighted over time)
        @statistic[WaitInQueue](source="WaitInQueue"; record=vector;);				//for test purposes
		@statistic[ResponseTime](source="ResponseTime"; record=mean;);				// ---
        @statistic[IdleTime](source="IdleTime"; record=sum, count;);				//sum of all the time the server spent on vacation
        @statistic[jobDuringVacation](source="jobDuringVacation"; record=mean);		//for test purposes
        @statistic[emptyingTime](source="emptyingTime"; record=vector;);			//time needed to dispose all the queue developed during vacation
    gates:
        input in;
}

