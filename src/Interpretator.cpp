#include <stdlib.h>

#include "VarTable.h"
#include "LabelTable.h"
#include "Exception.h"
#include "Poliz.h"

#include "Interpretator.h"


VarTable Interpretator::var_table;
LabelTable Interpretator::label_table;


void Interpretator::Run()
{
	cur_cmd = poliz_head;
	
	try
	{
		while(cur_cmd)
			cur_cmd->element->Evaluate(poliz_stack, cur_cmd);
	}
	catch(const PolizEx& info)
	{
		info.Print();
		exit(1);
	}
}
