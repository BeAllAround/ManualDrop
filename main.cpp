
#include <cstddef>
#include <iostream>


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

    template <typename... Args>
    ManuallyDrop(Args &&...args)
    {
        new(&u.t) T(std::forward<Args>(args)...);
    }


    ManuallyDrop(const ManuallyDrop& other) {
        new(u.t) T(other);
    }

    ManuallyDrop(ManuallyDrop&& other) {
        new(u.t) T(std::move(other));
    }

    ManuallyDrop& operator=(const ManuallyDrop& other) {
        // Equivalent to the respective operator=(const T&)
        u.t.~T();
        new(u.t) T(other);


        return *this;
    }

    ManuallyDrop& operator=(ManuallyDrop&& other) {
        // Equivalent to the respective operator=(T&&)
        u.t.~T();
        new(u.t) T(std::move(other));

        return *this;
    }


    void restore() __force_inline
    {
        u.t.~T();
    }

    ~ManuallyDrop() __force_inline
    {

    }

};

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

    
int main(){
    {
        ManuallyDrop<S> s;
        ManuallyDrop<S> s1 (1);

    }

    return 0;
}
