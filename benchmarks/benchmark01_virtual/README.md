sudo perf stat -r 100 -e cycles:u -e instructions:u ./raii.out # A hundred runs with -r 100

 Performance counter stats for './raii.out' (100 runs):

          56391076      cpu_atom/cycles:u/                                                      ( +-  5,43% )  (2,56%)
          86838493      cpu_core/cycles:u/                                                      ( +-  0,49% )  (97,44%)
         139283621      cpu_atom/instructions:u/         #    2,47  insn per cycle              ( +-  7,57% )  (2,56%)
         406888966      cpu_core/instructions:u/         #    4,69  insn per cycle              ( +-  0,60% )  (97,44%)

          0,068489 +- 0,000504 seconds time elapsed  ( +-  0,74% )

sudo perf stat -r 100 -e cycles:u -e instructions:u ./manualdrop.out # A hundred runs -r 100

 Performance counter stats for './manualdrop.out' (100 runs):

          53880216      cpu_atom/cycles:u/                                                      ( +-  5,35% )  (32,82%)
          82116212      cpu_core/cycles:u/                                                      ( +-  0,57% )  (67,18%)
         131816809      cpu_atom/instructions:u/         #    2,45  insn per cycle              ( +-  7,67% )  (32,82%)
         396484880      cpu_core/instructions:u/         #    4,83  insn per cycle              ( +-  0,51% )  (67,18%)

          0,066746 +- 0,000510 seconds time elapsed  ( +-  0,76% )

