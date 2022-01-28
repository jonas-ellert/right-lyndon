#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

// at all times, by design of the program, it holds i < j

template <std::integral intX = uint32_t>
auto right_lyndon_extension_improved(std::integral auto const* text,
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

  // auxiliary information required by the algorithm
  struct aux_type {
    intX max_lce = 0;
    intX left = 0;
  };
  std::vector<aux_type> aux(n + 1);

  // access functions (syntactic sugar)
  auto nss = [&res](size_t const idx) -> intX& { return res[idx].nss; };
  auto nlce = [&res](size_t const idx) -> intX& { return res[idx].nss_lce; };
  auto pss = [&res](size_t const idx) -> intX& { return res[idx].pss; };
  auto plce = [&res](size_t const idx) -> intX& { return res[idx].pss_lce; };
  auto max_lce = [&aux](size_t const idx) -> intX& { return aux[idx].max_lce; };
  auto left = [&aux](size_t const idx) -> intX& { return aux[idx].left; };

  // add sentinel symbol at the end
  auto S = [&](size_t const idx) {
    return (idx < n) ? (int64_t)text[idx] : std::numeric_limits<int64_t>::min();
  };

  // update the maximum lce of position j if necessary
  auto update_max_lce = [&](intX i, intX j, intX lce) {
    // check if this is a new maximum, or if this is the first lce(_,j)
    if (lce > max_lce(j) || left(j) == 0) {
      max_lce(j) = lce;
      left(j) = i;
    }
  };

  // We assign each NSS (respectively PSS) value together with its associated
  // LCE value. For each assignment, we check if this is the maximum LCE value
  // with right-hand side j, and update the max-lce if necessary.
  auto assign_pss = [&](intX i, intX j, intX lce) {
    pss(j) = i;
    plce(j) = lce;
    update_max_lce(i, j, lce);
  };
  auto assign_nss = [&](intX i, intX j, intX lce) {
    nss(i) = j;
    nlce(i) = lce;
    update_max_lce(i, j, lce);
  };

  // This is the extension technique described in the paper.
  auto extend = [&](intX const i, intX const j, intX& lce) {
    // If S(i) and S(j) are different, then we immediately return with LCE 0
    if (S(i) != S(j)) {
      lce = 0;
      return;
    }

    intX const d = j - i;
    intX rr = j + std::max((intX)1, max_lce(j));  // qj in the paper
    intX lr = rr - d;                             // qi in the paper
    intX extend = 0;

    while (S(lr) == S(rr)) {
      intX rl = left(rr);
      intX rlce = max_lce(rr);

      intX ll = rl - d;
      intX llce;

      // case 1:
      if (ll == i && lr == j) {  // implies rr = j + d
        llce = rlce + d;
      }
      // case 2:
      else if (nss(ll) == lr) {
        llce = nlce(ll);
      } else if (pss(lr) == ll) {
        llce = plce(lr);
      }
      // case 3:
      else {
        llce = plce(lr);
        rlce = nlce(pss(lr) + d);
      }

      extend = std::min(llce, rlce);
      if (extend < max_lce(rr)) break;

      extend = 0;
      rr = rr + std::max((intX)1, max_lce(rr));
      lr = rr - d;
    }

    lce = rr - j + extend;
  };

  // This is the main loop of the algorithm, see algorithm LceLyn in the paper
  for (int64_t i = n - 1; i >= 0; --i) {
    intX j = i + 1;
    intX lce = (S(i) == S(j)) ? (max_lce(j + 1) + 1) : ((intX)0);

    while (S(j + lce) > S(i + lce)) {
      assign_pss(i, j, lce);

      if (lce > nlce(j)) {
        lce = nlce(j);
        j = nss(j);
      } else if (lce < nlce(j)) {
        j = nss(j);
      } else {
        j = nss(j);
        extend(i, j, lce);
      }
    }

    assign_nss(i, j, lce);
  }

  return res;
}
