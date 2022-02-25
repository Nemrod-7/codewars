class Dinglemouse {
    std::string name, order;
    int age;
    char sex;

    public:

        Dinglemouse () { }

        Dinglemouse &setAge (int age) {
            this->age = age;
            if (order.find('a') == std::string::npos) order += 'a';
            return *this;
        }

        Dinglemouse &setSex (char sex) {
            this->sex = sex;
            if (order.find('s') == std::string::npos)  order += 's';
            return *this;
        }

        Dinglemouse &setName (const std::string &name) {
            this->name = name;
            if (order.find('n') == std::string::npos) order += 'n';
            return *this;
        }

        std::string hello() {
            std::string os = "Hello.";
          
            for (auto &it : order) {
                switch (it) {
                    case 'n' : os += " My name is " + name + "."; break;
                    case 'a' : os += " I am " + std::to_string(age) + "."; break;
                    case 's' : os += " I am " + std::string (sex == 'M' ? "male." : "female."); break;
                }
            }
          
            return os;
        }
};
