sudo perf stat -r 100 -e cycles:u -e instructions:u ./raii.out # A hundred runs

 Performance counter stats for './raii.out' (100 runs):

          55625265      cpu_atom/cycles:u/                                                      ( +-  4,23% )  (3,01%)
          88203554      cpu_core/cycles:u/                                                      ( +-  0,63% )  (96,99%)
         146761554      cpu_atom/instructions:u/         #    2,64  insn per cycle              ( +-  5,46% )  (3,01%)
         415184020      cpu_core/instructions:u/         #    4,71  insn per cycle              ( +-  0,75% )  (96,99%)

          0,067952 +- 0,000481 seconds time elapsed  ( +-  0,71% )

sudo perf stat -r 100 -e cycles:u -e instructions:u ./manualdrop.out # A hundred runs

 Performance counter stats for './manualdrop.out' (100 runs):

          50498714      cpu_atom/cycles:u/                                                      ( +-  4,56% )  (11,49%)
          82746250      cpu_core/cycles:u/                                                      ( +-  0,56% )  (88,51%)
         130656156      cpu_atom/instructions:u/         #    2,59  insn per cycle              ( +-  6,08% )  (11,49%)
         405219721      cpu_core/instructions:u/         #    4,90  insn per cycle              ( +-  0,66% )  (88,51%)

          0,065743 +- 0,000425 seconds time elapsed  ( +-  0,65% )

