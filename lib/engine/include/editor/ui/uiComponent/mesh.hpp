#pragma once
#include "editor/registry/registry.hpp"

class scene;

bool HasMesh(Scene* s, Entity e);
void AddMesh(Scene* s, Entity e, EditorContext& ctx);
void RemoveMesh(Scene* s, Entity e);
void DrawMeshInspector(Scene* s, Entity e, EditorContext& ctx);