#ifndef SYS_EX
#define SYS_EX

#include "Event.h"
using namespace std;

class SysExEvent : public Event {

    private :
        int64_t v_length;
        long event_length;

    public :
        SysExEvent(uint8_t *bytes, int64_t v_time);

        ostream & print(ostream &out);

        // ============= getters =================
        long getEventLength() const { return event_length; }
};
#endif