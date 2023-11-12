#include "MTrkEvent.h"
using namespace std;

MTrkEvent::MTrkEvent(uint8_t *data) {
    // list_events = new vector<MTrkEvent *>();

    int nb_bytes = 0;
    delta_time = read_delta_time(data, &nb_bytes);
}

