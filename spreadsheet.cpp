#include "spreadsheet.hpp"
#include "select.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>

Spreadsheet::~Spreadsheet()
{
    delete select;
}

void Spreadsheet::set_selection(Select* new_select)
{
    	delete select;
    	select = new_select;
}

void Spreadsheet::clear()
{
    column_names.clear();
    data.clear();
    delete select;
    select = nullptr;
}

void Spreadsheet::set_column_names(const std::vector<std::string>& names)
{
    // check if duplicate column names exist
    std::unordered_set<std::string> colSet;
    for(int i = 0; i < names.size(); i++) {
	if(colSet.find(names.at(i)) != colSet.end()) {
		std::cout << "Duplicate column names exist" << std::endl;
		exit(1);
	} else {
		colSet.insert(names.at(i));
	}
    }
    column_names=names;
}

void Spreadsheet::add_row(const std::vector<std::string>& row_data)
{
    data.push_back(row_data);
}

int Spreadsheet::get_column_by_name(const std::string& name) const
{
    for(int i=0; i<column_names.size(); i++)
        if(column_names.at(i) == name)
            return i;
    return -1;
}

void Spreadsheet::print_selection(std::ostream& out) const {
    int rows = data.size();
    int cols = data.at(0).size();
    if(select == nullptr) {
	for(int i = 0; i < rows; i++) {
	    for(int j = 0; j < cols; j++) {
	 	out << data.at(i).at(j) << " ";
	    }
	    out << std::endl;
	}
    } else {
	for(int i = 0; i < rows; i++) {
	    if(select->select(this, i)) {
		for(int j = 0; j < cols; j++) {
		    out << data.at(i).at(j) << " ";
		}
		out << std::endl;
	    }
	}
    }
}
