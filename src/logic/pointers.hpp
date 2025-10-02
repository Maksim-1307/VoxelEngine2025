#pragma once

#include <memory>

/* pseudonyms for std::shared_ptr and std::unique_ptr */

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using uptr = std::unique_ptr<T>;

template<typename T, typename... Args>
auto make_sptr(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
auto make_uptr(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

