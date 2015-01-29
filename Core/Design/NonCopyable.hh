#ifndef ORCA_NonCopyable_hh_
#define ORCA_NonCopyable_hh_

namespace ORCA{
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