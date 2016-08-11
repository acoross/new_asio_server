//
//  thread_pool.hpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef thread_pool_h
#define thread_pool_h

#include <thread>
#include <map>
#include <atomic>

namespace acoross {

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

}

#endif /* thread_pool_h */
