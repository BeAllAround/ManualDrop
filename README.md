# C++ Manual Drop

`ManualDrop<T>` enables you to entirely "drop" the implicit destructor of `T` RAII with the capability to invoke it explicitly or adopt it into another RAII-based object.



Examples include:

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

ManuallyDrop<S> s (1);
ManuallyDrop<S> s1 = s;


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

