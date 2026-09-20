#pragma once
#include <unordered_set>
#include <cstdint>
#include <optional>

using Entity = uint32_t;
class SelectionManager {
public:
    std::optional<Entity> GetActiveSelected() const;
    void SetSelected( Entity value);
    void ToggleSelection(Entity value);
    const std::unordered_set<Entity>& GetAllSelected() const;
    bool IsSelected(Entity value);
    void ClearSelection();

private:

    std::optional<Entity> op_activeEntity;
    std::unordered_set<Entity>us_entities;

};