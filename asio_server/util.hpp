//
//  util.hpp
//  asio_server
//
//  Created by 신일환 on 2016. 8. 12..
//  Copyright © 2016년 acoross. All rights reserved.
//

#ifndef util_h
#define util_h

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
}

#endif /* util_h */
