#ifndef TYPE_DESCRIPTION_H
#define TYPE_DESCRIPTION_H

#include <string>
#include <vector>
#include <list>
#include <deque>

std::string type_description(double) { return std::string("double"); }
std::string type_description(long double) { return std::string("long double"); }
std::string type_description(float) { return std::string("float"); }
std::string type_description(uint8_t) { return std::string("uint8_t"); }
std::string type_description(uint16_t) { return std::string("uint16_t"); }
std::string type_description(uint32_t) { return std::string("uint32_t"); }
std::string type_description(uint64_t) { return std::string("uint64_t"); }
std::string type_description(int8_t) { return std::string("int8_t"); }
std::string type_description(int16_t) { return std::string("int16_t"); }
std::string type_description(int32_t) { return std::string("int32_t"); }
std::string type_description(int64_t) { return std::string("int64_t"); }

template <typename T, typename Alloc>
std::string type_description(const std::vector<T, Alloc>&) { return std::string("vector<") + type_description(T()) + std::string(">"); }

template <typename T, typename Alloc>
std::string type_description(const std::deque<T, Alloc>&) { return std::string("deque<") + type_description(T()) + std::string(">"); }

template <typename T, typename Alloc>
std::string type_description(const std::list<T, Alloc>&) { return std::string("list<") + type_description(T()) + std::string(">"); }
#endif
