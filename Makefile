all:
	g++ servers/server_asio.cpp -o servers/server_asio -std=c++11 -lboost_system -lboost_serialization -pthread
	g++ servers/server_fork.cpp -o servers/server_fork -std=c++11 -lboost_system -lboost_serialization -pthread
	g++ servers/server_pthread.cpp -o servers/server_pthread -std=c++11 -lboost_system -lboost_serialization -pthread
	g++ client/client.cpp -o client/client -std=c++11 -lboost_system -lboost_serialization -pthread