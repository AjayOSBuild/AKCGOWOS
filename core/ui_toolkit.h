#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "compositor.h"

enum class UIEventType { MOUSE_CLICK, MOUSE_MOVE, KEY_PRESS, KEY_RELEASE };

struct UIEvent {
    UIEventType type;
    int x, y; 
    int key_code;
    bool pressed;
    bool handled = false;
};

class Widget {
public:
    int x = 0, y = 0;
    int width = 0, height = 0;
    bool visible = true;
    bool is_hovered = false;
    bool is_focused = false;

    virtual ~Widget() = default;

    virtual void SetBounds(int nx, int ny, int nwidth, int nheight) {
        x = nx; y = ny; width = nwidth; height = nheight;
    }

    virtual bool HitTest(int test_x, int test_y) const {
        return visible && (test_x >= x && test_x <= x + width && test_y >= y && test_y <= y + height);
    }

    virtual void OnEvent(UIEvent& event) { }

    virtual void Draw(Graphics2D& gfx, FontRenderer& font_engine) = 0;
};

class Container : public Widget {
protected:
    std::vector<std::shared_ptr<Widget>> children;

public:
    void AddChild(std::shared_ptr<Widget> child) {
        children.push_back(child);
    }

    virtual void PerformLayout() {}

    void OnEvent(UIEvent& event) override {
        if (!visible) return;

        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            auto& child = *it;
            if (event.type == UIEventType::MOUSE_CLICK || event.type == UIEventType::MOUSE_MOVE) {
                if (child->HitTest(event.x, event.y)) {
                    // Manage UI Focus
                    if (event.type == UIEventType::MOUSE_CLICK && event.pressed) {
                        for (auto& c : children) c->is_focused = false;
                        child->is_focused = true;
                    }
                    child->OnEvent(event);
                    if (event.handled) return;
                }
            } else {
                // Route keyboard events to the focused widget
                if (child->is_focused) {
                    child->OnEvent(event);
                    if (event.handled) return;
                }
            }
        }
    }

    void Draw(Graphics2D& gfx, FontRenderer& font_engine) override {
        if (!visible) return;
        for (auto& child : children) {
            child->Draw(gfx, font_engine);
        }
    }
};

// --- Button Widget ---
class Button : public Widget {
private:
    std::string label;
    std::function<void()> on_click;

public:
    Button(const std::string& text, std::function<void()> callback) : label(text), on_click(callback) {}

    void OnEvent(UIEvent& event) override {
        if (!visible) return;
        if (event.type == UIEventType::MOUSE_CLICK && event.pressed) {
            if (HitTest(event.x, event.y)) {
                if (on_click) on_click();
                event.handled = true;
            }
        }
    }

    void Draw(Graphics2D& gfx, FontRenderer& font_engine) override {
        if (!visible) return;
        uint32_t bg_color = is_focused ? 0xFF666666 : (is_hovered ? 0xFF777777 : 0xFF555555);
        gfx.FillRect(x, y, width, height, bg_color);
        gfx.DrawRect(x, y, width, height, 0xFF000000, 1);
        font_engine.DrawText(label, x + (width / 4), y + (height / 2) + 5, 12, 0xFFFFFFFF);
    }
};

// --- Label Widget ---
class Label : public Widget {
private:
    std::string text;
    uint32_t text_color;

public:
    Label(const std::string& t, uint32_t color = 0xFF000000) : text(t), text_color(color) {}

    void Draw(Graphics2D& gfx, FontRenderer& font_engine) override {
        if (!visible) return;
        font_engine.DrawText(text, x, y + height, 12, text_color);
    }
};

// --- Text Input Widget ---
class TextInput : public Widget {
private:
    std::string buffer;
    
public:
    TextInput() {}

    std::string GetText() const { return buffer; }

    void OnEvent(UIEvent& event) override {
        if (!visible || !is_focused) return;
        
        if (event.type == UIEventType::KEY_PRESS && event.pressed) {
            // Simplified ascii routing mapping (requires full keyboard driver in complete OS)
            if (event.key_code == 8) { // Backspace
                if (!buffer.empty()) buffer.pop_back();
                event.handled = true;
            } else if (event.key_code >= 32 && event.key_code <= 126) { // Printable ASCII
                buffer += static_cast<char>(event.key_code);
                event.handled = true;
            }
        }
    }

    void Draw(Graphics2D& gfx, FontRenderer& font_engine) override {
        if (!visible) return;
        
        // Draw input box background and border
        gfx.FillRect(x, y, width, height, 0xFFFFFFFF);
        uint32_t border_color = is_focused ? 0xFF0000FF : 0xFF000000;
        gfx.DrawRect(x, y, width, height, border_color, is_focused ? 2 : 1);
        
        // Draw text
        font_engine.DrawText(buffer, x + 5, y + (height / 2) + 5, 12, 0xFF000000);
        
        // Draw blinking cursor if focused
        if (is_focused) {
            int cursor_x = x + 5 + (buffer.length() * 8); // Approximation without exact font metrics
            gfx.FillRect(cursor_x, y + 5, 2, height - 10, 0xFF000000);
        }
    }
};

// --- Linear Layout Manager ---
enum class LayoutDirection { VERTICAL, HORIZONTAL };

class LinearLayout : public Container {
private:
    LayoutDirection direction;
    int spacing;

public:
    LinearLayout(LayoutDirection dir = LayoutDirection::VERTICAL, int spc = 5) 
        : direction(dir), spacing(spc) {}

    void PerformLayout() override {
        int current_x = x;
        int current_y = y;

        for (auto& child : children) {
            child->x = current_x;
            child->y = current_y;

            if (direction == LayoutDirection::VERTICAL) {
                current_y += child->height + spacing;
            } else {
                current_x += child->width + spacing;
            }
        }
    }
};
