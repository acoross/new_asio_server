//
//  task.hpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef task_h
#define task_h

namespace acoross {
	namespace action_game {

		class Task
		{
		public:
			virtual ~Task(){}
			virtual void Update() = 0;
		};

	}
}

#endif /* task_h */
