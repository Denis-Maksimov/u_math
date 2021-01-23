#include <iostream>

using namespace std;

class pid_r
{
private:
    double prew;
    double sum;
public:
    pid_r(double need);
    ~pid_r();

    double needed;

    double kp;
    double ki;
    double kd;

    

    double step(double x){

        double delta= needed-x;
        double p=delta*kp;
        sum+=delta;
        double i=sum*ki;
        double d=kd*(delta-prew);

        prew=delta;
        // cout <<"for x="<< x<< ": "<< p+i+d <<"\n";
        return p+i+d;


    }
};

pid_r::pid_r(double need)
{
    needed=need;
    kp=0.01;
    ki=0.;
    kd=0.;

    prew=0;
    sum=0;
}

pid_r::~pid_r()
{
}

    
#include <math.h>
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char const *argv[])
{
    pid_r a(30.5);
    double z=0;
    for (size_t i = 0; i < 250; i++)
    {
        z+=a.step(sin(z)*z);

        cout << z << "="<<sin(z)*z<<":"<<i<<"\n";
       
    }
    
    return 0;
}


