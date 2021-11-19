//
// Created by Molybdenum on 8/7/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//

#include "Core/Notification.hpp"
#include <unordered_map>
#include <vector>
namespace WN {

class NotificationCenter::Pimpl {
    std::unordered_multimap<NotificationName, std::pair<void*, NotificationCallback>> map;

public:
    void addObserver(const NotificationName& name, void* observer, const NotificationCallback& callback) noexcept {
//        map[std::move(name)].emplace_back(observer, callback);
        map.insert({name, {observer, callback}});
    }

    void postNotification(Notification &aNotification) {
        if (!map.count(aNotification.getName())) {
            return;
        }
//        for (auto &[observer, callback] : map.at(aNotification.getName())) {
//            callback(observer, aNotification);
//        }
        auto range = map.equal_range(aNotification.getName());
        for (auto it = range.first; it != range.second; ++it) {
            auto &[observer, callback] = (*it).second;
            callback(observer, aNotification);
        }
    }

};

NotificationCenter::NotificationCenter() {
    pimpl = new Pimpl;

}

NotificationCenter::~NotificationCenter() {
    delete pimpl;
}

void NotificationCenter::postNotification(Notification aNotification) {
    pimpl->postNotification(aNotification);
}

void NotificationCenter::addObserver(
        const NotificationName &name,
        void *observer,
        const NotificationCallback& callback) {
    pimpl->addObserver(name, observer, callback);

}


NotificationCenter &NotificationCenter::GetDefault() noexcept {
    static NotificationCenter center;
    return center;
}




}