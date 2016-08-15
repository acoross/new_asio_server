//
//  game.hpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef game_hpp
#define game_hpp

#include <memory>
#include <atomic>
#include <unordered_map>
#include <functional>

#define ASIO_STANDALONE
#include "asio.hpp"

#include "util.hpp"
#include "task.hpp"
#include "character.hpp"
#include "character_factory.hpp"

namespace acoross {
	namespace action_game {
		
		class Game : public std::enable_shared_from_this<Game> {
		public:
			Game(asio::io_service& ios)
			: ios_(ios) {
			}

			// start loop
			void Start() {
				loop();
			}

		private:
			void loop(){
				main_loop_logic();

				auto thissp = shared_from_this();
				ios_.post([thissp]() {
					thissp->loop();
				});
			}

			// single thread logic inside loop
			void main_loop_logic() {
				// move all char
				for (auto& pair : char_map_.GetObjects()) {
					pair.second->Update();
				}
			}

			asio::io_service& ios_;
			CharacterFactory char_map_;
		};
		
	}
}
#endif /* game_hpp */
