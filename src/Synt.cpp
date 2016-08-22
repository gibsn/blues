#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Exception.h"
#include "Interpretator.h"
#include "Poliz.h"
#include "Lex.h"
#include "LabelTable.h"

#include "Synt.h"


SyntAnalyser::SyntAnalyser()
	: current_lexeme(0),
	poliz_head(0),
	poliz_last(0),
	brace_count(0)
{
}


void SyntAnalyser::LoadFirstLexeme(Lexeme *head)
{
	if (head)
		current_lexeme = head;
	else
		throw EmptyInput();
}


void SyntAnalyser::GetNextLexeme()
{
	if ((current_lexeme->next) || (!brace_count))
		current_lexeme = current_lexeme->next;
	else
		throw UnexpectedEOF();
}


void SyntAnalyser::Run(Lexeme *head)
{
	try {
		LoadFirstLexeme(head);
		S();
	}
	catch(const Exception& info) {
		info.Print();
		exit(0);
	}
}


void SyntAnalyser::S()
{
	B();

	if (current_lexeme)
		throw NotWhatExpected("EOF", current_lexeme->string,
			current_lexeme->line_number);
}


void SyntAnalyser::B()
{
	if (strcmp("{", current_lexeme->string)) {
		if (!brace_count)
			throw NotWhatExpected("{", current_lexeme->string,
				current_lexeme->line_number);
		else
			throw UnknownLexeme(current_lexeme->string,
				current_lexeme->line_number);
	}

	brace_count++;
	GetNextLexeme();
	Main();

	while(strcmp("}", current_lexeme->string))
		Main();

	brace_count--;
	GetNextLexeme();
}


void SyntAnalyser::Main()
{
	if (current_lexeme->string[0] == '@') {
		if (!strcmp("@",current_lexeme->string))
			throw EmptyLabelName(current_lexeme->line_number);

		AddNewPolizElem(new PolizOpNOP);
		Interpretator::label_table.AddNewLabel(current_lexeme->string,
			poliz_last, current_lexeme->line_number);
		GetNextLexeme();

		if (strcmp(":", current_lexeme->string))
			throw NotWhatExpected(":", current_lexeme->string,
				current_lexeme->line_number);
		GetNextLexeme();
	}

	Operator();
}


void SyntAnalyser::Operator()
{
	if (!strcmp("if", current_lexeme->string))
		If();
	else if (current_lexeme->string[0] == '$')
		Assignment();
	else if (!strcmp("goto", current_lexeme->string))
		Goto();
	else if (!strcmp("print", current_lexeme->string))
		Print();
	else
		B();
}


void SyntAnalyser::If()
{
	PolizLabel *tmp_label1;
	PolizLabel *tmp_label2;

	GetNextLexeme();
	Expression();
	tmp_label1 = new PolizLabel(0, 0, 0);
	AddNewPolizElem(tmp_label1);
	AddNewPolizElem(new PolizOpGoFalse);
	Operator();
	if (!strcmp("else", current_lexeme->string)) {
		tmp_label2 = new PolizLabel(0, 0, 0);

		AddNewPolizElem(tmp_label2);
		AddNewPolizElem(new PolizOpGo);
		AddNewPolizElem(new PolizOpNOP);

		tmp_label1->SetAddr(poliz_last);
		GetNextLexeme();
		Operator();

		AddNewPolizElem(new PolizOpNOP);
		tmp_label2->SetAddr(poliz_last);
	} else {
		AddNewPolizElem(new PolizOpNOP);
		tmp_label1->SetAddr(poliz_last);
	}
}


void SyntAnalyser::Assignment()
{
	Variable();

	if (strcmp("=", current_lexeme->string))
		throw NotWhatExpected("=", current_lexeme->string,
			current_lexeme->line_number);

	GetNextLexeme();

	Expression();
	AddNewPolizElem(new PolizOpAssign);
}


void SyntAnalyser::Goto()
{
	GetNextLexeme();

	if (current_lexeme->string[0] != '@')
		throw NotWhatExpected("label", current_lexeme->string,
			current_lexeme->line_number);

	if (!strcmp("@",current_lexeme->string))
		throw EmptyLabelName(current_lexeme->line_number);

	AddNewPolizElem(new PolizLabel(current_lexeme->string, 0,
		current_lexeme->line_number));
	AddNewPolizElem(new PolizOpGo);

	GetNextLexeme();
}


void SyntAnalyser::Print()
{
	int argc = 1;

	GetNextLexeme();
	if (strcmp("(", current_lexeme->string))
		throw NotWhatExpected("(", current_lexeme->string,
			current_lexeme->line_number);

	GetNextLexeme();
	PrintArgument();
	while(!strcmp(",", current_lexeme->string)) {
		argc++;
		GetNextLexeme();
		PrintArgument();
	}

	if (strcmp(")", current_lexeme->string))
		throw NotWhatExpected(")", current_lexeme->string,
			current_lexeme->line_number);

	AddNewPolizElem(new PolizPrint(argc));
	GetNextLexeme();
}


void SyntAnalyser::PrintArgument()
{
	if (current_lexeme->string[0] == '"') {
		AddNewPolizElem(new PolizStr(current_lexeme->string));
		GetNextLexeme();
	} else {
		Expression();
	}
}


// void SyntAnalyser::Function()
// {
// 	int argc = 0;
//  	Lexeme *tmp;
//  	PolizOpDefFunction *func;
//
// 	tmp = current_lexeme;
// 	if (!strcmp("?", tmp->string))
// 		throw EmptyFunctionName(tmp->line_number);
//
// 	GetNextLexeme();
// 	if (!strcmp("(", current_lexeme->string)) {
// 		argc++;
// 		GetNextLexeme();
// 		Expression();
// 		while(!strcmp(",", current_lexeme->string)) {
// 			argc++;
// 			GetNextLexeme();
// 			Expression();
// 		}
//
// 		if (strcmp(")", current_lexeme->string))
// 			throw NotWhatExpected(")", current_lexeme->string,
// 				current_lexeme->line_number);
//
// 		GetNextLexeme();
// 	}
//
// 	func = new PolizOpDefFunction(tmp->string, tmp->line_number,argc);
// 	CheckFuncArgc(func);
// 	AddNewPolizElem(func);
// }


void SyntAnalyser::Expression()
{
	Expression1();
	if (IsComparison(current_lexeme->string)) {
		char *comparison_type = current_lexeme->string;

		GetNextLexeme();
		Expression1();

		if (!strcmp("<", comparison_type))
			AddNewPolizElem(new PolizOpLess);
		else if (!strcmp("=", comparison_type))
			AddNewPolizElem(new PolizOpEqual);
		else if (!strcmp(">", comparison_type))
			AddNewPolizElem(new PolizOpGreater);
	}
}


void SyntAnalyser::Expression1()
{
	T();

	char *operation_type = current_lexeme->string;

	while(  !strcmp("+", operation_type) ||
			!strcmp("-", operation_type) ||
			!strcmp("|", operation_type) )
	{
		GetNextLexeme();
		T();

		if (!strcmp("+", operation_type))
			AddNewPolizElem(new PolizOpPlus);
		else if (!strcmp("-", operation_type))
			AddNewPolizElem(new PolizOpMinus);
		else if (!strcmp("|", operation_type))
			AddNewPolizElem(new PolizOpOr);

		operation_type = current_lexeme->string;
	}
}


void SyntAnalyser::T()
{
	F();

	char *operation_type = current_lexeme->string;

	while(  !strcmp("*", operation_type) ||
			!strcmp("/", operation_type) ||
			!strcmp("&", operation_type) )
	{
		GetNextLexeme();
		F();
		if (!strcmp("*", operation_type))
			AddNewPolizElem(new PolizOpMultiply);
		else if (!strcmp("/", operation_type))
			AddNewPolizElem(new PolizOpDivide);
		else if (!strcmp("&", operation_type))
			AddNewPolizElem(new PolizOpAnd);

		operation_type = current_lexeme->string;
	}
}


void SyntAnalyser::F()
{
	if (!strcmp("!", current_lexeme->string)) {
		GetNextLexeme();
		F1();
		AddNewPolizElem(new PolizOpNot);
	} else if (!strcmp("-", current_lexeme->string)) {
		GetNextLexeme();
		F1();
		AddNewPolizElem(new PolizOpUnMinus);
	} else {
		F1();
	}
}


void SyntAnalyser::F1()
{
	if (!strcmp("(", current_lexeme->string)) {
		GetNextLexeme();
		Expression();

		if (strcmp(")", current_lexeme->string))
			throw NotWhatExpected(")", current_lexeme->string,
				current_lexeme->line_number);

		GetNextLexeme();
	} else if (current_lexeme->string[0] == '$' ) {
		Variable();
		AddNewPolizElem(new PolizOpDereference());
	} else if (current_lexeme->type == num) {
		int val;

		sscanf(current_lexeme->string, "%d", &val);
		AddNewPolizElem(new PolizInt(val));
		GetNextLexeme();
	} else {
		throw NotWhatExpected("expression", current_lexeme->string,
			current_lexeme->line_number);
	}
}


void SyntAnalyser::Variable()
{
	if (current_lexeme->string[0] != '$')
		throw NotWhatExpected("variable", current_lexeme->string,
			current_lexeme->line_number);

	if (!strcmp("$",current_lexeme->string))
		throw EmptyVarName(current_lexeme->line_number);

	AddNewPolizElem(new PolizVar(current_lexeme->string,
		current_lexeme->line_number));
	GetNextLexeme();

	if (!strcmp("[", current_lexeme->string)) {
		GetNextLexeme();
		Expression();

		if (strcmp("]", current_lexeme->string))
			throw NotWhatExpected("]", current_lexeme->string,
				current_lexeme->line_number);

		AddNewPolizElem(new PolizOpIndex);
		GetNextLexeme();
	}
}


void SyntAnalyser::AddNewPolizElem(PolizElem *new_elem)
{
	PolizList *tmp;

	tmp = new PolizList;
	tmp->element = new_elem;
	tmp->next = 0;

	if (!poliz_head) {
		poliz_head = tmp;
		poliz_last = poliz_head;
	} else {
		poliz_last->next = tmp;
		poliz_last = poliz_last->next;
	}

}


void SyntAnalyser::DeletePolizList(PolizList *head)
{
	if (head) {
		DeletePolizList(head->next);
		delete head;
	}
}

SyntAnalyser::~SyntAnalyser()
{
	DeletePolizList(poliz_head);
}

