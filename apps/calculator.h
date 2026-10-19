#pragma once
#include "../core/ui_toolkit.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

class CalculatorApp {
private:
    std::string current_input = "0";
    double previous_value = 0.0;
    char pending_operation = '\0';
    bool new_input_expected = true;

    void FormatOutput(double val) {
        std::ostringstream out;
        out << std::setprecision(8) << val;
        current_input = out.str();
    }

public:
    CalculatorApp() {
        std::cout << "[Calculator] Initializing native Calculator application...\n";
    }

    void HandleDigit(char digit) {
        if (new_input_expected) {
            current_input = "";
            new_input_expected = false;
        }
        if (current_input == "0" && digit != '.') {
            current_input = digit;
        } else {
            current_input += digit;
        }
        std::cout << "[Calculator] Input -> " << current_input << "\n";
    }

    void HandleOperation(char op) {
        if (!new_input_expected && pending_operation != '\0') {
            Calculate();
        }
        previous_value = std::stod(current_input);
        pending_operation = op;
        new_input_expected = true;
        std::cout << "[Calculator] Operation set -> " << op << "\n";
    }

    void Calculate() {
        if (pending_operation == '\0') return;

        double current_value = std::stod(current_input);
        double result = 0.0;

        switch (pending_operation) {
            case '+': result = previous_value + current_value; break;
            case '-': result = previous_value - current_value; break;
            case '*': result = previous_value * current_value; break;
            case '/': 
                if (current_value != 0) result = previous_value / current_value; 
                else {
                    current_input = "Error";
                    new_input_expected = true;
                    pending_operation = '\0';
                    std::cerr << "[Calculator] Error: Division by zero.\n";
                    return;
                }
                break;
        }

        FormatOutput(result);
        std::cout << "[Calculator] Result -> " << current_input << "\n";
        pending_operation = '\0';
        new_input_expected = true;
    }

    void Clear() {
        current_input = "0";
        previous_value = 0.0;
        pending_operation = '\0';
        new_input_expected = true;
        std::cout << "[Calculator] Cleared state.\n";
    }

    void RenderUI(Graphics2D& gfx, FontRenderer& font_engine) {
        // Draw Window Background
        gfx.FillRect(0, 0, 300, 400, 0xFFF0F0F0);
        
        // Draw Display Background
        gfx.FillRect(15, 15, 270, 60, 0xFFFFFFFF);
        gfx.DrawRect(15, 15, 270, 60, 0xFFCCCCCC, 1);
        
        // Draw Current Input (Right-aligned simulation)
        font_engine.DrawText(current_input, 20, 50, 24, 0xFF000000);

        // Draw Keypad (Simulated layout grid)
        int start_x = 15;
        int start_y = 90;
        int btn_w = 60;
        int btn_h = 50;
        int padding = 10;

        const std::vector<std::string> buttons = {
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "C", "0", "=", "+"
        };

        int idx = 0;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                int bx = start_x + col * (btn_w + padding);
                int by = start_y + row * (btn_h + padding);
                
                gfx.FillRect(bx, by, btn_w, btn_h, 0xFFE0E0E0);
                gfx.DrawRect(bx, by, btn_w, btn_h, 0xFFB0B0B0, 1);
                font_engine.DrawText(buttons[idx], bx + 25, by + 30, 16, 0xFF000000);
                idx++;
            }
        }
    }
};
