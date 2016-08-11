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

template <size_t ThreadCount>
void print_this_thread_num(acoross::ThreadPool<ThreadCount>& th_pool) {
	std::cout << th_pool.get_this_thread_num() << std::endl;
}


namespace acoross {
	namespace util {
		using UID_t = int64_t;

		class UniqueIdGenerator {
		public:
			UID_t Generate() {
				return ++uid_;
			}
		private:
			std::atomic<UID_t> uid_{0};
		};
	}

	namespace action_game {

		class User {

		};

		class Game {
		public:
			Game(asio::io_service& ios)
			: ios_(ios)
			{
			}

		private:
			asio::io_service& ios_;
			util::UniqueIdGenerator uid_gen_;
		};
	}

	namespace network {

		using asio::ip::tcp;

		class packet {
		public:
			static const int header_length = 12;
			char* data(){return nullptr;}
			bool decode_header(){return false;}
			size_t body_length(){return 0;}
			char* body(){return nullptr;}
			size_t length(){return 0;}
		};

		class UserSession
		: public std::enable_shared_from_this<UserSession> {
		public:
			UserSession(tcp::socket socket)
			: socket_(std::move(socket)) {
			}

			void start() {
				do_read_header();
			}

			void deliver(const packet& msg) {
				bool write_in_progress = !write_pacs_.empty();
				write_pacs_.push_back(msg);
				if (!write_in_progress)
				{
					do_write();
				}
			}

		private:
			void do_read_header() {
				auto self(shared_from_this());
				asio::async_read(socket_,
								 asio::buffer(read_pac_.data(), packet::header_length),
								 [this, self](std::error_code ec, std::size_t /*length*/) {
									 if (!ec && read_pac_.decode_header()) {
										 do_read_body();
									 } else {
									 }
								 });
			}

			void do_read_body() {
				auto self(shared_from_this());
				asio::async_read(socket_,
								 asio::buffer(read_pac_.body(), read_pac_.body_length()),
								 [this, self](std::error_code ec, std::size_t /*length*/) {
									 if (!ec) {

										 //TODO: packet 처리

										 do_read_header();
									 } else {
									 }
								 });
			}

			void do_write() {
				auto self(shared_from_this());
				asio::async_write(socket_,
								  asio::buffer(write_pacs_.front().data(),
											   write_pacs_.front().length()),
								  [this, self](std::error_code ec, std::size_t /*length*/) {
									  if (!ec) {
										  write_pacs_.pop_front();
										  if (!write_pacs_.empty()) {
											  do_write();
										  }
									  } else {
									  }
								  });
			}

			tcp::socket socket_;
			packet read_pac_;
			typedef std::deque<packet> packet_queue;
			packet_queue write_pacs_;
		};

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
}


int main(int argc, const char * argv[]) {
	using namespace acoross;

	ThreadPool<6> th_pool;
	asio::io_service io_service;

	network::Server server(io_service);
	server.start();

	std::cout << "start thread pool\n";
	th_pool.start([&](){
		io_service.run();
	});
	th_pool.join_all();
	std::cout << "end thread pool\n";

    return 0;
}
