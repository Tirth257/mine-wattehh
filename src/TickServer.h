#pragma once
#include"CONSTANTS.h"
class TickServer
{
private:

    chrono::duration<double> prevTime;
    chrono::duration<double> limit;
    public:

    // tick time should be in seconds
    TickServer(double limit){
        prevTime =chrono::duration<double>(0);
        this->limit = chrono::duration<double>(limit);
        
    }
    bool tickMe(chrono::duration<double> diff){
        if (prevTime.count() + diff.count() >= limit.count())
        {
        
            prevTime = prevTime - limit + diff;
            return true;
        }
        else
        {
            prevTime+=diff;
            return false;
        }
        
        
    }
    void Action(){
       // wt->Expend();
    }
};
 