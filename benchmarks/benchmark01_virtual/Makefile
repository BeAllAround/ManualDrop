run_benchmark:
	g++ main01.cpp -o raii.out -O3
	g++ main02.cpp -o manualdrop.out -O3
	sudo perf stat -r 100 -e cycles:u -e instructions:u ./raii.out # A hundred runs with -r 100
	sudo perf stat -r 100 -e cycles:u -e instructions:u ./manualdrop.out # A hundred runs -r 100
