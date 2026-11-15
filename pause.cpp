#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using Interval = std::pair<long long, long long>;

// Returns free intervals in [0, duration), assuming busy intervals are
// expressed as half-open intervals [start, end).
std::vector<Interval> free_slots(long long duration,
                                 std::vector<Interval> busy) {
    std::vector<Interval> gaps;

    if (duration <= 0) {
        return gaps;
    }

    // Clip intervals to the scheduling horizon and discard empty/invalid ones.
    std::vector<Interval> clipped;
    clipped.reserve(busy.size());

    for (const auto& [start, end] : busy) {
        const long long clipped_start = std::max(0LL, start);
        const long long clipped_end = std::min(duration, end);

        if (clipped_start < clipped_end) {
            clipped.emplace_back(clipped_start, clipped_end);
        }
    }

    std::sort(clipped.begin(), clipped.end());

    long long cursor = 0;
    for (const auto& [start, end] : clipped) {
        if (start > cursor) {
            gaps.emplace_back(cursor, start);
        }

        // Merge overlapping and adjacent busy intervals.
        cursor = std::max(cursor, end);
        if (cursor >= duration) {
            break;
        }
    }

    if (cursor < duration) {
        gaps.emplace_back(cursor, duration);
    }

    return gaps;
}

int main() {
    long long duration;
    int count;

    if (!(std::cin >> duration >> count) || count < 0) {
        return 1;
    }

    std::vector<Interval> busy(count);
    for (auto& [start, end] : busy) {
        if (!(std::cin >> start >> end)) {
            return 1;
        }
    }

    const auto gaps = free_slots(duration, busy);

    if (gaps.empty()) {
        std::cout << "-\n";
    } else {
        for (const auto& [start, end] : gaps) {
            std::cout << start << " " << end << '\n';
        }
    }

    return 0;
}
