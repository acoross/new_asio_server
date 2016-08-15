//
//  server.hpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef server_h
#define server_h

namespace acoross {
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

		class UserSession : public std::enable_shared_from_this<UserSession> {
		public:
			UserSession(tcp::socket&& socket)
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
		using UserSessionSP = std::shared_ptr<UserSession>;



		class Server {
		public:
			using tcp = asio::ip::tcp;

			Server(asio::io_service& ios, int port)
			: acceptor_(ios, tcp::endpoint(tcp::v4(), port)), socket_(ios) {}

			void start() {
				printf("start!\n");
				do_accept();
			}

		private:
			void do_accept() {
				acceptor_.async_accept(socket_,
									   [this](std::error_code ec) {
										   if (!ec) {
											   printf("accepted!\n");

//											   auto us = std::make_shared<UserSession>(std::move(socket_));
//											   us->start();

											   auto s = std::move(socket_);

											   char buf[1000]{0,};
											   s.receive(asio::buffer(buf, 1000));
											   
											   printf("got: %s\n", buf);

											   s.send(asio::buffer(buf, 1000));
										   }
										   do_accept();
									   });
			}

			tcp::acceptor acceptor_;
			tcp::socket socket_;
		};
		
	}
}

#endif /* server_h */
