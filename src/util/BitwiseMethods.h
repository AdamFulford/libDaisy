#ifndef __BITWISE_METHODS_H
#define __BITWISE_METHODS_H

#include <stdint.h>
#include <stdbool.h>

int bitwise_set_bit(int byte, int bit);
int bitwise_clear_bit(int byte, int bit);
int bitwise_clear_many(int byte, int value);
int bitwise_write_bit(int byte, int bit, bool state);
int bitwise_read_bit(int byte, int bit);
int bitwise_flip_bit(int byte, int bit);

uint16_t two8sTo16(int byte1, int byte2);
uint32_t two16sTo32(uint16_t byte1, uint16_t byte2);
void byte32to16(uint16_t *bytes, uint32_t byte32);

uint16_t bitwise_first_16_of_32(uint32_t value);
uint16_t bitwise_last_16_of_32(uint32_t value);

uint16_t convert12to16(int value);

uint32_t bitwise_slice(uint32_t value, uint8_t slice_start, uint8_t slice_length);

#endif