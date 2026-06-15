#pragma once
#include "RedisCacheApi.h"
#include <string>
#include <ctime>
#include <chrono>

class REDISCACHE_API rcValue
{

public:

	rcValue();

	~rcValue();

	rcValue(std::string&&, int = 0);

	rcValue(const rcValue& other);

	rcValue(rcValue&& other) noexcept;

	rcValue& operator=(const rcValue& other);

	rcValue& operator=(rcValue&& other) noexcept;

	const std::string getValue() const;

	int getTTL() const;

	std::chrono::time_point<std::chrono::high_resolution_clock> getInsertTime();

private:
	std::string m_value;
	int m_TTL;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_cTime;
};


