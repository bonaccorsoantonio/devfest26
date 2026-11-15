#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using Event = std::pair<int, long long>;

// Assumes event costs are non-negative.
std::pair<int, int> longest_window(const std::vector<Event>& events,
                                   int limit,
                                   long long budget) {
    if (limit <= 0 || budget < 0) {
        return {0, 0};
    }

    std::unordered_map<int, int> counts;
    int left = 0;
    long long total = 0;
    long long best_cost = 0;
    std::pair<int, int> best{0, 0};

    for (int right = 0; right < static_cast<int>(events.size()); ++right) {
        const auto [category, cost] = events[right];

        ++counts[category];
        total += cost;

        // Shrink until both constraints are satisfied.
        while (left <= right &&
               (static_cast<int>(counts.size()) > limit || total > budget)) {
            const auto [old_category, old_cost] = events[left++];

            auto it = counts.find(old_category);
            if (--it->second == 0) {
                counts.erase(it);
            }

            total -= old_cost;
        }

        const int length = right - left + 1;
        const int best_length = best.second - best.first;

        if (length > best_length ||
            (length == best_length && length > 0 && total < best_cost)) {
            best = {left, right + 1};
            best_cost = total;
        }
    }

    return best;
}

int main() {
    int count, limit;
    long long budget;

    if (!(std::cin >> count >> limit >> budget) || count < 0) {
        return 1;
    }

    std::vector<Event> events(count);
    for (auto& [category, cost] : events) {
        if (!(std::cin >> category >> cost)) {
            return 1;
        }
    }

    const auto [start, end] = longest_window(events, limit, budget);
    std::cout << start << " " << end << '\n';
    return 0;
}
