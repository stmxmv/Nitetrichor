// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// UUID.cpp
//
// Created by Molybdenum on 11/16/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//


#include "Core/UUID.hpp"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <objbase.h>
#include <string>
#include <algorithm>
#include <cctype>
#else
#include <uuid/uuid.h>
#endif
using namespace std;
namespace AN {
UUID::UUID(FromRandom_t) {
#if defined(_WIN32)
    ::UUID uuid;
    ::CoCreateGuid(&uuid);

    memcpy(Value, &uuid, Size);
#else
    uuid_generate_random(Value);
#endif
}

UUID::UUID(FromTime_t) {
#if defined(_WIN32)
    ::UUID uuid;
    ::CoCreateGuid(&uuid);

    memcpy(Value, &uuid, Size);
#else
    uuid_generate_time(Value);
#endif
}

UUID::UUID() {
#if defined(_WIN32)
    ::UUID uuid = *((::UUID *)&Value);
    UuidCreateNil(&uuid);

    memcpy(Value, &uuid, Size);
#else
    uuid_clear(Value);
#endif
}

std::string UUID::toString() const {
#if defined(_WIN32)
    ::UUID uuid;
    memcpy(&uuid, Value, Size);

    RPC_CSTR str;
    UuidToStringA(&uuid, &str);

    char ret[StringBufferSize];
    char* signedStr = reinterpret_cast<char*>(str);
    memcpy(ret, signedStr, StringBufferSize);
    transform(ret, ret + StringBufferSize,ret,  ::toupper);

#else
    char ret[StringBufferSize];
    uuid_unparse_upper(Value, ret);
#endif
    return {ret};
}


}




