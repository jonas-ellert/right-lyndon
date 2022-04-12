# Right-Lyndon

This repository contains a series of algorithms that compute the Lyndon table in a back-to-front online manner. The full description and analysis of these algorithms can be found in: (please cite if you use this repository in your research)

---

**Back-to-Front Online Lyndon Forest Construction.**  
*G. Badkobeh, M. Crochemore, J. Ellert, and C. Nicaud.*  
Accepted at the 33rd Annual Symposium on Combinatorial Pattern Matching (CPM 2022).  
(to appear in the conference proceedings)  

---




The repository provides four different algorithms. The first three algorithms take worst-case quadratic time in the length of the input string. The last algorithm takes worst-case linear time in the length of the input string. Section numbers refer to the CPM paper.

* The simple algorithm NaiveLyn as described in Section 4.1 is in file ```right-lyndon-naive.hpp```.
* The algorithm LceLyn as described in Section 4.2 is in file ```right-lyndon-extension-naive.hpp```.
* The algorithm LceLyn with the extension technique described in Section 4.3 is in file ```right-lyndon-extension-improved.hpp```.
* The algorithm LceLyn with the extension technique and modification thereof described in Section 4.4 and Appendix B is in file ```right-lyndon-extension-linear.hpp```.

The implementations are header-only! Simply include the respective header files in your project, and you are ready to use the algorithms (no linking required).
**Note that a c++20 compiler is required!** Possible compiler configuration:

```
g++ -std=c++2a -O3 -funroll-loops -march=native -Wall -pedantic bench.cpp
```
The above command compiles the included benchmark tool (```bench.cpp```). Afterwards, run ```./a.out /path/to/string.txt``` to execute the benchmark. This will run all algorithms for the specified file, and print the throughput on the terminal. As a sanity check, the tool checks whether all algorithms terminate with the same result. You can run the tool on a prefix of a large file by specifying the length of the desired prefix as follows: ```./a.out /path/to/string/dna.txt 42```. This will use only the first 42 symbols of the file as input.

Have fun, and feel free to contact the authors if you have any questions or comments!
