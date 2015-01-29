#ifndef Protium_STLAdapter_hh_
#define Protium_STLAdapter_hh_

#include "Protium/Allocation/SmallObjectAllocator.hh"


namespace Protium{
    namespace Allocation{

        //! Allocator to be used by STL containers

        /*!
            \param Host CRTP style template. Use like: `std::vector<MyClass, STLAdapter<MyClass> >`
            \param AllocType use if SmallObject uses custom Allocator
        */
        template<typename Host, typename AllocType = SmallObjectAllocator<> >
        class STLAdapter{
        public:
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;
            typedef Host* pointer;
            typedef const Host * const_pointer;
            typedef Host & reference;
            typedef const Host & const_reference;
            typedef Host value_type;
            typedef Host* hostPointer;
            typedef const Host* const_hostPointer;
            typedef Host& hostRef;
            typedef const Host& const_hostRef;

            //! Default constuctor does nothing
            inline STLAdapter() throw() {}

            //! Copy constructor utilizes nothing
            inline STLAdapter(const STLAdapter& ) throw() {}

            //! Templated copy constructor does nothing
            template<typename T>
            inline STLAdapter(const STLAdapter<T>& ) throw() {}

            //! Nothing to destruct
            inline ~STLAdapter() throw() {}

            //! Allows Adapters to use each other
            template<typename T>
            struct rebind{
                typedef STLAdapter<T> other;
            };

            //! Returns the address of the host object
            Host* address(Host& elem) const {return &elem;}

            //! Returns the const address of the host object
            const Host* address(const Host& elem) const {return &elem;}

            //! Allocates out a host object type.
            Host* allocate(std::size_t count, const void* hint=0){
                (void)hint;
                void* p = AllocType::Instance().Allocate(count*sizeof(Host),true );
                return reinterpret_cast<Host*>(p);
            }

            //! Deallocates host objects up to size
            void deallocate(Host* p, std::size_t size){
                AllocType::Instance().Deallocate(p, size* sizeof(Host));
            }

            //! Returns the maximum number of objects that can be allocated at once
            std::size_t max_size() const throw(){
                const std::size_t max = std::size_t(-1);
                const std::size_t bytes = max/sizeof(Host);
                return bytes;
            }

            //! Constructs a copy host instance at the pointer
            void construct(Host* p, const Host& value){
                new(p) Host(value);
            }

            //! Destructs the host instance at p
            void destroy(Host* p){
                (void)p;
                p->~Host();
            }
        };

        //! Always returns true. Nothing internal.
        template<typename Host>
        inline bool operator == (const STLAdapter<Host>&, const STLAdapter<Host>& ){
            return true;
        }

        //! Always returns false. Nothing internal.
        template < typename Host >
        inline bool operator != ( const STLAdapter< Host > & , const STLAdapter< Host > & )
        {
            return false;
        }

    }
}

#endif //File Guardian