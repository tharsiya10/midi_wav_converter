#include "Event.h"
using namespace std;

Event::Event(int64_t v_time) {
    this->v_time = v_time;
}

ostream & Event::print(ostream &out) {
    return out << "event" << endl;
}

ostream & operator << (ostream &out, Event &e) {
    return e.print(out);
}