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

    ~ManuallyDrop() __force_inline
    {

    }

};
