std::string my_first_interpreter(const std::string& code) {

    std::string os;
    int c = 0;

    for (auto &it : code)
        if (it == '+' || it == '.') {
            if (c == 256) c = 0;
            if (it == '.') os += c;
            else c++;
        }

    return os;
}
