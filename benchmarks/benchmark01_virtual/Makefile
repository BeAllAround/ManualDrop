run_benchmark:
	g++ main01.cpp -o raii.out -O3
	g++ main02.cpp -o manualdrop.out -O3
	sudo taskset -c 2 perf stat -r 100 ./raii.out
	sudo taskset -c 2 perf stat -r 100 ./manualdrop.out

# sudo taskset -c 2 perf stat -r 100 -e cycles:u -e instructions:u ./raii.out