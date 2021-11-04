class Sum {
    private :
        int m_data;
    public :
        Sum (int x) : m_data(x) {}

        Sum operator ()(int x) { return m_data + x; }
        operator int () { return m_data;}
        friend bool operator == (int n, const Sum &fn) { return fn.m_data == n;        }
};

auto add(int n) {
    return Sum(n);
}
