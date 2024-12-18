#pragma once
#ifndef COBS_CPP_H
#define COBS_CPP_H

#define MAX_PACKET_SIZE 254
#include <stdint.h>

namespace cobs {

inline uint8_t cobs_calcOverhead(const uint8_t* arr, const uint8_t& len, const uint8_t& START_BYTE)
{
	uint8_t overheadByte{};

	for (uint8_t i = 0; i < len; i++)
	{
		if (arr[i] == START_BYTE)
		{
			overheadByte = i;
            return overheadByte;
		}
	}

    //if not returned yet, return 0xFF (255)
    return 0xFF;
}

inline void cobs_stuffPacket(uint8_t* arr, uint8_t len, uint8_t START_BYTE)
{
    // Find last instance of START_BYTE
    int16_t refByte = -1; // Default to -1 (not found)

    for (int16_t i = len - 1; i >= 0; i--)
    {
        if (arr[i] == START_BYTE)
        {
            refByte = i;
            break; // Stop searching after finding the last instance
        }
    }

    // Process only if START_BYTE was found
    if (refByte != -1)
    {
        for (int16_t i = (len - 1); i >= 0; i--)
        {
            if (arr[i] == START_BYTE)
            {
                arr[i] = refByte - i;
                refByte = i;
            }
        }
    }
}

//this needs sorting - needs to handle length, do not use while loop
inline bool cobs_unpackPacket(uint8_t* arr, uint8_t OverheadByte, uint8_t START_BYTE, uint8_t length)
{
	uint8_t testIndex = OverheadByte;
	uint8_t delta     = 0;

    if(testIndex == 255)
    {
        return true;
    }

    else if(testIndex > length)
    {
        return false;
    }
    while (testIndex < length && arr[testIndex]!= 0)
    {
        delta          = arr[testIndex];
        arr[testIndex] = START_BYTE;
        testIndex += delta;

        if(testIndex >= length)
        {
            return false;
        }
    }

    if(testIndex < length)
    {
        arr[testIndex] = START_BYTE;
    }

    return true;

}

}   // Namespace cobs
#endif    // End of header