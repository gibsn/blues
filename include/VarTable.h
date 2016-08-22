#ifndef VARTABLE_H_SENTRY
#define VARTABLE_H_SENTRY


class VarTableRow
{
	char *name;
	int value;
	// int *var_addr;
	VarTableRow *next;

public:
	VarTableRow(const char *str, int i);
	~VarTableRow();

	char *GetName() const { return name; }
	int GetValue() const { return value; }
	// const int *GetVarAddr() const { return &value; }
	VarTableRow *GetNext() const { return next; }
	void SetVariable(int n) { value = n; }
	void SetNext(VarTableRow *new_row) { next = new_row; }
};


class VarTable
{
	VarTableRow *table_head;

	void AddNewVar(const char *new_name, int new_value);

	static void DeleteVarTable(VarTableRow *head);

public:
	VarTable();
	~VarTable();

	VarTableRow *GetVarByName(const char *name);
	void Assign(const char *name, int value);
};




#endif
