#include "BitwiseMethods.h"

/**
 * https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
 * 
 * The & (bitwise AND) The result of AND is 1 only if both bits are 1.
 * The | (bitwise OR) The result of OR is 1 if any of the two bits is 1.
 * The ^ (bitwise XOR) The result of XOR is 1 if the two bits are different.
 * The << (left shift) in C or C++ takes two numbers, left shifts the bits of the first operand, the second operand decides the number of places to shift.
 * The >> (right shift) in C or C++ takes two numbers, right shifts the bits of the first operand, the second operand decides the number of places to shift.
 * The ~ (bitwise NOT) in C or C++ takes one number and inverts all bits of it 
 * 
 * 
 * settings, clearing, toggling single bits --> https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
 * 
 * combine two 8 bit values into 16 bit value --> https://stackoverflow.com/questions/11193918/combine-merge-two-bytes-into-one/11193978
**/

/** Set bit index of byte to 1
 * @param byte target byte
 * @param bit index
*/ 
int bitwise_set_bit(int byte, int bit) {
  return (byte |= (1UL << bit));
}

int bitwise_clear_bit(int byte, int bit) {
  return (byte &= ~(1UL << bit));
}

/** @brief Clears all bits given a byte 
 * 0 means don't clear that bit, 1 means clear that bit
 * ex. byte == 0b00001100 will cleat bits 2 and 3
*/
int bitwise_clear_many(int byte, int value)
{
  return byte &= ~value;
}

/** write a given value to the selected bit index of a byte
 * @param byte 8/16/32 bit integer value
 * @param bit the bit / index to be set
 * @param state value to set. Either 1 or 0
 * @return bit status 1 or 0
*/
int bitwise_write_bit(int byte, int bit, bool state) {
  return (state ? bitwise_set_bit(byte, bit) : bitwise_clear_bit(byte, bit));
}

/** Return the status of a bit within a byte
 * @param byte 8/16/32 bit integer value
 * @param bit the bit / index to be read
 * @return bit status 1 or 0
*/ 
int bitwise_read_bit(int byte, int bit) {
  return (byte >> bit) & 0x01;
}

/**
 * @brief Invert / Flip a bit in a byte
 * 
 * @param byte 
 * @param bit 
 * @return int 
 */
int bitwise_flip_bit(int byte, int bit) {
  return bitwise_write_bit(byte, bit, !bitwise_read_bit(byte, bit));
}

/**
 * @brief merge two 8-bit values into a single 16-bit value
 * @param byte1 bits 7..0
 * @param byte2 bits 15..8
 */ 
uint16_t two8sTo16(int byte1, int byte2) {
  return (byte2 << 8) | byte1;
}

uint32_t two16sTo32(uint16_t byte1, uint16_t byte2) {
  return (byte2 << 16) | byte1;
}

void byte32to16(uint16_t *bytes, uint32_t byte32) {
  bytes[0] = (byte32 >> 16) & 0xffff;
  bytes[1] = byte32 & 0xffff;
}

/**
 * @brief returns the first half of a 32-bit value
 * 
 * @param value 
 * @return uint16_t 
 */
uint16_t bitwise_first_16_of_32(uint32_t value) {
  return value & 0xffff;
}

/**
 * @brief returns the last half of a 32-bit value
 *
 * @param value
 * @return uint16_t
 */
uint16_t bitwise_last_16_of_32(uint32_t value) {
  return (value >> 16) & 0xffff;
}

/**
 * @brief perform a 12-bit to 16-bit conversion
 * @param value 12-bit value to be converted
 * @return 16-bit value
*/
uint16_t convert12to16(int value) {
  return ((value << 4) & (uint16_t)0xFFF0) | ((value >> 8) & (uint16_t)0x000F);
}

/**
 * @brief return a slice of bytes within the given value
 *
 * @param value an 8-bit, 16-bit, or 32-bit value
 * @param slice_start the start bit of the slice
 * @param slice_length how many bits the slice contains
 * @return uint32_t
 */
uint32_t bitwise_slice(uint32_t value, uint8_t slice_start, uint8_t slice_length)
{
  return (value >> slice_start) & (~(0xFFFF << slice_length));
}