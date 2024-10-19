//
// Created by tzanellato on 12/07/24.
//

#ifndef HACHIKOMOUSE_H
#define HACHIKOMOUSE_H

#include "imgui.h"

namespace hachiko {

void MouseFrame();

bool IsMouseDown( ImGuiMouseButton button );
bool IsMouseClicked( ImGuiMouseButton button );
bool IsMouseDragging( ImGuiMouseButton button );
ImVec2 GetMouseDragDelta( ImGuiMouseButton button );

void ConsumeMouseEvents( ImGuiMouseButton button );
bool IsMouseClickReleased( ImGuiMouseButton button );

} // hachiko

#endif //HACHIKOMOUSE_H
