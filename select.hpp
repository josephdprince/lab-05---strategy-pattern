#ifndef __SELECT_HPP__
#define __SELECT_HPP__
#include <iostream>
#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
	column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
	if (column == -1)
	    return false; 
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_Contains : public Select_Column {
    private:
	Spreadsheet* sheet;
	std::string columnName;
	std::string rowContent;
    public:
	Select_Contains(Spreadsheet* sheet, std::string columnName, std::string rowContent) : Select_Column(sheet, columnName){
		this->rowContent = rowContent;
	}

	virtual ~Select_Contains() {}

	virtual bool select(const std::string& s) const {
		if (s.find(rowContent) != std::string::npos) 
			return true;
		else
			return false;
	}
};


class Select_Not : public Select {
    private:
	Select* content;
    public:
	Select_Not(Select* content) {
	    this->content = content;
	}
	
	virtual ~Select_Not() {
		delete this->content;
	}
	
	virtual bool select(const Spreadsheet* sheet, int row) const {
	    return !content->select(sheet, row);
	}
};

class Select_And : public Select {
    private:
	Select* obj1;
	Select* obj2;
    public:
	Select_And(Select* obj1, Select* obj2) {
		this->obj1 = obj1;
		this->obj2 = obj2;	
	}

	virtual ~Select_And() {
		delete this->obj1;
		delete this->obj2;
	}

	virtual bool select(const Spreadsheet* sheet, int row) const{
		return obj1->select(sheet, row) && obj2->select(sheet, row);
	}
};

class Select_Or : public Select {
    private:
	Select* obj1;
	Select* obj2;
    public:
	Select_Or(Select* obj1, Select* obj2) {
		this->obj1 = obj1;
		this->obj2 = obj2;

	}

	virtual ~Select_Or() {
		delete this->obj1;
		delete this->obj2;
	}

	virtual bool select(const Spreadsheet* sheet, int row) const {
		return obj1->select(sheet, row) || obj2->select(sheet, row);
	}
};
#endif //__SELECT_HPP__
