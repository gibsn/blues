#ifndef LEX_H_SENTRY
#define LEX_H_SENTRY

#include "Common.h"
#include "Stack.h"


struct Lexeme;


enum lexeme_types
{
	num = 1,
	string = 2,
	identificator = 3,
	keyword = 4,
	comment = 5
};

struct Lexeme
{
	int type;
	int line_number;
	char *string;
	Lexeme *next;
};


class LexAnalyser
{
	Lexeme *lexemes;
	int state;
	char buf[buffer_size];
	int buf_index;
	Stack<Lexeme> stack_memory;
	int lines_count;

	static void FreeLexemesList(Lexeme *head);

	void SetState(int new_state) { state = new_state; }

	Lexeme *MakeNewLexeme(int type);
	void AnalyseZero(int c);
	void AnalyseNum(int c);
	void AnalyseString(int c);
	void AnalyseIdentificator(int c);
	void AnalyseKeyword(int c);
	void AnalyseComment(int c);
	void SwitchState(int c);
	void CopyStackToList();
	void PrintLexemes();

public:
	LexAnalyser();
	~LexAnalyser();

	void Run();
	Lexeme *GetLexemesHead() const { return lexemes; }
};


#endif
