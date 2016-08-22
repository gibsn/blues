#ifndef LABELTABLE_H_SENTRY
#define LABELTABLE_H_SENTRY


struct PolizList;


class LabelTableRow
{
	char *name;
	PolizList *addr;
	LabelTableRow *next;

public:
	LabelTableRow(char *str, PolizList *p);
	~LabelTableRow();

	char *GetName() const { return name; }
	PolizList *GetAddr() const { return addr; }
	LabelTableRow *GetNext() const { return next; }
	void SetAddr(PolizList *p) { addr = p; }
	void SetNext(LabelTableRow *new_row) { next = new_row; }
};


class LabelTable
{
	LabelTableRow *table_head;

	static void DeleteLabelTable(LabelTableRow *head);

public:
	LabelTable();
	~LabelTable();

	void AddNewLabel(char *str, PolizList *addr1, int n);
	PolizList *GetLabelAddr(const char *name) const;
};


#endif
