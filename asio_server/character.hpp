//
//  character.hpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef character_hpp
#define character_hpp

#include <memory>
#include <deque>

#include "util.hpp"
#include "task.hpp"
#include "character_command.hpp"

namespace acoross {
	namespace action_game {

		struct Transform {
			Transform(){};

			int x{0};
			int y{0};
			int z{0};
		};

		class Character
		: public Task
		{
		public:
			Character(util::UID_t id)
			: id_(id) {

			}

			Character(util::UID_t id, const Transform& tran)
			: id_(id), tran_(tran) {

			}

			virtual void Update() override {
				while (!command_queue_.empty()) {
					auto cmd = std::move(command_queue_.front());
					command_queue_.pop_front();

					if (cmd){
						cmd->Do();
					}
				}
			}

		private:
			const util::UID_t id_;
			Transform tran_;
			std::deque<std::unique_ptr<CharacterCommand>> command_queue_;
		};

		using TaskSP = std::shared_ptr<Task>;
		using CharacterSP = std::shared_ptr<Character>;

	}
}
#endif /* character_hpp */
