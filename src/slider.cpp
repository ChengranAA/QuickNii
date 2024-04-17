#include "quicknii.hpp"

bool slider3d(const char* label){
    // styling 
    ImU32 const uFrameCol	= ImGui::GetColorU32(ImGuiCol_FrameBg) | 0xFF000000;
    ImU32 const uFrameCol2	= ImGui::GetColorU32(ImGuiCol_FrameBgActive);

    // Determine the size of the cube
    ImVec2 const vSizeSubstract = ImGui::CalcTextSize(std::to_string(1.0f).c_str()) * 1.1f;
    float const vSizeFull = ImGui::GetContentRegionAvail().x;
    float const fMinSize = (vSizeFull - vSizeSubstract.x * 0.8f);
    ImVec2 const vSize(fMinSize, fMinSize);

    
    float const fHeightOffset = ImGui::GetTextLineHeight();
    ImVec2 const vHeightOffset(0.0f, fHeightOffset);

    ImVec2 vPos = ImGui::GetCursorScreenPos();
    ImRect oRect(vPos + vHeightOffset, vPos + vSize + vHeightOffset);

    // Push the ID to the stack
    ImGuiID const id = ImGui::GetID(label);
    ImGui::PushID(id); 

    // Draw the cube 
    ImVec2 const vOriginPos = ImGui::GetCursorScreenPos();

    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    float const fX3 = vSize.x/3.0f;
    float const fY3 = vSize.y/3.0f;

    ImVec2 const vStart = oRect.Min;
    
    ImVec2 aPositions[] = {
      ImVec2(vStart.x,			        vStart.y + fX3),
      ImVec2(vStart.x,			        vStart.y + vSize.y),
      ImVec2(vStart.x + 2.0f*fX3,	    vStart.y + vSize.y),
      ImVec2(vStart.x + vSize.x,	    vStart.y + 2.0f*fY3),
      ImVec2(vStart.x + vSize.x,	    vStart.y),
      ImVec2(vStart.x + fX3,		    vStart.y)
    };

    pDrawList->AddPolyline(&aPositions[0], 6, uFrameCol2, true, 1.0f);

    // Cube Shape

    pDrawList->AddLine(
    oRect.Min + ImVec2(0.0f, vSize.y),
    oRect.Min + ImVec2(fX3, 2.0f*fY3), uFrameCol2, 1.0f);
    pDrawList->AddLine(
    oRect.Min + ImVec2(fX3, 2.0f*fY3),
    oRect.Min + ImVec2(vSize.x, 2.0f*fY3), uFrameCol2, 1.0f);
    pDrawList->AddLine(
    oRect.Min + ImVec2(fX3, 0.0f),
    oRect.Min + ImVec2(fX3, 2.0f*fY3), uFrameCol2, 1.0f);


    ImGui::PopID();  // Restore the previous ID stack.

    return 0;
}