#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "Common.h"

#include "argvparser.h"

using CommandLineProcessing::ArgvParser;


void ExtractFrom3rdSymbol(char *src, char *&dest)
{
	int i,j;

	dest=(char *)malloc(1);
	for (i=3, j=0; src[i]; i++, j++)
	{
		dest=(char *)realloc(dest,j+1);
		dest[j]=src[i];
	}
	dest=(char *)realloc(dest,j+1);
	dest[j]='\0';
}


void Maintenance::RealTimePrinting(const char *str)
{
	int i;

	for (i=0; str[i]; i++)
	{
		usleep(random() % 200000);
		putchar(str[i]);
		fflush(stdout);
	}
		usleep(random() % 1000000);
	putchar('\n');
}


void Maintenance::SendMessage(const char* str)
{
	RealTimePrinting(str);
	write(sockfd, str, strlen(str)+1);
}


Maintenance::Maintenance()
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		buf[0] = '\0';
		buf_length = 0;
	};


int CheckCmdErrors(int result, const ArgvParser &cmd)
{
    if (result)
    {
        printf("%s\n", cmd.parseErrorDescription(result).c_str());
        return 1;
    }

    if(atoi(cmd.optionValue("port").c_str()) == 0)
    {
    	printf("Please, enter valid port number\n");
    	return 1;
    }

    return 0;
}


int InitializeCmdParser(ArgvParser &cmd, int argc, char **argv)
{
    cmd.setIntroductoryDescription("Robot");
    cmd.setHelpOption("h", "help", "Print this help message");
    cmd.defineOption("ip", "IP address of the server",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("port", "Port of the server",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("mode", "Path to the file with the strategy for robot",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("name", "Name of the robot",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);

    cmd.defineOptionAlternative("ip", "i");
    cmd.defineOptionAlternative("port", "p");
    cmd.defineOptionAlternative("mode", "m");
    cmd.defineOptionAlternative("name", "n");

    return CheckCmdErrors(cmd.parse(argc, argv), cmd);
}


int Maintenance::ProcessCmdArguments(int argc, char **argv)
{
	ArgvParser cmd;

	if (InitializeCmdParser(cmd, argc, argv))
		return 1;

	strcpy(ip, cmd.optionValue("ip").c_str());
	port = atoi(cmd.optionValue("port").c_str());
	strcpy(program, cmd.optionValue("mode").c_str());
	strcpy(name, cmd.optionValue("name").c_str());

	return 0;
}


int Maintenance::GetSockFd()
{
	return sockfd;
}


int Maintenance::EstablishConnection()
{
	struct sockaddr_in addr;

	if(!inet_aton(ip, &addr.sin_addr))
	{
		fprintf(stderr, "Invalid IP, please try again\n\n");
		return -1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)))
	{
		perror("connect");
		fprintf(stderr, "Please, try again\n\n");
		return -1;
	}
	return 0;
}


void Maintenance::ShlBuf(int i, int count)
{
	int j,l;

	if (i<count-1)
		for (j=0; j<=i; j++)
			for (l=i-j+1; l<(count-j); l++)
				buf[l-1]=buf[l];
}


void Maintenance::ProcessSelect()
{
	int res, max_d;
	fd_set readfds;

	max_d=sockfd;
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);
	res=select(max_d+1, &readfds, NULL, NULL, NULL);
	if (res<0)
		perror("select");
	if (FD_ISSET(sockfd,&readfds))
	{
		if (ReadMessage()<=0)
		{
			fprintf(stderr, "You have been disconnected\n");
			exit(0);
		}
	}
}


int Maintenance::ExtractFromBuffer(char *dest)
{
	int i,j,found_end=0;

	for (i=0; i<buf_length; i++)
		if (!buf[i])
		{
			found_end=1;
			break;
		}
	if (found_end)
	{
		for (j=0; j<=i; j++)
			dest[j]=buf[j];
		ShlBuf(i, buf_length);
		buf_length -= i + 1;
		return 0;
	}
	return -1;
}


int Maintenance::ReadMessage()
{
	int count = 1, i;
	char tmp[buffer_size];

	count = read(sockfd, tmp, buffer_size-1);
	if (count == -1)
	{
		perror("read");
		return -1;
	}
	if (count == 0)
		return 0;
	for (i = 0; i < count; i++)
		buf[buf_length+i] = tmp[i];
	buf_length += count;
	return count;
}


char *Maintenance::GetName()
{
	return name;
}


void Maintenance::RedirectToProg()
{
	int fd;
	fd = open(program, O_RDONLY);
	if (fd < 0)
	{
		perror(program);
		exit(1);
	}
	dup2(fd, 0);
	close(fd);
}


Maintenance::~Maintenance()
{
	shutdown(sockfd,2);
	close(sockfd);
	close(0);
}
