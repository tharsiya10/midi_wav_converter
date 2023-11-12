
#include "read_bytes.h"
#include "vector"
using namespace std;

class MTrkEvent {

    private :
        int delta_time ;
        // event
        // vector<MetaEvent *> *list_events;

    public :
        MTrkEvent(uint8_t *data);

        // =========== getters ===========
        long getDeltaTime() const { return delta_time; }
};