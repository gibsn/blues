#include <stdlib.h>

#include "Exception.h"
#include "LabelTable.h"
#include "Lex.h"
#include "Poliz.h"
#include "Synt.h"
#include "VarTable.h"

#include "Interpretator.h"


VarTable Interpretator::var_table;
LabelTable Interpretator::label_table;


void Interpretator::Run()
{
	try {
		LexAnalyser lex;
		SyntAnalyser synt;

		lex.Run();
		synt.Run(lex.GetLexemesHead());
		poliz_head = synt.GetPolizHead();
		PolizList *cur_cmd = poliz_head;

		// now evaluate POLIZ
		while (cur_cmd)
			cur_cmd->element->Evaluate(poliz_stack, cur_cmd);
	}
	catch(const PolizEx& info) {
		info.Print();
		exit(1);
	}
}
