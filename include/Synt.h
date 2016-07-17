#ifndef SYNT_H_SENTRY
#define SYNT_H_SENTRY


class PolizElem;
struct PolizList;
struct Lexeme;


class SyntAnalyser
{
	Lexeme *current_lexeme;
	PolizList *poliz_head;
	PolizList *poliz_last;
	int brace_count;

	void S();
	void B();
	void Main();
	void Operator();
	void IntegratedOperators();
	void Prod();
	void Buy();
	void Sell();
	void Function();
	void If();
	void Assignment();
	void Goto();
	void Print();
	void PrintArgument();
	void Expression();
	void Expression1();
	void T();
	void F();
	void F1();
	void Variable();

	void GetNextLexeme();
	void AddNewPolizElem(PolizElem *new_elem);
	void LoadFirstLexeme(Lexeme *head);

	static void DeletePolizList(PolizList *head);

public:
	SyntAnalyser();
	void Run(Lexeme *head);
	PolizList *GetPolizHead() { return poliz_head; }
	~SyntAnalyser();
};


#endif
