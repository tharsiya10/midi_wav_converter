#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>
using namespace std;

uint16_t read_2bytes(uint8_t *bytes);
uint32_t read_3bytes(uint8_t *bytes);
uint32_t read_4bytes(uint8_t *bytes);
uint64_t read_var_length(uint8_t *bytes, int *nb_bytes);
uint32_t read_delta_time(uint8_t *bytes, int *nb_bytes);
char* read_string(uint8_t *bytes, int length);