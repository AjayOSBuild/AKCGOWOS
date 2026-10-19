#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "compositor.h" // For Graphics2D and FontRenderer access

enum class UIEventType { MOUSE_CLICK, MOUSE_MOVE, KEY_PRESS, KEY_RELEASE };

struct UIEvent {
    UIEventType type;
    int x, y; // Mouse coordinates relative to the widget
    int key_code;
    bool handled = false;
};

class Widget {
protected:
    int x = 0, y = 0;
    int width = 0, height = 0;
    bool visible = true;
    bool is_hovered = false;

public:
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

    bool IsVisible() const { return visible; }
    void SetVisible(bool v) { visible = v; }
};

class Container : public Widget {
protected:
    std::vector<std::shared_ptr<Widget>> children;

public:
    void AddChild(std::shared_ptr<Widget> child) {
        children.push_back(child);
    }

    void OnEvent(UIEvent& event) override {
        if (!visible) return;

        // Iterate children in reverse (top-most elements first)
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            auto& child = *it;
            if (event.type == UIEventType::MOUSE_CLICK || event.type == UIEventType::MOUSE_MOVE) {
                if (child->HitTest(event.x, event.y)) {
                    // Translate coordinates to child's local space
                    UIEvent local_event = event;
                    local_event.x -= x;
                    local_event.y -= y;
                    
                    child->OnEvent(local_event);
                    if (local_event.handled) {
                        event.handled = true;
                        return;
                    }
                }
            } else {
                // Keyboard events broadcast or target focused child
                child->OnEvent(event);
                if (event.handled) return;
            }
        }
    }

    void Draw(Graphics2D& gfx, FontRenderer& font_engine) override {
        if (!visible) return;
        
        // Draw all children back-to-front
        for (auto& child : children) {
            child->Draw(gfx, font_engine);
        }
    }
};
