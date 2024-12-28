#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iomanip>
#include <sstream>
#include <string>

class Calculator {
public:
    double evaluate( const std::string& expression );
    double calculate( double num1, double num2, char op );
    double calculateProfit( double price, int amount, double sellingPrice );
    bool isNumber( const std::string& str );

    static std::string formatRevenue( double revenue ) {
        std::ostringstream formattedRevenue;

        if (revenue >= 1e9) {
            formattedRevenue << std::fixed << std::setprecision( 2 ) << (revenue / 1e9) << "b";
        }
        else if (revenue >= 1e6) {
            formattedRevenue << std::fixed << std::setprecision( 2 ) << (revenue / 1e6) << "m";
        }
        else if (revenue >= 1e3) {
            formattedRevenue << std::fixed << std::setprecision( 2 ) << (revenue / 1e3) << "k";
        }
        else {
            formattedRevenue << std::fixed << std::setprecision( 2 ) << revenue;
        }

        return formattedRevenue.str( );
    }

private:

    double parseNumber( const std::string& str );
};

#endif
