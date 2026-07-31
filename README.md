# C++ Manual Drop

`ManualDrop<T>` enables you to entirely "drop" the implicit destructor of `T` RAII with the capability to invoke it explicitly or adopt it into another RAII-based object.

## Installation

Just dump the `ManualDrop.hpp` header into your project and you are good to go!





# Examples

## Manual Restoration

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


s.restore(); // Trigger s.~S()
s1.restore(); // Trigger s1.~S()

// Output
/*
S(int)
S(const S&)
~S()
~S()
*/
```



## Adopting it into RAII

```cpp
ManuallyDrop<S> s (1);
S s1 = std::move(s); // [Implicit] Contextual conversion to S&& (operator S&&()) triggered here


// Implicit s1.~S() invoked
// Thus, s.restore() is not needed

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
  // RESTORE REQUIRED (IF INITIALIZED) BEFORE COPY/MOVE ASSIGNMENT SINCE THERE IS NO IMPLICIT DESTRUCTION
  s.restore(); 
  s = std::move(s1);

  // s1.restore(); // NOT NEEDED ESSENTIALLY AS IT IS MOVED INTO s
  s.restore();
}
```

