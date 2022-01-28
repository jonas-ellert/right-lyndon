#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

// at all times, by design of the program, it holds i < j

template <std::integral intX = uint32_t>
auto right_lyndon_extension_naive(std::integral auto const* text,
                                  size_t const n) {
  //
  // the integer type has to be wide enough!
  if (n > std::numeric_limits<intX>::max()) {
    std::abort();
  }

  // the result: NSS and PSS array, each annotated with LCEs
  struct xss_type {
    intX nss;
    intX pss;
    intX nss_lce = 0;
    intX pss_lce = 0;
  };
  std::vector<xss_type> res(n, {(intX)n, (intX)n});

  // access functions (syntactic sugar)
  auto nss = [&res](size_t const idx) -> intX& { return res[idx].nss; };
  auto nlce = [&res](size_t const idx) -> intX& { return res[idx].nss_lce; };
  auto pss = [&res](size_t const idx) -> intX& { return res[idx].pss; };
  auto plce = [&res](size_t const idx) -> intX& { return res[idx].pss_lce; };

  // add sentinel symbol at the end
  auto S = [&](size_t const idx) {
    return (idx < n) ? (int64_t)text[idx] : std::numeric_limits<int64_t>::min();
  };

  // We assign each NSS (respectively PSS) value together with its associated
  // LCE value.
  auto assign_pss = [&](intX i, intX j, intX lce) {
    pss(j) = i;
    plce(j) = lce;
  };
  auto assign_nss = [&](intX i, intX j, intX lce) {
    nss(i) = j;
    nlce(i) = lce;
  };

  // Naive extension
  auto scan_lce = [&](intX const i, intX const j, intX& lce) {
    while (S(i + lce) == S(j + lce)) ++lce;
  };

  // This is the main loop of the algorithm, see algorithm LceLyn in the paper
  for (int64_t i = n - 1; i >= 0; --i) {
    intX j = i + 1;
    intX lce = (S(i) == S(j))
                   ? ((intX)1 + ((nss(j) == j + 1) ? nlce(j) : plce(j + 1)))
                   : ((intX)0);

    while (S(j + lce) > S(i + lce)) {
      assign_pss(i, j, lce);

      if (lce > nlce(j)) {
        lce = nlce(j);
        j = nss(j);
      } else if (lce < nlce(j)) {
        j = nss(j);
      } else {
        j = nss(j);
        scan_lce(i, j, lce);
      }
    }

    assign_nss(i, j, lce);
  }

  return res;
}
