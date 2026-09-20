#ifndef SEARCH_UTILS_H
#define SEARCH_UTILS_H

#include <vector>
#include <algorithm>

// Generic Function Template for searching items in any standard container based on a predicate
template <typename Container, typename Predicate>
Container searchItems(const Container& container, Predicate pred) {
    Container results;
    for (const auto& item : container) {
        if (pred(item)) {
            results.push_back(item);
        }
    }
    return results;
}

// Generic Class Template for search and filter operations across entity vectors
template <typename T>
class SearchEngine {
public:
    // Filter items matching a condition
    template <typename Predicate>
    static std::vector<T> filter(const std::vector<T>& items, Predicate pred) {
        std::vector<T> matches;
        for (const auto& item : items) {
            if (pred(item)) {
                matches.push_back(item);
            }
        }
        return matches;
    }

    // Find first item matching a condition
    template <typename Predicate>
    static bool findFirst(const std::vector<T>& items, Predicate pred, T& outResult) {
        auto it = std::find_if(items.begin(), items.end(), pred);
        if (it != items.end()) {
            outResult = *it;
            return true;
        }
        return false;
    }
};

#endif // SEARCH_UTILS_H
