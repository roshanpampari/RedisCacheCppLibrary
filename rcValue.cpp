#include "rcValue.h"

rcValue::rcValue() : m_TTL(0)
{
}

rcValue::~rcValue()
{}

rcValue::rcValue(std::string&& value, int TTL) : m_value(std::move(value)), m_TTL(0)
{
	m_cTime = std::chrono::high_resolution_clock::now();
}

rcValue::rcValue(const rcValue& other) = default;

rcValue::rcValue(rcValue&& other) noexcept = default;

rcValue& rcValue::operator=(const rcValue& other) = default;

rcValue& rcValue::operator=(rcValue&& other) noexcept = default;

const std::string rcValue::getValue() const
{
	return m_value;
}

int rcValue::getTTL() const
{
	return m_TTL;
}

std::chrono::time_point<std::chrono::high_resolution_clock> rcValue::getInsertTime()
{
	return m_cTime;
}
