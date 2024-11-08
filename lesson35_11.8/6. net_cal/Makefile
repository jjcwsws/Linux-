.PHONY:all
all:cal_server cal_client

cal_server:TcpServerMain.cc
	g++ -o $@ $^ -std=c++14 -lpthread -ljsoncpp
cal_client:TcpClientMain.cc
	g++ -o $@ $^ -std=c++14 -ljsoncpp
.PHONY:clean
clean:
	rm -f cal_server cal_client