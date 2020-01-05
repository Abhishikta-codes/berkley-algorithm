CXX= g++
THR= lpthread
MAKE= make

compile : server.cpp client.cpp
	$(CXX) -o server server.cpp -$(THR)

	$(CXX) -o client client.cpp 
	
clean : 
	rm server client
	ls -ltr
	$(info CLEAN SUCCESSFUL!!!)

run_server : 
	./server

run_client :
	./client
