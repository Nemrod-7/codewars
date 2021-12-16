#include <iostream>
#include <functional>

template<class T> class MagicCompare {

    private :
        T value, prev;
        bool state;
    public :
        MagicCompare (const T& x = 0, bool flag = true) : value(x), prev(x), state(flag) {};

        operator bool()  { return state; }
        operator T()  { return value; }

        MagicCompare<T> compare (std::function<bool(T lhs, T rhs)> comparator, const MagicCompare<T> &rhs) {
            MagicCompare<T> curr = *this;

            if (!(comparator (this->value, rhs.prev))) {
                curr.state = false;
            }

            curr.value = rhs.value;
            return curr;
        }

        MagicCompare<T> operator ++() { return this->operator += (1); }
        MagicCompare<T> operator ++(int) {
            MagicCompare<T> prev(this->value);
            ++value;
            return prev;
        }

        template <class R> MagicCompare<T> operator += (R x) {
            value += x;
            return *this;
        }
        template <class R> MagicCompare<T> operator *(R x) {
            MagicCompare<T> curr = x;
            curr.state = state;
            return value * curr.value;
        }

        template<class R> MagicCompare<T> operator <(R rhs) {
            return compare (std::less<T>(), MagicCompare<T>(rhs));
        }
        template<class R> MagicCompare<T> operator >(R rhs) {
            return compare(std::greater<T>(), MagicCompare<T>(rhs));
        }
        template<class R> MagicCompare<T> operator <=(R rhs) {
            return compare(std::less_equal<T>{}, MagicCompare<T>(rhs));
        }
        template<class R> MagicCompare<T> operator >=(R rhs) {
            return compare(std::greater_equal<T>{}, MagicCompare<T>(rhs));
        }
        template<class R> MagicCompare<T> operator ==(R rhs) {
            return compare(std::equal_to<T>{}, MagicCompare<T>(rhs));
        }
        template<class R> MagicCompare<T> operator !=(R rhs) {
            return compare(std::not_equal_to<T>{}, MagicCompare<T>(rhs));
        }
};

template<class T> MagicCompare<T> operator<(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T>(lhs) < rhs;
}
template<class T> MagicCompare<T> operator>(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T>(lhs) > rhs;
}
template<class T> MagicCompare<T> operator<=(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T>(lhs) <= rhs;
}
template<class T> MagicCompare<T> operator>=(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T>(lhs) >= rhs;
}
template<class T> MagicCompare<T> operator==(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T>(lhs) == rhs;
}
template<class T> MagicCompare<T> operator!=(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T>(lhs) != rhs;
}
template<class T> MagicCompare<T> operator*(T lhs, const MagicCompare<T> &rhs) {
    return MagicCompare<T> (lhs) * rhs;
}


int main () {

    using Integer = MagicCompare<int>;
    using Float = MagicCompare<double>;

    for (Float x = 0; x < 1; x += 0.01) {
        //std::cout << x << " ";
    }
}
