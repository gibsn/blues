#ifndef COMMON_H_SENTRY
#define COMMON_H_SENTRY


enum { buffer_size = 2048 };

class Maintenance
{
	char program[buffer_size];

public:
	bool ProcessCmdArguments(int argc, char **argv);
	void RedirectToProg();
	~Maintenance();
};


bool IsDigit(int c);
bool IsSeparator(int c);
bool IsLetter(int c);
bool IsID(int c);
bool IsComparison(char *str);


#endif
