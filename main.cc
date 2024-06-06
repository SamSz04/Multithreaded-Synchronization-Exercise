#include "waiting_once_api.hpp"
#include <cassert>
#include <thread>
#include <chrono>
#include <vector>

void test_call_once_wo()
{
	std::atomic<int> cnt_accu = 0, cnt_now = 0;
	waiting_once wo;
	std::jthread threads[8];
	for (auto &t : threads)
		t = std::jthread([&]() {
			wo.call_once_waiting([&]() {
				++cnt_now;
				++cnt_accu;
				assert(cnt_now == 1);
				--cnt_now;
			});
			assert(cnt_accu == 1);
			assert(cnt_now == 0);
		});
}

void test_call_once_owo()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    optimized_waiting_once owo;
    std::jthread threads[8];
    for (auto &t : threads)
        t = std::jthread([&]() {
            owo.call_once_waiting([&]() {
                ++cnt_now;
                ++cnt_accu;
                assert(cnt_now == 1);
                --cnt_now;
            });
            assert(cnt_accu == 1);
            assert(cnt_now == 0);
        });
}

void test_long_init_wo()
{
	std::atomic<int> cnt_accu = 0, cnt_now = 0;
	waiting_once wo;
	std::jthread threads[8];
	for (auto &t : threads)
		t = std::jthread([&]() {
			wo.call_once_waiting([&]() {
				++cnt_now;
				++cnt_accu;
				assert(cnt_now == 1);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				--cnt_now;
			});
			assert(cnt_accu == 1);
			assert(cnt_now == 0);
		});
}

void test_long_init_owo()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    optimized_waiting_once owo;
    std::jthread threads[8];
    for (auto &t : threads)
        t = std::jthread([&]() {
            owo.call_once_waiting([&]() {
                ++cnt_now;
                ++cnt_accu;
                assert(cnt_now == 1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                --cnt_now;
            });
            assert(cnt_accu == 1);
            assert(cnt_now == 0);
        });
}

#if __cpp_lib_print >= 202207L
#include <print>
void report_throughput(double seconds, long ops_per_second)
{
	std::print("Time: {}s.  Throughput: {} ops/s\n", seconds, ops_per_second);
}
#else
#include <iostream>
void report_throughput(double seconds, long ops_per_second)
{
    std::cout << "Time: " << seconds << "s.  Throughput: " << ops_per_second << " ops/s\n";
}
#endif

void test_throughput_wo(int times_per_thread, size_t threads_num)
{
	waiting_once wo;
	wo.call_once_waiting([]() {}); // init done
	auto t1 = std::chrono::system_clock::now();
	{
		std::vector<std::jthread> threads(threads_num);
		for (auto &t : threads)
			t = std::jthread([&](int times) {
				for (int i = 0; i < times; ++i)
					wo.call_once_waiting([]() {});
			}, times_per_thread);
	}
	auto t2 = std::chrono::system_clock::now();
	auto duration = t2 - t1;
	auto duration_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    	std::cout << "ThreadNum: " << threads_num << "    ";
	report_throughput(duration_seconds, times_per_thread * threads_num / duration_seconds);
}

void test_throughput_owo(int times_per_thread, size_t threads_num)
{
    optimized_waiting_once owo;
    owo.call_once_waiting([]() {}); // init done
    auto t1 = std::chrono::system_clock::now();
    {
        std::vector<std::jthread> threads(threads_num);
        for (auto &t : threads)
            t = std::jthread([&](int times) {
                for (int i = 0; i < times; ++i)
                    owo.call_once_waiting([]() {});
            }, times_per_thread);
    }
    auto t2 = std::chrono::system_clock::now();
    auto duration = t2 - t1;
    auto duration_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    std::cout << "ThreadNum: " << threads_num << "    ";
    report_throughput(duration_seconds, times_per_thread * threads_num / duration_seconds);
}

int main()
{
    std::cout << "Testing waiting_once:" << std::endl;
    test_call_once_wo();
	test_long_init_wo();

	test_throughput_wo(10000000, 1);
	test_throughput_wo(10000000, 2);
	test_throughput_wo(10000000, 4);
	test_throughput_wo(10000000, 8);

    std::cout << "Testing optimized_waiting_once:" << std::endl;
    test_call_once_owo();
    test_long_init_owo();

    test_throughput_owo(10000000, 1);
    test_throughput_owo(10000000, 2);
    test_throughput_owo(10000000, 4);
    test_throughput_owo(10000000, 8);
}
