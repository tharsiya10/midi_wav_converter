#ifndef META
#define META

#include "Event.h"
using namespace std;

class MetaEvent : public Event {

    private :
        // struct track_data *data;
        uint8_t meta_type ;
        int64_t v_length;
        long event_length;

    public :
        MetaEvent(uint8_t* bytes, int64_t v_time, struct track_data *data);
        void extract_data(uint8_t *bytes, int *nb_bytes, struct track_data *data);

        ostream & print(ostream &out);

        // ============= getters ================
        uint8_t getMetaType() { return meta_type; }
        int64_t getVLength() { return v_length; }
        long getEventLength() { return event_length; }

};
#endif