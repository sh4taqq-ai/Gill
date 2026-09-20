#include "editor/selector/selectionManager.hpp"

std::optional<Entity> SelectionManager::GetActiveSelected() const {
    return  op_activeEntity;
}

const std::unordered_set<Entity>& SelectionManager::GetAllSelected() const {
    return us_entities;
}


void SelectionManager::SetSelected(Entity value) {
    if (op_activeEntity != value) {
        op_activeEntity = value;
        us_entities.clear();
        us_entities.insert(value);
    }
}

bool SelectionManager::IsSelected(Entity value) {
    return  us_entities.contains(value);
}

void SelectionManager::ToggleSelection(Entity value) {
    if (us_entities.contains(value) && op_activeEntity != value) {
        op_activeEntity = value;
    }
    else  if (us_entities.contains(value) && op_activeEntity == value) {
        us_entities.erase(value);
    }
    else {
        us_entities.insert(value);
        op_activeEntity = value;
    }
}

void SelectionManager::ClearSelection() {
    us_entities.clear();
    op_activeEntity = std::nullopt;
}
