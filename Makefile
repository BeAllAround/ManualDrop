run_tests:
	g++ main.cpp -o out.out -Og --std=c++11 && valgrind -s --track-origins=yes ./out.out
	g++ main.cpp -o out.out -O0 --std=c++11 && valgrind -s --track-origins=yes ./out.out
	g++ main.cpp -o out.out -O1 --std=c++11 && valgrind -s --track-origins=yes ./out.out
	g++ main.cpp -o out.out -O2 --std=c++11 && valgrind -s --track-origins=yes ./out.out
	g++ main.cpp -o out.out -O3 --std=c++11 && valgrind -s --track-origins=yes ./out.out
