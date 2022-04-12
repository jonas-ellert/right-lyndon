# Right-Lyndon

This repository contains a series of algorithms that compute the Lyndon table in a back-to-front online manner. The full description and analysis of these algorithms can be found in:

---

**Back-to-Front Online Lyndon Forest Construction.**  
*G. Badkobeh, M. Crochemore, J. Ellert, and C. Nicaud.*  
Accepted at the 33rd Annual Symposium on Combinatorial Pattern Matching (CPM 2022).  
(to appear in the conference proceedings)  

---




The repository provides four different algorithms. The first three algorithms take worst-case quadratic time in the length of the input string. The last algorithm takes worst-case linear time in the length of the input string. Section numbers refer to the CPM paper.

* The simple algorithm NaiveLyn as described in Section 4.1, see file ```right-lyndon-naive.hpp```
* The algorithm LceLyn as described in Section 4.2, see file ```right-lyndon-extension-naive.hpp```
* The algorithm LceLyn with the extension technique described in Section 4.3, see file ```right-lyndon-extension-improved.hpp```
* The algorithm LceLyn with the extension technique and modification thereof described in Section 4.4 and Appendix B, see file ```right-lyndon-extension-linear.hpp```


**Note that a c++20 compiler is required!** Possible compiler configuration:

```
g++ -std=c++2a -O3 -funroll-loops -march=native -Wall -pedantic bench.cpp
```
