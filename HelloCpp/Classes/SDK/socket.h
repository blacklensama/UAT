#pragma once

#if WIN32
#pragma comment(lib,"pthreadVCE2.lib")

#endif

#include <pthread.h>

#ifdef WIN32
#include <WinSock2.h>

typedef int socklen_t;

#else
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <netdb.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <arpa/inet.h>  
typedef int             SOCKET; 

#define INVALID_SOCKET  -1  
#define SOCKET_ERROR    -1  

#endif // WIN32

#ifdef WIN32  
#pragma comment(lib, "wsock32") 
#endif     

static pthread_t p1, p2;

void* test_thread(void* arge);

class ODSocket
{
public:
	ODSocket(SOCKET sock = INVALID_SOCKET);

	~ODSocket();

	bool Create(int af, int type, int protocol = 0);

	bool Connect(const char* ip, unsigned short port);

	bool Bind(unsigned short port);

	bool Listen(int backlog = 5);

	bool Accept(ODSocket& s, char* fromip = NULL);

	int Send(const char* buf, int len, int flag = 0);

	int Recv(char* buf, int len, int flags = 0);

	int Close();

	int GetError();

	static int Init();

	static int Clean();
protected:
	SOCKET m_sock;  
	fd_set  fdR;  
private:
};


class SocketThread
{
public:
	int start();
	ODSocket getSocket();
	void stop();
	static SocketThread* Instance();
	static void Release();
protected:
	ODSocket od;
	pthread_t pid;
	SocketThread();
	~SocketThread();
private:
	static void* start_thread(void*);
	static SocketThread* _instance;
};