// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// UUID.hpp
//
// Created by Molybdenum on 11/16/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_UUID_HPP
#define NITETRICHOR_UUID_HPP

#include <string>
namespace AN {

// see https://github.com/apple/swift/blob/7123d2614b5f222d03b3762cb110d27a9dd98e24/include/swift/Basic/UUID.h

class UUID {
public:
    enum {
        /// The number of bytes in a UUID's binary representation.
        Size = 16,

        /// The number of characters in a UUID's string representation.
        StringSize = 36,

        /// The number of bytes necessary to store a null-terminated UUID's string
        /// representation.
        StringBufferSize = StringSize + 1,
    };
    unsigned char Value[Size];
private:
    enum FromRandom_t { FromRandom };
    enum FromTime_t { FromTime };

    explicit UUID(FromRandom_t);

    explicit UUID(FromTime_t);
public:
    /// Empty constructor.
    UUID();

    static UUID fromRandom() { return UUID(FromRandom); }

    static UUID fromTime() { return UUID(FromTime); }

    [[nodiscard]] std::string toString() const;
};
}



#endif//NITETRICHOR_UUID_HPP
