/*!
* Created by Molybdenum on 9/17/21.
*
*/
#ifndef NITETRICHOR_HASH_HPP
#define NITETRICHOR_HASH_HPP

namespace NC {

template<typename Key = void>
struct Hash;


template <>
struct Hash<void> {
    template <typename T>
    inline size_t operator()(T&& t) const noexcept(noexcept(Hash<std::remove_reference_t<T>>()(std::forward<T>(t)))) {
        return Hash<std::remove_reference_t<T>>()(std::forward<T>(t));
    }
};

template<>
struct Hash<std::string> {
    inline size_t operator()(const std::string &string) noexcept {
        static constexpr size_t magic = 13131313;
        size_t ret = 0;
        for (auto i: string) {
            ret = ret * magic + i;
        }
        return ret;
    }
};




}


#endif //NITETRICHOR_HASH_HPP
