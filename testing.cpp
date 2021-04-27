#ifndef __TESTING_CPP__
#define __TESTING_CPP__

#include "gtest/gtest.h"
#include "select.hpp"
#include "spreadsheet.cpp"
#include <sstream>
#include <string>

TEST(NotTest, Notsomeone) {
	std::ostringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"First", "Last", "Age", "Major"});
	sheet.add_row({"Amanda","Andrews","22","business"});
        sheet.add_row({"Brian","Becker","21","computer science"});
	
	sheet.set_selection( new Select_Not(new Select_Contains(&sheet, "Last", "Becker")));
	sheet.print_selection(out);

	EXPECT_EQ(out.str(), "Amanda Andrews 22 business \n");
} 

TEST(NotTest, ContainsDoesntExist) {
        std::ostringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Amanda","Andrews","22","business"});
        sheet.add_row({"Brian","Becker","21","computer science"});

        sheet.set_selection( new Select_Not(new Select_Contains(&sheet, "Last", "Prince")));
        sheet.print_selection(out);

        EXPECT_EQ(out.str(), "Amanda Andrews 22 business \nBrian Becker 21 computer science \n");
}


TEST(NotTest, multiplenots) {
	std::ostringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Amanda","Andrews","22","business"});
        sheet.add_row({"Brian","Becker","21","computer science"});
	sheet.add_row({"Joseph", "Prince", "19", "computer engineering"});
	sheet.add_row({"Puloma", "Katiyar", "??", "computer science"});


        sheet.set_selection( new Select_Not(new Select_Or(new Select_Contains(&sheet, "Last", "Becker"), new Select_Contains(&sheet, "First", "m"))));
        sheet.print_selection(out);

        EXPECT_EQ(out.str(), "Joseph Prince 19 computer engineering \n");
}

TEST(OrTest, simpleOr) {
        std::ostringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Amanda","Andrews","22","business"});
        sheet.add_row({"Brian","Becker","21","computer science"});
        sheet.add_row({"Joseph", "Prince", "19", "computer engineering"});
        sheet.add_row({"Puloma", "Katiyar", "??", "computer science"});


        sheet.set_selection( new Select_Or(new Select_Contains(&sheet, "Age", "??"), new Select_Contains(&sheet, "Major", "business")));
        sheet.print_selection(out);

        EXPECT_EQ(out.str(), "Amanda Andrews 22 business \nPuloma Katiyar ?? computer science \n");
}

TEST(OrTest, multipleOr) {
        std::ostringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Amanda","Andrews","22","business"});
        sheet.add_row({"Brian","Becker","21","computer science"});
        sheet.add_row({"Joseph", "Prince", "19", "computer engineering"});
        sheet.add_row({"Puloma", "Katiyar", "??", "computer science"});


        sheet.set_selection( new Select_Or(new Select_Contains(&sheet, "First", "ma"), new Select_Or(new Select_Contains(&sheet, "Last", "P"), new Select_Contains(&sheet, "Age", "22"))));
        sheet.print_selection(out);

        EXPECT_EQ(out.str(), "Amanda Andrews 22 business \nJoseph Prince 19 computer engineering \nPuloma Katiyar ?? computer science \n");
}

TEST(AndTest, simpleAnd) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"First", "Last", "Age", "Major"});
	sheet.add_row({"Diane","Dole","20","computer engineering"});
    	sheet.add_row({"David","Dole","22","electrical engineering"});
   	sheet.add_row({"Dominick","Dole","22","communications"});
	
	sheet.set_selection(new Select_And(new Select_Contains(&sheet, "Last", "Dole"), new Select_Contains(&sheet, "Age", "22")));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "David Dole 22 electrical engineering \nDominick Dole 22 communications \n");
}	

TEST(AndTest, multipleAnd) {
	std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
	sheet.add_row({"Brian","Becker","21","computer science"});
    	sheet.add_row({"Carol","Conners","21","computer science"});
    	sheet.add_row({"Joe","Jackson","21","mathematics"});
    	sheet.add_row({"Sarah","Summers","21","computer science"});
	
	sheet.set_selection(new Select_And(new Select_Contains(&sheet, "Age", "21"), new Select_And(new Select_Contains(&sheet, "First", "o"), new Select_Contains(&sheet, "Major", "computer science"))));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Carol Conners 21 computer science \n");
}

TEST(ContainsTest, simpleContains) {
	std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Brian","Becker","21","computer science"});
        sheet.add_row({"Carol","Conners","21","computer science"});

	sheet.set_selection(new Select_Contains(&sheet, "Last", "er"));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Brian Becker 21 computer science \nCarol Conners 21 computer science \n");
}

TEST(ContainsTest, edgeCaseContains) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Brian","Becker","21","computer science"});
        sheet.add_row({"Carol","Conners","21","computer science"});

        sheet.set_selection(new Select_Contains(&sheet, "Last", "Er"));
        sheet.print_selection(out);
	EXPECT_EQ(out.str(), "");
}

TEST(ContainsTest, invalidColumnName) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"Brian","Becker","21","computer science"});
        sheet.add_row({"Carol","Conners","21","computer science"});

        sheet.set_selection(new Select_Contains(&sheet, "Las", "Er"));
        sheet.print_selection(out);
        EXPECT_EQ(out.str(), "");
}

TEST(ContainsTest, duplicateCols) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major", "First"});
        sheet.add_row({"Brian","Becker","21","computer science"});
        sheet.add_row({"Carol","Conners","21","computer science"});

        sheet.set_selection(new Select_Contains(&sheet, "Last", "Er"));
        sheet.print_selection(out);
        EXPECT_EQ(out.str(), "Duplicate column names exist");
}
#endif
