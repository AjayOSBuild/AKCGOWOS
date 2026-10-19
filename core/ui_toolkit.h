#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "compositor.h" // For Graphics2D and FontRenderer access

enum class UIEventType { MOUSE_CLICK, MOUSE_MOVE, KEY_PRESS, KEY_RELEASE };

struct UIEvent {
    UIEventType type;
    int x, y; // Mouse coordinates relative to the screen/widget
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

    virtual ~Widget() = default;

    virtual void SetBounds(int nx, int ny, int nwidth, int nheight) {
        x = nx; y = ny; width = nwidth; height = nheight;
    }

    virtual bool HitTest(int test_x, int test_y) const {
        return visible && (test_x >= x && test_x <= x + width && test_y >= y && test_y <= y + height);
    }

    virtual void OnEvent(UIEvent& event) {
        // Base implementation does nothing
    }

    virtual void Draw(Graphics2D& gfx, FontRenderer& font_engine) = 0; // Pure virtual
};

class Container : public Widget {
protected:
    std::vector<std::shared_ptr<Widget>> children;

public:
    void AddChild(std::shared_ptr<Widget> child) {
        children.push_back(child);
    }

    virtual void PerformLayout() {
        // Base container does no automatic layout
    }

    void OnEvent(UIEvent& event) override {
        if (!visible) return;

        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            auto& child = *it;
            if (event.type == UIEventType::MOUSE_CLICK || event.type == UIEventType::MOUSE_MOVE) {
                if (child->HitTest(event.x, event.y)) {
                    child->OnEvent(event);
                    if (event.handled) return;
                }
            } else {
                child->OnEvent(event);
                if (event.handled) return;
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

// --- Interactive Button Widget ---
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
                std::cout << "[Button] Clicked: " << label << "\n";
                if (on_click) on_click();
                event.handled = true;
            }
        }
    }

    void Draw(Graphics2D& gfx, FontRenderer& font_engine) override {
        if (!visible) return;
        uint32_t bg_color = is_hovered ? 0xFF777777 : 0xFF555555;
        gfx.FillRect(x, y, width, height, bg_color);
        gfx.DrawRect(x, y, width, height, 0xFF000000, 1);
        font_engine.DrawText(label, x + (width / 4), y + (height / 2) + 5, 12, 0xFFFFFFFF);
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
