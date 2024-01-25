# Quick-N-Dirty
Simple, quick-n-dirty implementations of *things*. Mainly out of interest to see how things work, so no quality guaranteed. If you've read the code and for some weird reason want to use one of these programs, feel free to just copy it. 

Every folder will contain a header file with the class and a main.cpp which will run some tests for simple benchmarking. The header file will contain everything you need, and will be under the namespace `qnd`. So a hashmap would be `qnd::hashmap`.

To run one of the files, just use the executable supplied in the repository, which will compile it using g++ and provide a benchmark of the time taken to execute the tests.

`./run [foldername]`

for example if I wanted to run the hashmap tests, it would be `./run hashmap`.

# Benchmarks

Benchmarks are included in a sub-folder named /stats within each qnd project. For example, the stats of hashmap will be located in hashmap/stats/*.txt

Please note: to work out how the benchmarks work, look inside the main.cpp. There will be a testing section which will show what we are doing, and what we are comparing it to (reading the README.md also should work, if it exists). 

The timing function is found in /timer at `qnd::timer`. It provides timing capabilities, file-writing capabilities and calculating of basic statistics such as mean, standard deviation, variance and min/maxes.
