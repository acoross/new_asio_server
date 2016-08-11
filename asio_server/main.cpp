//
//  main.cpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 11..
//  Copyright © 2016년 acoross. All rights reserved.
//

#include <iostream>
#include <memory>

#define ASIO_STANDALONE
#include "asio.hpp"

#include "thread_pool.hpp"

template <size_t ThreadCount>
void print_this_thread_num(acoross::ThreadPool<ThreadCount>& th_pool) {
	std::cout << th_pool.get_this_thread_num() << std::endl;
}


namespace acoross {

	class Server {
	public:
		using tcp = asio::ip::tcp;

		Server(asio::io_service& ios)
		: acceptor_(ios), socket_(ios) {}

		void start() {
			do_accept();
		}

	private:
		void do_accept() {
			acceptor_.async_accept(socket_,
								   [this](std::error_code ec) {
									   if (!ec) {
										   //todo something
									   }
									   do_accept();
								   });
		}

		tcp::acceptor acceptor_;
		tcp::socket socket_;
	};
}


int main(int argc, const char * argv[]) {
	using namespace acoross;

	ThreadPool<6> th_pool;
	asio::io_service io_service;

	acoross::Server server(io_service);
	server.start();

	std::cout << "start thread pool\n";
	th_pool.start([&](){
		io_service.run();
	});
	th_pool.join_all();
	std::cout << "end thread pool\n";

    return 0;
}
