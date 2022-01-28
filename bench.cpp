#include "right-lyndon-extension-improved.hpp"
#include "right-lyndon-extension-naive.hpp"
#include "right-lyndon-naive.hpp"
#include "right-lyndon.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>

struct timer {
  using millisec = std::chrono::milliseconds;

  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end;

  void start() { begin = std::chrono::steady_clock::now(); }
  void stop() { end = std::chrono::steady_clock::now(); }

  uint64_t millis() const {
    return std::chrono::duration_cast<millisec>(end - begin).count();
  }

  double mibs(uint64_t const n) const {
    auto ms = millis();
    double const s = ms / 1000.0;
    double const mib = n / 1024.0 / 1024.0;
    return mib / s;
  }

  static auto measure(std::string s, uint64_t n, auto &&function) {
    std::cout << s << " start!" << std::endl;
    timer t;
    t.start();
    decltype(function()) result = function();
    t.stop();
    std::cout << s << " time: " << t.millis() << "[ms]"
              << " = " << t.mibs(n) << "mibs" << std::endl;
    return result;
  };
};

int main(int argc, char *argv[]) {
  if (argc != 3 && argc != 2) {
    std::cerr << "Wrong number of parameters." << std::endl;
    std::cerr << "First parameter: path to input file (mandatory)."
              << std::endl;
    std::cerr << "Second parameter: prefix size in bytes (optinal)."
              << std::endl;
    return -1;
  }

  std::vector<uint8_t> string;
  uint64_t const max_size =
      (argc == 3) ? std::stoll(argv[2]) : std::numeric_limits<uint64_t>::max();

  // read file
  std::ifstream t(argv[1]);
  t.seekg(0, std::ios::end);
  uint64_t const n = std::min((uint64_t)t.tellg(), max_size);
  t.seekg(0, std::ios::beg);
  string.resize(n);
  std::copy_n((std::istreambuf_iterator<char>(t)), n, string.begin());

  std::vector<uint32_t> nss(n);

  std::cout << "Ready!" << std::endl;
  std::cout << "string = " << argv[1] << std::endl;
  std::cout << "     n = " << n << std::endl;
  std::cout << std::endl;

  std::cout << "\nFirst 800:\n";
  for (size_t i = 0; i < std::min((uint64_t)800, n); ++i)
    std::cout << ((i % 80) ? "" : "\n") << (char)string[i];
  std::cout << std::endl << std::endl;

  {
    auto result = timer::measure("Extension linear", n, [&]() {
      return right_lyndon(string.data(), string.size());
    });
    for (size_t i = 0; i < n; ++i) nss[i] = result[i].nss;
    std::cout << std::endl;
  }

  {
    auto result = timer::measure("Extension improved", n, [&]() {
      return right_lyndon_extension_improved(string.data(), string.size());
    });
    for (size_t i = 0; i < n; ++i)
      if (nss[i] != result[i].nss) std::exit(-1);  // sanity check
    std::cout << "Results consistent.\n" << std::endl;
  }

  {
    auto result = timer::measure("Extension naive", n, [&]() {
      return right_lyndon_extension_naive(string.data(), string.size());
    });
    for (size_t i = 0; i < n; ++i)
      if (nss[i] != result[i].nss) std::exit(-1);  // sanity check
    std::cout << "Results consistent.\n" << std::endl;
  }

  {
    auto result = timer::measure("Naive (no extension)", n, [&]() {
      return right_lyndon_naive(string.data(), string.size());
    });
    for (size_t i = 0; i < n; ++i)
      if (nss[i] != result[i].nss) std::exit(-1);  // sanity check
    std::cout << "Results consistent." << std::endl;
  }

  return 0;
}
