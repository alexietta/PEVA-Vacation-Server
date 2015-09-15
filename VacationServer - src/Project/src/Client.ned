package project;

simple Client
{
    parameters:
        bool isConstant = default(true);		//if true the IAT is constant
        double IATmean;
    gates:
        output out;
}
