#include <Calculator/Calc.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

double Calculator::calculate( double num1, double num2, char op ) {
    switch (op) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/':
        if (num2 == 0) {
            throw std::invalid_argument( "Division by zero" );
        }
        return num1 / num2;
    default: throw std::invalid_argument( "Invalid operator" );
    }
}

double Calculator::evaluate( const std::string& expression ) {
    std::istringstream stream( expression );
    double num1, num2;
    char op;

    stream >> num1 >> op >> num2;

    if (stream.fail( )) {
        //throw std::invalid_argument( "Invalid expression format" );
        return num1;
    }

    return calculate( num1, num2, op );
}

double Calculator::calculateProfit( double price, int amount, double sellingPrice ) {

    double cost = price * amount;
    double revenue = sellingPrice * amount;

    std::cout << " Evaluated Profit [>] " << formatRevenue( revenue - cost ) << std::endl;

    return revenue;
}

double Calculator::parseNumber( const std::string& str ) {
    if (isNumber( str )) {
        return std::stod( str );
    }
    throw std::invalid_argument( "Invalid number format" );
}

bool Calculator::isNumber( const std::string& str ) {
    std::istringstream ss( str );
    double d;
    char c;
    if (!(ss >> d)) {
        return false;
    }
    return !(ss >> c);
}
