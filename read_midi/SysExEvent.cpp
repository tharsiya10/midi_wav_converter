#include "SysExEvent.h"
using namespace std;

SysExEvent::SysExEvent(uint8_t* bytes, int64_t v_time) : Event(v_time) {
    int n = 0;
    v_length = read_var_length(bytes, &n);
    n += v_length;
    this->event_length = n;
}

ostream & SysExEvent::print(ostream &out) {
    out << "system-excl-event\n" << endl;
    return out;
}