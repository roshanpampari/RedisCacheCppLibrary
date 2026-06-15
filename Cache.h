#pragma once
#include "RedisCacheApi.h"
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include "rcValue.h"

class REDISCACHE_API Cache
{
public:

	Cache();

	~Cache();

	void set(const std::string& key, const std::string& value);

	void set(const std::string& key, const std::string& value, int TTL);

	std::string get(const std::string& key);

	void removeKey(const std::string& key);

	bool exists(const std::string& key);


	double ttl(const std::string& key);

	void printKeys();

private:

	double unSafe_ttl(const std::string& key);

	void periodicCheck();

	void stopPeriodicCheck();

private:
	std::unordered_map<std::string, rcValue> hmap;
	std::atomic<bool> bperiodicCheck;	// better synchronization btw threads
	std::thread t1;
	std::mutex m_hmap;
	std::condition_variable cv;
};