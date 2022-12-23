#pragma once

template<typename T>
    T gcd(T a, T b) {
        return b == 0 ? a : gcd(b, a % b);
    }

template<typename T>
    T distance(T x1, T y1, T x2, T y2) {
        return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    }

template<typename T>
    T max(T a, T b) {
        return a > b ? a : b;
    }

template<typename T>
    T max(T a, T b, T c) {
        return max(max(a, b), c);
    }

template<typename T>
    T max(T a, T b, T c, T d) {
        return max(max(a, b), max(c, d));
    }

template<typename T>
    T min(T a, T b) {
        return a < b ? a : b;
    }

template<typename T>
    T min(T a, T b, T c) {
        return min(min(a, b), c);
    }

template<typename T>
    T min(T a, T b, T c, T d) {
        return min(min(a, b), min(c, d));
    }
