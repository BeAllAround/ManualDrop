#include <iostream>

#include <vector>


#include <cassert>
#include <cstring>

/**
 * @author Alex Milenkovic
 *
 * Copyright 2026, Alex Milenkovic. All rights reserved.
 * Use of this source code is governed by the MIT license
 * that can be found in the LICENSE file.
 *
 * SPDX-License-Identifier: MIT
 */




// NOTE: In case, some compilers don't support always_inline as the same macro
#define __force_inline __attribute__((always_inline))

template<class T>
class ManuallyDrop {
    public:
        union U{

            T t;

            U() __force_inline
            {

            }

            ~U() __force_inline
            {
            }

        } u;

    ManuallyDrop() __force_inline
    {
        new (&u.t) T();
        // Due to
        /*
        // valgrind -s --track-origins=yes ./out.out
        ==18468== 1 errors in context 1 of 1:
        ==18468== Conditional jump or move depends on uninitialised value(s)
        ==18468==    at 0x10945D: main (in /home/alex/Project/ManualDrop/out.out)
        ==18468==  Uninitialised value was created by a stack allocation
        ==18468==    at 0x109344: main (in /home/alex/Project/ManualDrop/out.out)
        */
    }

    // NOTE: Making this constructor explicit so as not to cause conflict/ambiguity with the copy/move constructor below
    template <typename... Args>
    explicit ManuallyDrop(Args &&...args)
    {
        new(&u.t) T(std::forward<Args>(args)...);
    }


    ManuallyDrop(const ManuallyDrop<T>& other) {
        new(&u.t) T(other.u.t);
    }

    // NOTE: For example, std::vector will invoke the copy constructor during reallocation, but only if the element's move constructor is not marked noexcept
    ManuallyDrop(ManuallyDrop<T> && other) noexcept
    {
        new(&u.t) T(std::move(other.u.t));
    }

    ManuallyDrop& operator=(const ManuallyDrop<T> & other) {
        // Equivalent to the respective operator=(const T&)
        // u.t.~T(); // NOTE: Should not be technically allowed

        new(&u.t) T(other.u.t);


        return *this;
    }

    ManuallyDrop& operator=(ManuallyDrop<T> && other) {
        // Equivalent to the respective operator=(T&&)
        // u.t.~T(); // NOTE: Should not be technically allowed

        new(&u.t) T(std::move(other));

        return *this;
    }


    // TODO: SUPPORT THESE?
    ManuallyDrop(T&&) = delete;
    ManuallyDrop(const T&) = delete;
    ManuallyDrop&operator=(T&&) = delete;
    ManuallyDrop&operator=(const T&) = delete;

    /*
    operator T&(){
        return u.t;
    }
    */
    // For implictly adopting T into the RAII: "T t = std::move(resource);""
    operator T&&(){
        return std::move(u.t);
    }

    /*
    const T& get_resource() {
        return u.t;
    }
    */

    T& get_resource() {
        return u.t;
    }

    T* get_resource_as_pointer() {
        return &u.t;
    }


    void drop() __force_inline
    {
        u.t.~T();
    }

    void destroy() __force_inline
    {
        drop();
    }

    // Even with an empty body, this destructor is considered to be non-trivial
    // However, it will be NOP nonetheless.
    /*
    ~ManuallyDrop() __force_inline
    {
    }
    */

    // Trivial destructor, explicitly defaulted
    // Note: Optimized away as a CALL -> NOP from -O1 optimization levels on
    ~ManuallyDrop() = default;

};



#define start_time clock_t s_t_a_r_t = clock();

#define end_time printf("[Cpu_time_used: %f]\n", static_cast < double > (clock() - s_t_a_r_t) / CLOCKS_PER_SEC);


class Base {
    public:
    int* n {nullptr};

    Base(int i) {
      // n = new int(i);
    }

    virtual ~Base() {
        delete n;
        n = nullptr;
    }
};

class Derived : public Base {
    public:
        int* n1 {nullptr};

        Derived(int i) : Base(i) {
            // n1 = new int(i);
        }

        void sanity_check() {

            assert(n1 != nullptr);
            assert(
                (reinterpret_cast<Base*>(this)->n) != nullptr
            );

            /*
            std::cout << "Derived::n1 " << *n1 << std::endl;

            std::cout << "Derived::Base::n " << 
                *(reinterpret_cast<Base*>(this)->n) << std::endl;
            */
        }

        void derived_call_only() {
            assert(n1 != nullptr);

            std::cout << "Derived CALL! Derived::n1 " << *n1 << std::endl;
        }

        ~Derived() {
            delete n1;
            n1 = nullptr;
        }

};


class Derived1 : public Base {


    public:
        int* n2 {nullptr};



        ~Derived1() {
            delete n2;
            n = nullptr;
        }

};

class Derived2 : public Base {


    public:
        int* n2 {nullptr};



        ~Derived2() {
            delete n2;
            n = nullptr;
        }

};


class _Holder {
    public:
    Base* d {nullptr};

    _Holder(int i) {
        d = new Derived(i);
    }

    _Holder(_Holder&& other) noexcept {
        d = other.d;
        other.d = nullptr;
    }




    ~_Holder() __attribute__((noinline)) // Forcing the destructor not to be inlined
    {
        // Force virtual destructor 
        if(d != nullptr) {

            delete d;
            d = nullptr;
        }
    }
};


_Holder _make_holder(int i) {
    return _Holder(i);
}


// sudo perf stat ./out.out && sudo perf stat ./out1.out
#define _SIZE 1000000

// NOTE: Moving one milion objects benchmark
void benchmark01() {

    std::vector<_Holder> v;
    v.reserve(10);

    for(size_t i = 0; i < _SIZE; i++ ) {
        _Holder to_move = _make_holder(i);
        v.push_back(std::move(to_move));
        // v.push_back(_make_holder(i));
    }

}


ManuallyDrop<_Holder> _make_droppable_holder(int i) {
    return ManuallyDrop<_Holder>(i);
}


void benchmark02() {
    std::vector<_Holder> v;
    v.reserve(10);

    // This benchmarks also takes all of the reallocations into consideration S(S&&) + ~S() as noted in the ManualDrop/README.md of top folder of this repo.
    for(size_t i = 0; i < _SIZE; i++ ) {
        ManuallyDrop<_Holder> to_move = _make_droppable_holder(i);
        v.push_back(std::move(to_move));
        // v.push_back(_make_droppable_holder(i));
    }

}


