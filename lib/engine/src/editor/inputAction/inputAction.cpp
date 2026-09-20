#include "editor/inputAction/inputAction.hpp"
#include "core/input/input.hpp"

bool EditorInputMap::IsActionPressed(EditorAction action) {
    {
        return p_input->IsKeyPressed(um_keybinds.at(action));
    }
}
