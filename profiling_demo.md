# Profiling Demo



## Sampling - gprof

## Sampling - MSVC

## Tracing - Tracy

## Performance Counters - Perf

* `perf list`

### Small Samples

`cd ~/perf-oriented-dev/small_samples/build`

* mmul
  - `perf stat -e L1-dcache-loads,L1-dcache-load-misses,cache-references,cache-misses ./mmul`
  - 2048, no padding
  ```
       27852691596      L1-dcache-loads:u                                           
       11378077032      L1-dcache-load-misses:u   #   40.85% of all L1-dcache accesses
       16279503294      cache-references:u                                          
        8288921218      cache-misses:u            #   50.916 % of all cache refs    

     196.449822711 seconds time elapsed
  ```
  - 2048, padding
  ```
       25799085648      L1-dcache-loads:u                                           
       10807949814      L1-dcache-load-misses:u   #   41.89% of all L1-dcache accesses
       16958029463      cache-references:u                                          
         565346689      cache-misses:u            #    3.334 % of all cache refs    

      29.378866348 seconds time elapsed
  ```
  - 2048, transposed, no padding
  ```
       27610235384      L1-dcache-loads:u                                           
       11795048470      L1-dcache-load-misses:u   #   42.72% of all L1-dcache accesses
       17458727341      cache-references:u                                          
        7790688807      cache-misses:u            #   44.623 % of all cache refs    

     185.321371785 seconds time elapsed
  ```
  - 2048, transposed, padded
  ```
       25815093115      L1-dcache-loads:u                                           
       10800418191      L1-dcache-load-misses:u   #   41.84% of all L1-dcache accesses
       16729212552      cache-references:u                                          
           7387949      cache-misses:u            #    0.044 % of all cache refs    

       8.228864283 seconds time elapsed
  ```
  - 2048, tiled, padded
  ```
       27988147620      L1-dcache-loads:u                                           
         481439069      L1-dcache-load-misses:u   #    1.72% of all L1-dcache accesses
        1205792770      cache-references:u                                          
          46977119      cache-misses:u            #    3.896 % of all cache refs    

       4.226429780 seconds time elapsed
  ```
  - vectorization
    * `clang -Rpass-missed=loop-vectorize  -mavx2 -fopenmp  -Ofast -DNDEBUG -Wall -Wextra -Wpedantic -Werror -MD -MT CMakeFiles/mmul.dir/mmul/mmul.c.o -MF CMakeFiles/mmul.dir/mmul/mmul.c.o.d -o CMakeFiles/mmul.dir/mmul/mmul.c.o -c /home/peter/perf-oriented-dev/small_samples/mmul/mmul.c && /usr/bin/cc -O3 -DNDEBUG  CMakeFiles/mmul.dir/mmul/mmul.c.o -o mmul`
    * 

* delannoy
  - `cd ~/perf-oriented-dev/small_samples/build`
  - `perf stat ./delannoy 14`
  - `perf stat -e L1-dcache-load-misses,L1-dcache-loads ./delannoy 14`

### Large Samples

* npb
  - `cd ~/perf-oriented-dev/larger_samples/npb_bt/build`
  - `perf stat ./npb_bt_a`
  - `perf stat -e L1-dcache-load-misses ./npb_bt_a`
  - `perf stat -e L1-dcache-load-misses,L1-dcache-loads ./npb_bt_a`
  - `perf stat -e L1-dcache-load-misses,L1-dcache-loads ./npb_bt_s`

* ssca2
  - `cd ~/perf-oriented-dev/larger_samples/ssca2/build`
  - `perf stat ./ssca2 17`
  - `perf stat -e L1-dcache-load-misses,L1-dcache-loads ./ssca2 17`

## Sampling + Flame Graph - Perf

* npb
  - `cd ~/perf-oriented-dev/larger_samples/npb_bt/build`
  - `perf record -F 99 -g -- ./npb_bt_a`
  - `perf script | ~/FlameGraph/stackcollapse-perf.pl > out.perf-folded`
  - `~/FlameGraph/flamegraph.pl out.perf-folded > perf.svg`

## Memory Usage - Massif
