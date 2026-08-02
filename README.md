# C++ Manual Drop

`ManualDrop<T>` enables you to entirely "drop" the implicit destructor of `T` RAII with the capability to invoke it explicitly or move it into another RAII-based object.

`ManuallyDrop<T>` eliminates the destructor call for objects whose lifetime is known to end via transfer of ownership rather than scope exit. This can reduce overhead when destructors are non-trivial or cannot be optimized away, particularly in low-level containers, allocators, and performance-critical code.

This library is a utility for explicitly separating object lifetime from storage lifetime.

One of the recommended use cases is when an object is guaranteed to be moved out of the scope, so that no additional destructor [potentially overhead in some cases] call is invoked for it.

This "potential" overhead mostly involves expensive situations where the `~T()` destructor cannot be inlined so you are saving:

- one branch to the destructor
- one return
- whatever the destructor itself checks

For example,

```cpp
template<class T>
using Moveable = ManuallyDrop<T>;

{
  std::vector<S> v;

  Moveable<S> s (10);

  v.push_back(
    std::move(s)
  );

  // Output
  /*
  S(int)
  S(S&&)
  ~S() // Only one destructor invoked by the ~std::vector<S>() "RAII" as the s is guareeted to be moved
  */
}
```



### Types with expensive moved-from destructors

Some libraries don't bother making moved-from destruction cheap.

For example, even after moving, we end up with:

```cpp
~S() {
    unregister_from_global_registry();
}
```





## Installation

Just dump the `ManualDrop.hpp` header into your project and you are good to go!





# Examples

## Manual Destroy

```cpp
class S {
    S();
    S(int);
    S(const S&);
    S(S&&);
    S&operator=(const S&);
    S&operator=(S&&);
    ~S();
}

ManuallyDrop<S> s (1); // S(int)
ManuallyDrop<S> s1 = s; // S(const S&)


s.drop(); // Trigger s.~S()
s1.drop(); // Trigger s1.~S()

// Output
/*
S(int)
S(const S&)
~S()
~S()
*/
```



## Moving it under RAII ownership

```cpp
ManuallyDrop<S> s (1);
S s1 = std::move(s); // [Implicit] Contextual conversion to S&& (operator S&&()) triggered here


// Implicit s1.~S() invoked
// Thus, s.drop() is not needed

// Output
/*
S(int)
S(S&&)
~S()
*/
```



## Guaranteed No Automatic RAII Cleanup

```cpp
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
```



## Usage of Copy/Move Assignments

```cpp
{
  ManuallyDrop<S> s (2);
  ManuallyDrop<S> s1 (1);
  // DROP REQUIRED (IF INITIALIZED) BEFORE COPY/MOVE ASSIGNMENT SINCE THERE IS NO IMPLICIT DESTRUCTION
  s.drop(); 
  s = std::move(s1);

  // s1.drop(); // NOT NEEDED ESSENTIALLY AS IT IS MOVED INTO s
  s.drop();
}
```



## Buffer Reallocation RAII Improvements

```cpp
// Compare the RAII outputs of these two from the main.cpp test suite
{
  BLOCK("S RAII Output | Vector reallocation move + destructor") {
    std::vector<S> v;
    v.reserve(10);

    for(size_t i = 0; i < 12; i++) {
      S item (i);
      v.push_back(std::move(item));
    }

  }

  BLOCK("Moveable<S> RAII Output | Vector reallocation move + destructor") {
    std::vector<Moveable<S>> v;
    v.reserve(10);

    for(size_t i = 0; i < 12; i++) {
      Moveable<S> item (i);
      v.push_back(std::move(item));
    }

    defer: {

      for(auto start = v.begin(); start != v.end(); start++) {
        (*start).drop();
      }

    }
  }

}

```

