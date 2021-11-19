/*!
* Created by Molybdenum on 9/28/21.
*
*/
#ifndef NITETRICHOR_WRAP_ARRAY_HPP
#define NITETRICHOR_WRAP_ARRAY_HPP
namespace NC {
/// \brief wrapper for array in stack to stl, because std::array not support push_back etc, 
///        and it can be easily replaced by std::vector to support dynamic allocation.
/// \tparam T
/// \tparam Size
template<typename T, int Size>
class Array {
    int cur;
    T _array[Size];
public:
    typedef T value_type;
    typedef T &reference;
    typedef const T &const_reference;
    typedef int size_type;
    typedef T *iterator;
    typedef const T *const_iterator;

    inline constexpr Array() noexcept: cur{}, _array{} {}

    inline void push_back(const T &item) { _array[cur++] = item; }

    inline void push_back(T &&item) { _array[cur++] = item; }

    inline void pop_back() { --cur; }

    inline void clear() noexcept { memset(this, 0, sizeof *this); }

    inline reference operator[](size_type pos) { return _array[pos]; }

    inline const_reference operator[](size_type pos) const { return _array[pos]; }

    inline reference front() { return _array[0]; }

    [[nodiscard]] inline const_reference front() const { return _array[0]; }

    inline iterator begin() noexcept { return _array; }

    [[nodiscard]] inline const_iterator begin() const noexcept { return _array; }

    inline iterator end() noexcept { return _array + cur; }

    [[nodiscard]] inline const_iterator end() const noexcept { return _array + cur; }

    [[nodiscard]] inline bool empty() const noexcept { return !cur; }

};
}
#endif //NITETRICHOR_WRAP_ARRAY_HPP
