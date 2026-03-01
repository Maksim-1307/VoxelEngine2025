#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>

class Profiler {
    std::string name;
    std::chrono::high_resolution_clock::time_point start;

    struct Stats {
        double total_ms = 0;
        int count = 0;
    };

    static std::unordered_map<std::string, Stats>& get_map() {
        static std::unordered_map<std::string, Stats> stats_map;
        return stats_map;
    }

public:
    Profiler(const std::string& name) : name(name) {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Profiler() {
        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end - start).count();
        
        auto& s = get_map()[name];
        s.total_ms += duration;
        s.count++;
    }

    static void print_results() {
        std::cout << "\n=== Profiler Results (Avg ms) ===\n";
        for (const auto& [name, s] : get_map()) {
            std::cout << name << ": " << (s.total_ms / s.count) << " ms (calls: " << s.count << ")\n";
        }
    }
};