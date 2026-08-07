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

