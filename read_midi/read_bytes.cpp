#include "read_bytes.h"
using namespace std;

uint16_t read_2bytes(uint8_t *bytes) {
    
    return bytes[0] << 8u | bytes[1];
}

uint32_t read_3bytes(uint8_t *bytes) {
    return bytes[0] << 16u | bytes[1] << 8u | bytes[2];
}

uint32_t read_4bytes(uint8_t *bytes) {
    
    return bytes[0] << 24u | bytes[1] << 16u | bytes[2] << 8u | bytes[3];
}

// read variable length quantity for time division
uint64_t read_var_length(uint8_t *bytes, int *nb_bytes) {
    uint64_t value = 0;
    int i = *nb_bytes;
    while(1){
        value = (value << 7) | (bytes[i] & 0x7F);
        if(!(bytes[i] & 0x80)) {
            break;
        }
        i++;
    }
    *nb_bytes = i+1;
    return value;
}

uint32_t read_delta_time(uint8_t *bytes, int *nb_bytes){
    uint8_t v = 0;
    uint32_t value = 0;
    while(1) {
        (*nb_bytes) += 1;
        v = bytes[(*nb_bytes) - 1];
        value = (value << 7) | (v & 0x7F);
        if((v & 0x80) != 1) break;
    }
    return value;
}

char* read_string(uint8_t *bytes, int length) {
    char *res = (char *) malloc(sizeof(char) * length);
    for(int i = 0; i < length; i++){
        res[i] = bytes[i];
    }
    return res;
}

