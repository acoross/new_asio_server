//
//  main.cpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 11..
//  Copyright © 2016년 acoross. All rights reserved.
//

#include <iostream>
#include <memory>
#include <atomic>
#include <map>
#include <deque>

#define ASIO_STANDALONE
#include "asio.hpp"

#include "thread_pool.hpp"
#include "server.hpp"


template <size_t ThreadCount>
void print_this_thread_num(acoross::ThreadPool<ThreadCount>& th_pool) {
	std::cout << th_pool.get_this_thread_num() << std::endl;
}

int main(int argc, const char * argv[]) {
	using namespace acoross;

	ThreadPool<2> th_pool;
	asio::io_service io_service;

	network::Server server(io_service, 11000);
	server.start();

	std::cout << "start thread pool\n";
	th_pool.start([&](){
		printf("%d\n", th_pool.get_this_thread_num());
		io_service.run();
	});
	th_pool.join_all();
	std::cout << "end thread pool\n";

    return 0;
}
