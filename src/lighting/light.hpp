#pragma once

#include <cstdint> 
#include <iostream>

struct light {
    uint16_t value; 

    uint8_t get(int channel) const { return (value >> (4 * channel)) & 0xF; }
    uint8_t getR() const { return (value >> 0) & 0xF; }
    uint8_t getG() const { return (value >> 4) & 0xF; }
    uint8_t getB() const { return (value >> 8) & 0xF; }
    uint8_t getS() const { return (value >> 12) & 0xF; }

    void setR(uint8_t a) { value = (value & 0xFFF0) | (a & 0xF); }
    void setG(uint8_t b) { value = (value & 0xFF0F) | ((b & 0xF) << 4); }
    void setB(uint8_t c) { value = (value & 0xF0FF) | ((c & 0xF) << 8); }
    void setS(uint8_t d) { value = (value & 0x0FFF) | ((d & 0xF) << 12); }

    bool operator>(const light& other) const {
        return other.value > this->value;
    }
    bool operator<(const light& other) const {
        return other.value < this->value;
    }
    bool operator>=(const light& other) const {
        return other.value >= this->value;
    }
    bool operator<=(const light& other) const {
        return other.value <= this->value;
    }
};
