#ifndef COMMON_H_SENTRY
#define COMMON_H_SENTRY


enum { buffer_size = 2048 };

class Maintenance
{
	int port, sockfd, buf_length;
	char ip[16], buf[buffer_size], name[buffer_size], program[buffer_size];

	void ShlBuf(int i, int count);
public:
	int ProcessCmdArguments(int argc, char **argv);
	int EstablishConnection();
	void ProcessSelect();
	int ReadMessage();
	void SendMessage(const char* str);
	int ExtractFromBuffer(char *dest);
	void RealTimePrinting(const char *str);
	void RedirectToProg();
	char *GetName();
	int GetSockFd();
	Maintenance();
	~Maintenance();
};



#endif
