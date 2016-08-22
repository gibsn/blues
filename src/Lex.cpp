#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Stack.h"
#include "Exception.h"

#include "Lex.h"

Lexeme *LexAnalyser::MakeNewLexeme(int type)
{
	Lexeme *tmp;

	tmp = new Lexeme;
	tmp->type = type;
	tmp->line_number = lines_count;

	buf[buf_index] = '\0';
	tmp->string = strndup(buf, buf_index);
	buf_index = 0;

	tmp->next = 0;

	return tmp;
}


void LexAnalyser::FreeLexemesList(Lexeme *head)
{
	if (head) {
		FreeLexemesList(head->next);

		if (head->string)
			free(head->string);

		delete head;
	}
}


void LexAnalyser::CopyStackToList()
{
	Lexeme *new_lexeme;

	stack_memory.Pop(new_lexeme);
	while(new_lexeme) {
		new_lexeme->next = lexemes;
		lexemes = new_lexeme;
		stack_memory.Pop(new_lexeme);
	}
}


void LexAnalyser::AnalyseZero(int c)
{
	if (IsDigit(c)) {
		buf[buf_index++] = c;
		SetState(num);
	} else if (IsLetter(c)) {
		buf[buf_index++] = c;
		SetState(keyword);
	} else if (IsID(c)) {
		buf[buf_index++] = c;
		SetState(identificator);
	} else if (c == '"') {
		buf[buf_index++] = c;
		SetState(string);
	} else if (c == '#') {
		SetState(comment);
	} else if (c == '\n') {
		lines_count++;
	} else if ( (c == ' ') || (c == '\t') ) {
		//pass
	} else if (IsSeparator(c)) {
		buf[buf_index++] = c;
		stack_memory.Push(MakeNewLexeme(keyword));
	} else if (c == EOF) {
		throw 666;
	} else {
		throw IllegalSymbol(c, lines_count);
	}
}


void LexAnalyser::AnalyseNum(int c)
{
	if (IsDigit(c)) {
		buf[buf_index++] = c;
	} else if (c == '\n') {
		stack_memory.Push(MakeNewLexeme(num));
		lines_count++;
		SetState(0);
	} else if ((c == ' ') || (c == '\t')) {
		stack_memory.Push(MakeNewLexeme(num));
		SetState(0);
	} else if (c == '#') {
		stack_memory.Push(MakeNewLexeme(num));
		SetState(comment);
	} else if (IsSeparator(c)) {
		stack_memory.Push(MakeNewLexeme(num));
		buf[buf_index++] = c;
		stack_memory.Push(MakeNewLexeme(keyword));
		SetState(0);
	} else {
		throw IllegalSymbol(c, lines_count);
	}
}


void LexAnalyser::AnalyseString(int c)
{
	if (c != '"') {
		buf[buf_index++] = c;
	} else {
		buf[buf_index++] = c;
		stack_memory.Push(MakeNewLexeme(string));
		SetState(0);
	}
}


void LexAnalyser::AnalyseIdentificator(int c)
{
	if (IsLetter(c) || IsDigit(c) || (c == '_')) {
		buf[buf_index++] = c;
	} else if (c == '\n') {
		stack_memory.Push(MakeNewLexeme(identificator));
		lines_count++;
		SetState(0);
	} else if ((c == ' ') || (c == '\t')) {
		stack_memory.Push(MakeNewLexeme(identificator));
		SetState(0);
	} else if (c == '#') {
		stack_memory.Push(MakeNewLexeme(identificator));
		SetState(comment);
	} else if (IsSeparator(c)) {
		stack_memory.Push(MakeNewLexeme(identificator));
		buf[buf_index++] = c;
		stack_memory.Push(MakeNewLexeme(keyword));
		SetState(0);
	} else {
		throw IllegalSymbol(c, lines_count);
	}
}


void LexAnalyser::AnalyseKeyword(int c)
{
	if (IsLetter(c)) {
		buf[buf_index++]=c;
	} else if (c == '\n') {
		stack_memory.Push(MakeNewLexeme(keyword));
		lines_count++;
		SetState(0);
	} else if ((c == ' ') || (c == '\t')) {
		stack_memory.Push(MakeNewLexeme(keyword));
		SetState(0);
	} else if (c == '#') {
		stack_memory.Push(MakeNewLexeme(keyword));
		SetState(comment);
	} else if (IsSeparator(c)) {
		stack_memory.Push(MakeNewLexeme(keyword));
		buf[buf_index++] = c;
		stack_memory.Push(MakeNewLexeme(keyword));
		SetState(0);
	} else {
		throw IllegalSymbol(c, lines_count);
	}
}


void LexAnalyser::AnalyseComment(int c)
{
	if (c == '#')
		SetState(0);
}


void LexAnalyser::SwitchState(int c)
{
	if (state == 0)
		AnalyseZero(c);
	else if (state == num)
		AnalyseNum(c);
	else if (state == string)
		AnalyseString(c);
	else if (state == identificator)
		AnalyseIdentificator(c);
	else if (state == keyword)
		AnalyseKeyword(c);
	else if (state == comment)
		AnalyseComment(c);
}


void LexAnalyser::PrintLexemes()
{
	Lexeme *iterator = lexemes;

	while (iterator) {
		fprintf(stderr, "%d: %s\n",iterator->line_number, iterator->string);
		iterator = iterator->next;
	}
}


void LexAnalyser::Run()
{
	int c;

	while(true) {
		try {
			SwitchState(c = getchar());
		}

		catch(const SyntaxEx& info) {
			info.Print();
			exit(0);
		}
		catch(...) {
			CopyStackToList();
			break;
		}
	}
}


LexAnalyser::LexAnalyser()
	: lexemes(0),
	state(0),
	buf_index(0),
	lines_count(0)
{
}


LexAnalyser::~LexAnalyser()
{
	FreeLexemesList(lexemes);
}

