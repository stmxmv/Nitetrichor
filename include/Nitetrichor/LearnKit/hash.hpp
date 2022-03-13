/*!
* Created by Molybdenum on 9/17/21.
*
*/
#ifndef NITETRICHOR_HASH_HPP
#define NITETRICHOR_HASH_HPP
#include <string>

namespace AN {



template<typename Key = void>
struct hash;


template <>
struct hash<void> {
    template <typename T>
    inline size_t operator()(T&& param) const noexcept(noexcept(hash<std::remove_reference_t<T>>()(std::forward<T>(param)))) {
        return hash<std::remove_reference_t<T>>()(std::forward<T>(param));
    }
};

template<>
struct hash<int> {
    inline size_t operator()(int num) noexcept {
        static constexpr int magic = 11;
        return num % magic;
    }
};

//static int find_cnt = 0;


template<typename Key, typename Value, typename HashFunc = hash<Key>>
class hash_table {

    struct table_node {
        Key key;
        Value value;
        std::unique_ptr<table_node> next;
        table_node() : key(), value(), next() {}
    };

    std::vector<std::unique_ptr<table_node>> table;
public:

    explicit hash_table(int size) : table(size) {}


    Value &operator[](Key key) {
        size_t hash_value = HashFunc()(key);
        if (!table[hash_value]) {
            table[hash_value] = std::make_unique<table_node>();
            table[hash_value]->key = std::move(key);
            return table[hash_value]->value;
        }
        std::unique_ptr<table_node> new_node = std::make_unique<table_node>();
        new_node->key = std::move(key);

        new_node->next = std::move(table[hash_value]);
        table[hash_value] = std::move(new_node);
        return table[hash_value]->value;
    }

    int index(Key key) {
        size_t hash_value = HashFunc()(key);

        table_node *node = table[hash_value].get();
//        find_cnt = 1;
        while (node) {
            if (node->key == key) {
                return (int)hash_value;
            }
//            ++find_cnt;
            node = node->next.get();
        }
        return -1;
    }

};




}
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
