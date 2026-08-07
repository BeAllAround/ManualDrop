#include <iostream>

#include <vector>


#include <cassert>
#include <cstring>

#include "../../ManualDrop.hpp"


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


