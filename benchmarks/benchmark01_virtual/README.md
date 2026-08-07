sudo taskset -c 2 perf stat -r 100 -e cycles:u -e instructions:u ./raii.out

 Performance counter stats for './raii.out' (100 runs):

     <not counted>      cpu_atom/cycles:u/                                                      (0,00%)
          81947837      cpu_core/cycles:u/                                                      ( +-  0,10% )
     <not counted>      cpu_atom/instructions:u/                                                (0,00%)
         384757437      cpu_core/instructions:u/         #    4,70  insn per cycle              ( +-  0,00% )

          0,056722 +- 0,000134 seconds time elapsed  ( +-  0,24% )

sudo taskset -c 2 perf stat -r 100 -e cycles:u -e instructions:u ./manualdrop.out

 Performance counter stats for './manualdrop.out' (100 runs):

     <not counted>      cpu_atom/cycles:u/                                                      (0,00%)
          77071071      cpu_core/cycles:u/                                                      ( +-  0,07% )
     <not counted>      cpu_atom/instructions:u/                                                (0,00%)
         376757448      cpu_core/instructions:u/         #    4,89  insn per cycle              ( +-  0,00% )

          0,054394 +- 0,000109 seconds time elapsed  ( +-  0,20% )

FULL perf output:

sudo taskset -c 2 perf stat -r 100 ./raii.out

 Performance counter stats for './raii.out' (100 runs):

             68,59 msec task-clock                       #    1,041 CPUs utilized               ( +-  0,64% )
                 3      context-switches                 #   43,736 /sec                        ( +-  3,26% )
                 0      cpu-migrations                   #    0,000 /sec                      
             12421      page-faults                      #  181,080 K/sec                       ( +-  0,00% )
     <not counted>      cpu_atom/cycles/                                                        (0,00%)
         137621409      cpu_core/cycles/                 #    2,006 GHz                         ( +-  0,29% )
     <not counted>      cpu_atom/instructions/                                                  (0,00%)
         460589608      cpu_core/instructions/                                                  ( +-  0,00% )
     <not counted>      cpu_atom/branches/                                                      (0,00%)
         100335724      cpu_core/branches/               #    1,463 G/sec                       ( +-  0,00% )
     <not counted>      cpu_atom/branch-misses/                                                 (0,00%)
             29713      cpu_core/branch-misses/                                                 ( +-  0,67% )
             TopdownL1 (cpu_core)                 #     24,8 %  tma_backend_bound      
                                                  #      2,0 %  tma_bad_speculation    
                                                  #     19,0 %  tma_frontend_bound     
                                                  #     54,2 %  tma_retiring             ( +-  0,19% )

          0,065903 +- 0,000453 seconds time elapsed  ( +-  0,69% )

sudo taskset -c 2 perf stat -r 100 ./manualdrop.out

 Performance counter stats for './manualdrop.out' (100 runs):

             64,47 msec task-clock                       #    1,043 CPUs utilized               ( +-  0,34% )
                 3      context-switches                 #   46,531 /sec                        ( +-  3,27% )
                 0      cpu-migrations                   #    0,000 /sec                      
             12421      page-faults                      #  192,655 K/sec                       ( +-  0,00% )
     <not counted>      cpu_atom/cycles/                                                        (0,00%)
         132677800      cpu_core/cycles/                 #    2,058 GHz                         ( +-  0,28% )
     <not counted>      cpu_atom/instructions/                                                  (0,00%)
         452544027      cpu_core/instructions/                                                  ( +-  0,00% )
     <not counted>      cpu_atom/branches/                                                      (0,00%)
          97328914      cpu_core/branches/               #    1,510 G/sec                       ( +-  0,00% )
     <not counted>      cpu_atom/branch-misses/                                                 (0,00%)
             28840      cpu_core/branch-misses/                                                 ( +-  0,55% )
             TopdownL1 (cpu_core)                 #     26,7 %  tma_backend_bound      
                                                  #      2,2 %  tma_bad_speculation    
                                                  #     16,4 %  tma_frontend_bound     
                                                  #     54,7 %  tma_retiring             ( +-  0,28% )

          0,061792 +- 0,000242 seconds time elapsed  ( +-  0,39% )

