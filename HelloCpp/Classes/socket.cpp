#include "list.h"
#include <iostream>

#include "HelloWorldScene.h"

USING_NS_CC;

ODSocket::ODSocket(SOCKET sock)
{
	m_sock = sock;
}

ODSocket::~ODSocket()
{

}

int ODSocket::Init()
{
#ifdef WIN32
	WSADATA wsa;
	
	WORD version = MAKEWORD(2,0);

	int ret = WSAStartup(version, &wsa);

	if ( ret )
	{   
		std::cerr << "Initilize winsock error !" << std::endl;
		return -1;  
	}  


#endif // WIN32
	return 0;
}

int ODSocket::Clean()
{
#ifdef WIN32
	return WSACleanup();
#endif
	return 0;
}

bool ODSocket::Create(int af, int type, int protocol /* = 0 */)
{
	m_sock = socket(af, type, protocol);

	if (m_sock == INVALID_SOCKET)
	{
		return false;
	}

	return true;
}

bool ODSocket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;   
	svraddr.sin_family = AF_INET;  
	svraddr.sin_addr.s_addr = inet_addr(ip);  
	svraddr.sin_port = htons(port);   

	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));  

	if ( ret == SOCKET_ERROR )
	{   
		return false;  
	}  

	return true;  
}

bool ODSocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;  
	svraddr.sin_family = AF_INET;  
	svraddr.sin_addr.s_addr = INADDR_ANY;  
	svraddr.sin_port = htons(port);    

	int opt =  1;  

	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 )    
		return false;    

	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));  
	if ( ret == SOCKET_ERROR )
	{   
		return false;  
	}  

	return true; 
}

bool ODSocket::Listen(int backlog) 
{  
	int ret = listen(m_sock, backlog);  
	if ( ret == SOCKET_ERROR )
	{   
		return false;  
	}  
	return true; 
}    

bool ODSocket::Accept(ODSocket&  s, char* fromip /* = NULL */) 
{  
	struct sockaddr_in cliaddr;  
	socklen_t addrlen = sizeof(cliaddr);  
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);  

	if ( sock == SOCKET_ERROR )
	{   
		return false;  
	}    

	s = sock;  

	if ( fromip != NULL )   
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));    

	return true; 
}    

int ODSocket::Send(const char* buf, int len, int flags) 
{  
	int bytes;  
	int count = 0;    

	while ( count < len )
	{     
		bytes = send(m_sock, buf + count, len - count, flags);   

		if ( bytes == -1 || bytes == 0 )    
			return -1;   

		count += bytes;  
	}     

	return count; 
}    

int ODSocket::Recv(char* buf, int len, int flags) 
{  
	return (recv(m_sock, buf, len, flags)); 
}    

int ODSocket::Close() 
{ 
#ifdef WIN32  
	return (closesocket(m_sock)); 
#else  
	return (close(m_sock)); 
#endif 
}    

int ODSocket::GetError() 
{ 
#ifdef WIN32  
	return (WSAGetLastError()); 
#else 
	return 0;
	//return (errno); 
#endif 
}    



void* test_thread(void* arge)
{
	int ret, nLeft, length;
	SOCKET sListen, sServer; //�����׽��֣������׽���
	struct sockaddr_in saServer, saClient; //��ַ��Ϣ   
	char *ptr;//���ڱ�����Ϣ��ָ��   
	std::string str;
	//����Socket,ʹ��TCPЭ��
	sListen=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//�������ص�ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(9907); //ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.s_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ

	//��
	ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));

	//������������
	ret = listen(sListen, 5);

	printf("Waiting for client connecting!\n");
	printf("Tips: Ctrl+c to quit!\n");
	//�����ȴ����ܿͻ�������
	

	int len;
	while(1)//ѭ�������ͻ��ˣ���Զ��ֹͣ�����ԣ��ڱ���Ŀ�У�����û����������
	{
		length = sizeof(saClient);
		sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
		
		char sendMessage[]="hello client";  //������Ϣ���ͻ���  
		send(sServer,sendMessage,strlen(sendMessage)+1,0);  

		char receiveMessage[5000];

		nLeft = sizeof(receiveMessage);
		ptr = (char *)&receiveMessage;

		while((len=recv(sServer,ptr, BUFSIZ,0))>0)  
		{  
			ptr[len]='/0';  
			printf("receive message:%s\n", receiveMessage); 
			singletion::Instance()->add(std::string(ptr));
			do 
			{
				str = singletion::Instance()->getResult();
			} while (str == "");
			
			if(send(sServer,ptr,len,0)<0)  
			{  
				perror("write");  
				return NULL;  
			}  
		}  		
	} 

	ODSocket::Clean();

	return NULL;
}

SocketThread* SocketThread::_instance = NULL;

SocketThread::SocketThread()
{

}

SocketThread::~SocketThread()
{

}

SocketThread* SocketThread::Instance()
{
	if (_instance == NULL)
	{
		_instance = new SocketThread();
	}
	return _instance;
}

void SocketThread::Release()
{
	if (_instance != NULL)
	{
		delete _instance;
	}
}

void* SocketThread::start_thread(void* agre)
{
	SocketThread* t = (SocketThread*)agre;
	ODSocket s;
	s.Init();


	return NULL;
}

