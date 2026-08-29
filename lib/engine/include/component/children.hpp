#pragma once
#include <algorithm>
#include <vector>
#include <cstdint>

using Entity = uint32_t;
struct ChildComponent {
    std::vector<Entity> children;

    void AddChild(Entity child) {
        children.push_back(child);
    }
    void RemoveChild(Entity child) {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        if (children.empty()) {
            return;
        }
    }
    void RemoveAllChildren() {
        children.clear();
    }

    bool IsEmpty() {
        return children.empty();
    }

};