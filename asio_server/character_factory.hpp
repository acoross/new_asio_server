//
//  character_factory.h
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef character_factory_hpp
#define character_factory_hpp

#include <memory>

#include "util.hpp"
#include "character.hpp"

namespace acoross {
	namespace action_game {

		class CharacterFactory
		{
		public:
			auto& GetObjects() const {
				return char_map_;
			}

		private:
			util::UniqueIdGenerator uid_gen_;
			std::unordered_map<util::UID_t, CharacterSP> char_map_;
		};

	}
}
#endif /* character_factory_hpp */
