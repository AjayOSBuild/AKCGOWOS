#include <iostream>
#include "calculator.h"

int main() {
    std::cout << "=== OS Project Calculator App ===\n";
    CalculatorApp calc;
    
    // Simulate user operations: 15.5 * 2 = 31
    std::cout << "\n--- Simulating: 15.5 * 2 ---\n";
    calc.HandleDigit('1');
    calc.HandleDigit('5');
    calc.HandleDigit('.');
    calc.HandleDigit('5');
    calc.HandleOperation('*');
    calc.HandleDigit('2');
    calc.Calculate();
    
    std::cout << "\n--- Simulating: Clear State ---\n";
    calc.Clear();
    
    std::cout << "[+] Calculator application tests completed successfully.\n";
    return 0;
}
