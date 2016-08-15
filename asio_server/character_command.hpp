//
//  character_command.h
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef character_command_hpp
#define character_command_hpp

namespace acoross {
	namespace action_game {
		
		class CharacterCommand
		{
		public:
			virtual void Do() = 0;
		};

	}
}

#endif /* character_command_hpp */
