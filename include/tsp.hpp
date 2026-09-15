#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

struct Point {
    double x;
    double y;
};

inline double distance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// A tour is a permutation of valid city indices; include the closing edge.
inline double path_length(
    const std::vector<int>& path,
    const std::vector<Point>& cities
) {
    if (path.empty()) {
        return 0.0;
    }
    double length = 0.0;
    for (std::size_t i = 0; i + 1 < path.size(); ++i) {
        length += distance(cities[path[i]], cities[path[i + 1]]);
    }
    length += distance(cities[path.back()], cities[path.front()]);
    return length;
}

struct RandomPermutation {
    std::vector<int> path;

    template <class RNG>
    std::vector<int> operator()(RNG& G) {
        std::vector<int> x = path;
        std::shuffle(x.begin(), x.end(), G);
        return x;
    }
};
