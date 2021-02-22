#pragma once

#include <set>
#include <string>
#include <vector>
#include <unordered_set>

namespace pyutils {
    template <typename T1, typename T2>
    bool in(T1 to_find, std::vector<T2> vec) {
        return std::find(vec.begin(), vec.end(), to_find) != vec.end();
    }

    template <typename T1, typename T2>
    bool in(T1 to_find, std::set<T2> vec) {
        return std::find(vec.begin(), vec.end(), to_find) != vec.end();
    }

    template <typename T1, typename T2>
    bool in(T1 to_find, std::unordered_set<T2> vec) {
        return std::find(vec.begin(), vec.end(), to_find) != vec.end();
    }

    // inline to stop duplicate symbol link errors
    // not sure this is portable...
    inline bool in(std::string to_find, std::string str) {
        return str.find(to_find) != std::string::npos;
    }

    inline std::string get_str_between(std::string item, std::string start_token, std::string end_token) {
        auto start = item.find(start_token);
        auto real_start = start + start_token.size();
        auto end = item.find(end_token);
        std::string card_name = item.substr(real_start, end - real_start);
        return card_name;
    }
}
