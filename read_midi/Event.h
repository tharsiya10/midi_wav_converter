#ifndef EVENT
#define EVENT

#include "read_bytes.h"
#include "Data.h"
using namespace std;

class Event {

    protected :
        int64_t v_time ;

    public :
        Event(int64_t v_time);
        
        virtual ostream & print(ostream &out);
        friend ostream & operator << (ostream &out, Event &e);
        // ========== getters ============
        int64_t getVTime() { return v_time; }
};
#endif