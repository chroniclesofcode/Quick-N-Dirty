# Quick-N-Dirty
Simple, quick-n-dirty implementations of *things*. Mainly out of interest to see how things work, so no quality guaranteed. If you've read the code and for some weird reason want to use one of these programs, feel free to just copy it. 

Every folder will contain a header file with the class and a main.cpp which will run some tests for simple benchmarking. The header file will contain everything you need, and will be under the namespace `qnd`. So a hashmap would be `qnd::hashmap`.

To run one of the files, just use the executable supplied in the repository, which will compile it using g++ and provide a benchmark of the time taken to execute the tests.

`./run [foldername]`

for example if I wanted to run the hashmap tests, it would be `./run hashmap`.
