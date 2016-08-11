//
//  main.cpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 11..
//  Copyright © 2016년 acoross. All rights reserved.
//

#include <iostream>
#include <thread>
#include <map>
#include <atomic>

#define ASIO_STANDALONE
#include "asio.hpp"

template <size_t ThreadCount>
class ThreadPool {
public:
	constexpr int get_thread_count() const {
		return ThreadCount;
	}

	int get_this_thread_num() {
		return th_nums[std::this_thread::get_id()];
	}

	void set_this_thread_num(int n) {
		while(spinlock.test_and_set())
			;

		th_nums[std::this_thread::get_id()] = n;

		spinlock.clear();
	}

	template <class FuncT>
	void start(const FuncT& func){
		int i = 0;
		for (auto& t : th){
			t = std::thread([&](int n)
							{
								set_this_thread_num(n);
								func();
							}, i++);
		}
	}

	void join_all() {
		for (auto& t : th) {
			t.join();
		}
	}

private:
	std::atomic_flag spinlock;
	std::vector<std::thread> th{ThreadCount};
	std::map<std::thread::id, int> th_nums;
};

template <size_t ThreadCount>
void print_this_thread_num(ThreadPool<ThreadCount>& th_pool) {
	std::cout << th_pool.get_this_thread_num() << std::endl;
}

int main(int argc, const char * argv[]) {

	ThreadPool<6> th_pool;
	std::cout << th_pool.get_thread_count() << std::endl;

	asio::io_service io_service;

	auto func = [&](){
		print_this_thread_num(th_pool);

		io_service.post([&](){
			print_this_thread_num(th_pool);
		});

		io_service.dispatch([&](){
			std::cout << 9 << std::endl;
		});

		print_this_thread_num(th_pool);
	};

	io_service.post(func);

	std::cout << "start thread pool\n";
	th_pool.start([&](){
		io_service.run();
	});

	th_pool.join_all();
	std::cout << "end thread pool\n";

    return 0;
}
