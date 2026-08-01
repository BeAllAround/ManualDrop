#include <iostream>

#include <vector>

#include <cstddef>
#include <cassert>


#include "ManualDrop.hpp"



class S {
  public:
  int* i_ptr { nullptr };
  S() {
    std::cout << "S()" << std::endl;
  }

  S(int i) {
    std::cout << "S(int)" << std::endl;
    i_ptr = new int(i);
  }

  S(const S& other) {
    std::cout << "S(const S&)" << std::endl;
    i_ptr = new int(*other.i_ptr);
  }

  // NOTE: std::vector will invoke the copy constructor during reallocation, but only if the element's move constructor is not marked noexcept
  S(S&& other) noexcept
  {
    std::cout << "S(S&&)" << std::endl;
    i_ptr = other.i_ptr;
    other.i_ptr = nullptr;
  }


  S& operator=(const S& other) {
    std::cout << "S&operator=(const S&)" << std::endl;
    if(i_ptr != nullptr) {
      delete i_ptr;
    }
    i_ptr = new int(*other.i_ptr);
    return *this;
  }


  S& operator=(S&& other) {
    std::cout << "S&operator=(S&&)" << std::endl;
    if(i_ptr != nullptr) {
      delete i_ptr;
    }
    i_ptr = other.i_ptr;
    other.i_ptr = nullptr;
    return *this;
  }

  ~S() {
    std::cout << "~S()" << std::endl;

    if(i_ptr != nullptr) { // The delete below already does this check, effectively
      delete i_ptr;
      i_ptr = nullptr;
    }

  }
};

#define _LOGS_VERBOSE

int _block_print(const std::string& block_name) {
    #ifdef _LOGS_VERBOSE
    std::cout << "Testing " << block_name << " Block" << std::endl;
    #endif

    return 0;
}

int _defer_block_print(const std::string& block_name) {
    #ifdef _LOGS_VERBOSE
    std::cout << "Finished " << block_name << " Block" << std::endl;
    #endif

    return 0;
}

#define assertm(msg, exp) assert((void(msg), exp))

#define TEST_BLOCK(block_name) for(int i = _block_print(block_name); i < 1; i++, _defer_block_print(block_name))

template<class T>
using Moveable = ManuallyDrop<T>;

    
int main(){

    TEST_BLOCK("Basic Tests | Implicit Destruction Disabled") {

      int* s_p_i { nullptr };
      int* s1_p_i { nullptr };

      {
        ManuallyDrop<S> s;
        ManuallyDrop<S> s1 (1);

        s_p_i = s.get_resource_as_pointer()->i_ptr;

        s1_p_i = s1.get_resource_as_pointer()->i_ptr;

      }

      assertm(
        "Detached on-heap members of S not cleaned up", 
        s_p_i == nullptr &&
        *(s1_p_i) == 1
      );

      // Heap deallocation of the detached int*
      delete s1_p_i;


    }


    TEST_BLOCK("C++ Semantics") {

      TEST_BLOCK("Copy Constructor") {
        ManuallyDrop<S> s (1);
        ManuallyDrop<S> s1 = s;

        assertm(
          "Copy OK",
          s.get_resource_as_pointer() != s1.get_resource_as_pointer() &&
          s.get_resource_as_pointer()->i_ptr != s1.get_resource_as_pointer()->i_ptr &&
          *(s.get_resource_as_pointer()->i_ptr) == *(s1.get_resource_as_pointer()->i_ptr)
        );

        s.restore();
        s1.restore();

      }

      TEST_BLOCK("Copy Assignment") {
        {

          ManuallyDrop<S> s;
          ManuallyDrop<S> s1 (1);

          s = s1;

          assertm(
            "Copy OK",
            s.get_resource_as_pointer() != s1.get_resource_as_pointer() &&
            s.get_resource_as_pointer()->i_ptr != s1.get_resource_as_pointer()->i_ptr &&
            *(s.get_resource_as_pointer()->i_ptr) == *(s1.get_resource_as_pointer()->i_ptr)
          );

          s.restore();
          s1.restore();
        }


        {
          ManuallyDrop<S> s (2);
          ManuallyDrop<S> s1 (1);

          // RESTORE REQUIRED BEFORE COPY/MOVE ASSIGNMENT SINCE THERE IS NO IMPLICIT DESTRUCTION
          s.restore(); 
          s = s1;

          assertm(
            "Copy OK",
            s.get_resource_as_pointer() != s1.get_resource_as_pointer() &&
            s.get_resource_as_pointer()->i_ptr != s1.get_resource_as_pointer()->i_ptr &&
            *(s.get_resource_as_pointer()->i_ptr) == *(s1.get_resource_as_pointer()->i_ptr)
          );

          s.restore();
          s1.restore();
        }


      }


      TEST_BLOCK("Move Constructor") {
        ManuallyDrop<S> s (1);
        ManuallyDrop<S> s1 = std::move(s);

        assertm(
          "Move OK",
          s.get_resource_as_pointer() != s1.get_resource_as_pointer() &&
          s.get_resource_as_pointer()->i_ptr == nullptr && // i_ptr moved
          s1.get_resource_as_pointer()->i_ptr != nullptr &&
          *(s1.get_resource_as_pointer()->i_ptr) == 1
        );

        // s.restore(); // NOT NEEDED ESSENTIALLY AS IT IS MOVED INTO s1
        s1.restore();

      }

      TEST_BLOCK("Move Assignment") {

        {

          ManuallyDrop<S> s;
          ManuallyDrop<S> s1 (1);

          s = std::move(s1);

          assertm(
            "Move OK",
            s.get_resource_as_pointer() != s1.get_resource_as_pointer() &&
            s1.get_resource_as_pointer()->i_ptr == nullptr && // i_ptr moved
            s.get_resource_as_pointer()->i_ptr != nullptr &&
            *(s.get_resource_as_pointer()->i_ptr) == 1
          );

          // s1.restore(); // NOT NEEDED ESSENTIALLY AS IT IS MOVED INTO s
          s.restore();
        }

        {

          ManuallyDrop<S> s (2);
          ManuallyDrop<S> s1 (1);

          // RESTORE REQUIRED BEFORE COPY/MOVE ASSIGNMENT SINCE THERE IS NO IMPLICIT DESTRUCTION
          s.restore(); 
          s = std::move(s1);

          assertm(
            "Move OK",
            s.get_resource_as_pointer() != s1.get_resource_as_pointer() &&
            s1.get_resource_as_pointer()->i_ptr == nullptr && // i_ptr moved
            s.get_resource_as_pointer()->i_ptr != nullptr &&
            *(s.get_resource_as_pointer()->i_ptr) == 1
          );

          // s1.restore(); // NOT NEEDED ESSENTIALLY AS IT IS MOVED INTO s
          s.restore();
        }

      }

      TEST_BLOCK("Moving it under RAII ownership OK") {
        ManuallyDrop<S> s (1);
        S s1 = std::move(s); // operator T&&() Triggered here

        assertm(
          "Move OK",
          s.get_resource_as_pointer()->i_ptr == nullptr && // i_ptr moved
          s1.i_ptr != nullptr &&
          *(s1.i_ptr) == 1
        );


        // Implicit s1.~S() invoked
        // Thus, s.restore() is not needed

      }

    }


    TEST_BLOCK("Guaranteed MOVE out of the scope") {
      std::vector<S> v;

      Moveable<S> s (10);


      v.push_back(
        std::move(s)
      );

      assertm(
        "S moved OK", 
        v[0].i_ptr != nullptr &&
        *(v[0].i_ptr) == 10
      );

      // Output
      /*
      S(int)
      S(S&&)
      ~S() // Only one destructor invoked of the ~std::vector<S> "RAII" as the s is guaranteed to be moved
      */
    }

    return 0;
}
