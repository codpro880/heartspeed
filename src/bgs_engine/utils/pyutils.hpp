#include <set>
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
}
