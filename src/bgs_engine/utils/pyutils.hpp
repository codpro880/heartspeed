namespace pyutils {
    template <typename T1, typename T2>
    bool contains(std::vector<T1> vec, T2 to_find) {
        return std::find(vec.begin(), vec.end(), to_find) != vec.end();
    }
                                                              
}
