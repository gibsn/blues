#include <string.h>
#include <stdlib.h>

#include "VarTable.h"


VarTableRow::VarTableRow(const char *str, int i)
{
	if (str)
		name = strndup(str, strlen(str));
	else
		name = 0;
	var_addr = new int;
	*var_addr = i;
	next = 0;
}


char *VarTableRow::GetName() const
{
	return name;
}


int *VarTableRow::GetVarAddr() const
{
	return var_addr;
}


VarTableRow *VarTableRow::GetNext() const
{
	return next;
}


void VarTableRow::SetNext(VarTableRow *new_row)
{
	next = new_row;
}


VarTableRow::~VarTableRow()
{
	if (name)
		free(name);
	if (var_addr)
		delete var_addr;
}


VarTable::VarTable()
{
	table_head = 0;
}


void VarTable::AddNewVar(const char *new_name, int new_value)
{
	VarTableRow *new_var = new VarTableRow(new_name, new_value);

	new_var->SetNext(table_head);
	table_head = new_var;
}


int *VarTable::GetVarAddr(const char *name) const
{
	VarTableRow *tmp = table_head;
	
	while (tmp)
		if (!strcmp(tmp->GetName(), name))
			return tmp->GetVarAddr();
		else
			tmp = tmp->GetNext();
	return 0;
}


void VarTable::DeleteVarTable(VarTableRow *head)
{
	if (head)
	{
		DeleteVarTable(head->GetNext());
		delete head;
	}
}


VarTable::~VarTable()
{
	DeleteVarTable(table_head);
}
















