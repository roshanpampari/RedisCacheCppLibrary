#include <chrono>
#include "Cache.h"
#include <utility>


Cache::Cache()
{
	bperiodicCheck = true;
	t1 = std::thread(&Cache::periodicCheck, this);
}

Cache::~Cache()
{
	stopPeriodicCheck();
}

void Cache::set(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> lock(m_hmap);
    hmap[key] = rcValue(std::string(value)); // construct rcValue from a temporary string
}

void Cache::set(const std::string& key, const std::string& value, int TTL)
{
    std::lock_guard<std::mutex> lock(m_hmap);
    hmap[key] = rcValue(std::string(value), TTL); // construct rcValue with TTL
}

std::string Cache::get(const std::string& key)
{

	if (exists(key))
	{
		std::unique_lock<std::mutex> lock(m_hmap);

		return hmap[key].getValue();
	}
	else
		return std::string();
}

void Cache::removeKey(const std::string& key)
{
	if (exists(key))
	{
		std::unique_lock<std::mutex> lock(m_hmap);

		hmap.erase(key);
	}
}

bool Cache::exists(const std::string& key)
{

	std::unique_lock<std::mutex> lock(m_hmap);

	if (hmap.find(key) != hmap.end())
	{
		int cTTL = hmap[key].getTTL();
		auto timeLeft = unSafe_ttl(key);

		// if TTL is zero treat as no expiration
		if (cTTL == 0 || timeLeft > 0)
		{
			return true;
		}

		// Delete the expired keys
		hmap.erase(key);
	}
	return false;
}

double Cache::unSafe_ttl(const std::string& key)
{
	if (hmap.find(key) != hmap.end())
	{
		auto cTime = std::chrono::high_resolution_clock::now();
		auto insTime = hmap[key].getInsertTime();
		int cTTL = hmap[key].getTTL();

		if (cTTL == 0)
			return 1;

		// compute elapsed time in seconds and compare with TTL (assumed to be seconds)
		auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(cTime - insTime).count();

		return cTTL - timeElapsed;
	}	
	return -1;
}

double Cache::ttl(const std::string& key)
{
	std::unique_lock<std::mutex> lock(m_hmap);

	return unSafe_ttl(key);
}

void Cache::printKeys()
{
	std::unique_lock<std::mutex> lock(m_hmap);

	for (const auto& itr : hmap)
	{
		std::cout << itr.first << " : " << itr.second.getValue() << std::endl;
	}

	return;
}

void Cache::periodicCheck()
{
	while (bperiodicCheck)
	{
		std::unique_lock<std::mutex> lock(m_hmap);

		bool bStopPeriodicCheck = cv.wait_for(lock, std::chrono::seconds(5), [&] { return (!bperiodicCheck); });

		if (bStopPeriodicCheck)
			return;

			for (auto itr = hmap.begin(); itr != hmap.end();)
			{
				double timeLeft = unSafe_ttl(itr->first);

				if (timeLeft <= 0)
				{
					itr = hmap.erase(itr);	// erase() returns next valid iterator
				}
				else
					itr++;
			}
	}
}

void Cache::stopPeriodicCheck()
{
	bperiodicCheck = false;
	
	cv.notify_one();
	
	if(t1.joinable())
		t1.join();

	return;
}

