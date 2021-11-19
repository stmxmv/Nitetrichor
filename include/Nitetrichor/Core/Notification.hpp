//
// Created by Molybdenum on 8/7/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//

#ifndef LEARNKIT_NOTIFICATION_H
#define LEARNKIT_NOTIFICATION_H

#include <any>
#include <string>
#include <functional>
#include <utility>
namespace WN {


#pragma GCC visibility push(default)

typedef std::string NotificationName;


class Notification {
    std::any object;
    NotificationName name;
public:
    Notification(NotificationName name, std::any object) : object(std::move(object)), name(std::move(name)) {}

    [[nodiscard]] const std::any& getObject() const noexcept { return object; }

    [[nodiscard]] const NotificationName& getName() const noexcept { return name; }
};

typedef std::function<void(void*, const Notification&)> NotificationCallback;



//template<typename T, typename Callback>
//struct MethodHelper {
//    inline static Callback callback;
//    static void call(void * self, const Notification& aNotification) {
//        std::invoke(callback, *(T*)self, aNotification);
//    }
//};


class NotificationCenter {
    class Pimpl;

    Pimpl * pimpl;

public:
    NotificationCenter();
    virtual ~NotificationCenter();

    static NotificationCenter& GetDefault() noexcept;

    void postNotification(Notification aNotification);

    template<typename T, typename Callback>
    void addObserver(const NotificationName& name, T* observer, Callback callback) {
        const std::function<void(T&, const Notification&)> memFunc(callback);
        const NotificationCallback memCallback = [memFunc = std::move(memFunc)]
                (void* self, const Notification& aNotification) -> void {
            memFunc(*reinterpret_cast<T*>(self), aNotification);
        };
        addObserver(name, (void*)observer, memCallback);
    }

    void addObserver(const NotificationName& name, void* observer, const NotificationCallback&);

};


}

#pragma GCC visibility pop

#endif //LEARNKIT_NOTIFICATION_H
