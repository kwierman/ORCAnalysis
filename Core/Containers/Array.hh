#ifndef Protium_Array_hh
#define Protium_Array_hh

namespace Protium{
    namespace Containers{
        template<class T, std::size_t N>
        class Array{
            T data[N];
        public:

            typedef T              value_type;
            typedef T*             iterator;
            typedef const T*       const_iterator;
            typedef T&             reference;
            typedef const T&       const_reference;
            typedef std::size_t    size_type;
            typedef std::ptrdiff_t difference_type;

            static size_type size() { return N; }
            static bool empty() { return false; }
            static size_type max_size() { return N; }
            enum { static_size = N };

            // check range (may be private because it is static)
            static void rangecheck (size_type i) {
                if (i >= size()) {
                    throw std::out_of_range("array<>: index out of range");
                }
            }


                    // iterator support
            iterator begin() { return elems; }
            const_iterator begin() const { return elems; }
            iterator end() { return elems+N; }
            const_iterator end() const { return elems+N; }

            reference front() 
            { 
                return elems[0]; 
            }
            
            const_reference front() const 
            {
                return elems[0];
            }
            
            reference back() 
            { 
                return elems[N-1]; 
            }
            
            const_reference back() const 
            { 
                return elems[N-1]; 
            }

            typedef std::reverse_iterator<iterator> reverse_iterator;
            typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

            reverse_iterator rbegin() { return reverse_iterator(end()); }
            const_reverse_iterator rbegin() const {
                return const_reverse_iterator(end());
            }
            reverse_iterator rend() { return reverse_iterator(begin()); }
            const_reverse_iterator rend() const {
                return const_reverse_iterator(begin());
            }

            // operator[]
            reference operator[](size_type i) 
            { 
                PRTIUM_ASSERT( i < N && "out of range" ); 
                return elems[i];
            }
            
            const_reference operator[](size_type i) const 
            {     
                PROTIUM_ASSERT( i < N && "out of range" ); 
                return elems[i]; 
            }
            reference at(size_type i) { rangecheck(i); return elems[i]; }
            const_reference at(size_type i) const { rangecheck(i); return elems[i]; }

            // swap (note: linear complexity)
            void swap (array<T,N>& y) {
                std::swap_ranges(begin(),end(),y.begin());
            }

            // direct access to data (read-only)
            const T* data() const { return elems; }
            T* data() { return elems; }

            // use array as C array (direct read/write access to data)
            T* c_array() { return elems; }

            // assignment with type conversion
            template <typename T2>
            array<T,N>& operator= (const array<T2,N>& rhs) {
                std::copy(rhs.begin(),rhs.end(), begin());
                return *this;
            }

            // assign one value to all elements
            void assign (const T& value)
            {
                std::fill_n(begin(),size(),value);
            }


        };

        template<class T>
        class Array<T, 0>{
      public:
        // type definitions
        typedef T              value_type;
        typedef T*             iterator;
        typedef const T*       const_iterator;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

        // iterator support
        iterator begin() { return iterator( reinterpret_cast< T * >( this ) ); }
        const_iterator begin() const { return const_iterator(  reinterpret_cast< const T * >( this ) ); }
        iterator end() { return begin(); }
        const_iterator end() const { return begin(); }

        typedef std::reverse_iterator<iterator,T> reverse_iterator;
        typedef std::reverse_iterator<const_iterator,T> const_reverse_iterator;

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        // operator[]
        reference operator[](size_type i)
        {
            return failed_rangecheck();
        }

        const_reference operator[](size_type i) const
        {
            return failed_rangecheck();
        }

        // at() with range check
        reference at(size_type i)               {   return failed_rangecheck(); }
        const_reference at(size_type i) const   {   return failed_rangecheck(); }

        // front() and back()
        reference front()
        {
            return failed_rangecheck();
        }

        const_reference front() const
        {
            return failed_rangecheck();
        }

        reference back()
        {
            return failed_rangecheck();
        }

        const_reference back() const
        {
            return failed_rangecheck();
        }

        // size is constant
        static size_type size() { return 0; }
        static bool empty() { return true; }
        static size_type max_size() { return 0; }
        enum { static_size = 0 };

        void swap (array<T,0>& y) {
        }

        // direct access to data (read-only)
        const T* data() const { return 0; }
        T* data() { return 0; }

        // use array as C array (direct read/write access to data)
        T* c_array() { return 0; }

        // assignment with type conversion
        template <typename T2>
        array<T,0>& operator= (const array<T2,0>& ) {
            return *this;
        }

        // assign one value to all elements
        void assign (const T& ) {   }

        // check range (may be private because it is static)
        static reference failed_rangecheck () {
                std::out_of_range e("attempt to access element of an empty array");
                throw e;
                static T placeholder;
                return placeholder;
            }
        };


        // comparisons
        template<class T, std::size_t N>
        bool operator== (const Array<T,N>& x, const Array<T,N>& y) {
            return std::equal(x.begin(), x.end(), y.begin());
        }
        template<class T, std::size_t N>
        bool operator< (const Array<T,N>& x, const Array<T,N>& y) {
            return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
        }
        template<class T, std::size_t N>
        bool operator!= (const Array<T,N>& x, const Array<T,N>& y) {
            return !(x==y);
        }
        template<class T, std::size_t N>
        bool operator> (const Array<T,N>& x, const Array<T,N>& y) {
            return y<x;
        }
        template<class T, std::size_t N>
        bool operator<= (const Array<T,N>& x, const Array<T,N>& y) {
            return !(y<x);
        }
        template<class T, std::size_t N>
        bool operator>= (const Array<T,N>& x, const Array<T,N>& y) {
            return !(x<y);
        }

        // global swap()
        template<class T, std::size_t N>
        inline void swap (Array<T,N>& x, Array<T,N>& y) {
            x.swap(y);
        }
    }
}

#endif //File Guardian



