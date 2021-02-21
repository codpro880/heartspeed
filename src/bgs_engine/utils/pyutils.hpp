namespace pyutils {
    bool contains(std::vector<std::string> vec, std::string to_find) {
        return std::find(vec.begin(), vec.end(), to_find) != vec.end();
    }
                                                              
}
