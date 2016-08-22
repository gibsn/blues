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


static void InitializeCmdParser(ArgvParser &cmd, int argc, char **argv)
{
    cmd.setIntroductoryDescription("Blues programming language");
    cmd.setHelpOption("h", "help", "Print this help message");
    // cmd.defineOption("ip", "IP address of the server",
    //     ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    // cmd.defineOption("port", "Port of the server",
    //     ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    // cmd.defineOption("mode", "Path to the file with the strategy for robot",
    //     ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    // cmd.defineOption("name", "Name of the robot",
    //     ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    //
    // cmd.defineOptionAlternative("ip", "i");
    // cmd.defineOptionAlternative("port", "p");
    // cmd.defineOptionAlternative("mode", "m");
    // cmd.defineOptionAlternative("name", "n");
}


bool Maintenance::ProcessCmdArguments(int argc, char **argv)
{
	ArgvParser cmd;

	InitializeCmdParser(cmd, argc, argv);

	int result = cmd.parse(argc, argv);
	if (result) {
        printf("%s\n", cmd.parseErrorDescription(result).c_str());
		return true;
	}

    //
	// strcpy(ip, cmd.optionValue("ip").c_str());
	// port = atoi(cmd.optionValue("port").c_str());
	// strcpy(program, cmd.optionValue("mode").c_str());
	// strcpy(name, cmd.optionValue("name").c_str());

	return false;
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
	close(0);
}


bool IsDigit(int c)
{
	return (c >= '0') && (c <= '9');
}


bool IsSeparator(int c)
{
	switch(c)
	{
		case'+':
		case'-':
		case'*':
		case'/':
		case'%':
		case'<':
		case'>':
		case'=':
		case'&':
		case'|':
		case'!':
		case',':
		case':':
		case'[':
		case']':
		case'{':
		case'}':
		case'(':
		case')':
		case'#':
			return true;
		default:
			return false;
	}
}


bool IsLetter(int c)
{
	return  ((c >= 'a') && (c <= 'z')) ||
			((c >= 'A') && (c <= 'Z'));
}


bool IsID(int c)
{
	return (c == '?') ||
		   (c == '@') ||
		   (c == '$');
}


bool IsComparison(char *str)
{
	if (!strcmp("<",  str) ||
		!strcmp(">",  str) ||
		!strcmp("=", str) )
		return true;
	else
		return false;
}


