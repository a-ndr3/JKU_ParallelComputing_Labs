#include <iostream>
#include <vector>
#include <chrono>
#include <execution>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <thread>
#include <future>
#include <atomic>
#include <string>

int segmented_sieve(int low, int high)
{

	int limit = floor(sqrt(high)) + 1;

	std::vector<bool> prime(limit + 1, true);
	std::vector<bool> segment(high - low + 1, true);

	int local_sum = 0;

	for (int p = 2; p * p <= limit; p++)
	{
		if (prime[p])
		{
			for (int i = p * 2; i <= limit; i += p)
				prime[i] = false;
		}
	}

	for (int p = 2; p <= limit; p++)
	{
		if (prime[p]) {
			int lowLim = std::max(p * p, (low + p - 1) / p * p);
			for (int i = lowLim; i <= high; i += p)
				segment[i - low] = false;
		}
	}

	for (int i = 0; i < segment.size(); i++)
		if (segment[i] && low + i != 0 && low + i != 1)
			local_sum++;

	return local_sum;
}

struct Segment {
	int low;
	int high;
};

int main()
{
	int n = 2000000000;
	auto str = std::to_string(n);
	for (int i = str.length() - 3; i > 0; i -= 3) {
		str.insert(i, "_");
	}

	std::cout << "Counting from 0 to " << str << std::endl;

	int segment_size = std::sqrt(n);

#pragma region futures
	std::vector<std::future<int>> futures;
	int total_futures_sum = 0;
	
	auto t_start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i += segment_size)
	{
		futures.push_back(std::async(std::launch::async, segmented_sieve, i, std::min(i + segment_size, n)));
	}
	for (auto& f : futures)
		total_futures_sum += f.get();
	auto t_end = std::chrono::high_resolution_clock::now();

	std::cout << "FUTURE: Execution Time(seconds): " << std::chrono::duration<double>(t_end - t_start).count() << std::endl;
	std::cout << "FUTURE: Number of primes: " << total_futures_sum << std::endl;
#pragma endregion

#pragma region c17
	std::vector<Segment> segments;

	for (int i = 0; i < n; i += segment_size) {
		segments.push_back({ i, std::min(i + segment_size, n) });
	}
	t_start = std::chrono::high_resolution_clock::now();
	auto total = std::transform_reduce(std::execution::par, segments.begin(), segments.end(), 0,
		std::plus<>(),
		[](const Segment& s) {
			return segmented_sieve(s.low, s.high);
		}
	);
	t_end = std::chrono::high_resolution_clock::now();

	std::cout << "PAR: Execution Time(seconds): " << std::chrono::duration<double>(t_end - t_start).count() << std::endl;
	std::cout << "PAR: Number of primes: " << total << std::endl;
#pragma endregion

#pragma region sequential
	std::vector<int> numbers(std::sqrt(n));
	std::iota(numbers.begin(), numbers.end(), 1);

	std::vector<bool> prime(n + 1, true);
	prime[0] = prime[1] = false;

	t_start = std::chrono::high_resolution_clock::now();
	for (auto p = 2; p * p <= n; p++)
	{
		if (prime[p])
		{
			for (auto i = p * p; i <= n; i += p)
				prime[i] = false;
		}
	}
	int prime_count = std::count(prime.begin(), prime.end(), true);
	t_end = std::chrono::high_resolution_clock::now();

	std::cout << "Execution Time(seconds): " << std::chrono::duration<double>(t_end - t_start).count() << std::endl;
	std::cout << "Number of primes: " << prime_count << std::endl;
#pragma endregion

	return 0;
}