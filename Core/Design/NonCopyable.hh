#ifndef Protium_NonCopyable_hh_
#define Protium_NonCopyable_hh_

namespace Protium{
	namespace Design{

		class NonCopyable {
		protected:
			NonCopyable(void) {}
		private:
			NonCopyable(const NonCopyable&);
			NonCopyable& operator=(const NonCopyable&);
		};
	}
}

#endif //File Guardian