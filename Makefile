combine_o: combine.cpp dataset.hpp matrix.hpp sbmp.hpp
	g++ -O2 --std=c++14 combine.cpp -o ./combine
combine_g: combine.cpp dataset.hpp matrix.hpp sbmp.hpp
	g++ -g --std=c++14 combine.cpp -o ./combine

neural_o: neural.cpp neural.hpp dataset.hpp matrix.hpp sbmp.hpp
	g++ -O2 --std=c++14 neural.cpp -o ./neural
neural_g: neural.cpp neural.hpp dataset.hpp matrix.hpp sbmp.hpp
	g++ -g --std=c++14 neural.cpp -o ./neural
