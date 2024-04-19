#include "quicknii.hpp"

float Dist2(ImVec2 const v, ImVec2 const w)
{
  // return Sqr(v.x - w.x) + Sqr(v.y - w.y);
  return ImLengthSqr(v - w);
}

float DistOnSegmentSqr(ImVec2 const p, ImVec2 const v, ImVec2 const w)
{
  float l2 = Dist2(v, w);
  // if (l2 == 0.0f)
  //	return 0.0f;

  float t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;

  return ImSaturate(t) * Dist2(w, v);
}

float DistToSegmentSqr(ImVec2 const p, ImVec2 const v, ImVec2 const w)
{
  // float l2 = Dist2(v, w);
  ////if (l2 == 0.0f)
  ////	return Dist2(p, v);
  //
  // float t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
  // t = ImSaturate(t);
  //
  // return Dist2(p, ImVec2(v.x + t * (w.x - v.x),
  //					   v.y + t * (w.y - v.y)));

  float const lDx = v.x - w.x;
  float const lDy = v.y - w.y;

  float const dx = v.x - p.x;
  float const dy = v.y - p.y;

  float const l2 = ImSqrt(ImLengthSqr(w - v));

  return ImAbs(lDx * dy - lDy * dx) / l2;
}

bool SliderScalar3D(char const *pLabel, int *pValueX, int *pValueY, int *pValueZ, float const fMinX, float const fMaxX, float const fMinY, float const fMaxY, float const fMinZ, float const fMaxZ, float const fScale /*= 1.0f*/)
{
  assert(fMinX < fMaxX);
  assert(fMinY < fMaxY);
  assert(fMinZ < fMaxZ);

  ImGuiID const iID = ImGui::GetID(pLabel);

  ImVec2 const vSizeSubstract = ImGui::CalcTextSize(std::to_string(1.0f).c_str()) * 1.1f;

  float const vSizeFull = ImGui::GetContentRegionAvail().x;
  float const fMinSize = (vSizeFull - vSizeSubstract.x * 0.5f) * fScale * 0.75f;
  ImVec2 const vSize(fMinSize, fMinSize);

  float const fHeightOffset = ImGui::GetTextLineHeight();
  ImVec2 const vHeightOffset(0.0f, fHeightOffset);

  ImVec2 vPos = ImGui::GetCursorScreenPos() * 1.1f;
  ImRect oRect(vPos + vHeightOffset, vPos + vSize + vHeightOffset);

  ImGui::Text(pLabel);

  ImGui::PushID(iID);

  //ImU32 const uFrameCol = ImGui::GetColorU32(ImGuiCol_FrameBg) | 0xFF000000;

  ImVec4 const vGrey(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
  ImVec4 const vLightGrey(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
  ImVec4 const vLightBlue(50.0f / 255.0f, 50.0f / 255.0f, 70.0f / 255.0f, 0.8f);
  
  ImU32 const uFrameCol = ImGui::GetColorU32(vLightBlue);
  ImU32 const uFrameCol2 = ImGui::GetColorU32(vGrey);
  ImU32 const uFrameCol3 = ImGui::GetColorU32(vLightGrey);
  

  int &fX = *pValueX;
  int &fY = *pValueY;
  int &fZ = *pValueZ;

  float const fDeltaX = fMaxX - fMinX;
  float const fDeltaY = fMaxY - fMinY;
  float const fDeltaZ = fMaxZ - fMinZ;

  // ImVec2 const vOriginPos = ImGui::GetCursorScreenPos();

  ImDrawList *pDrawList = ImGui::GetWindowDrawList();

  float const fX3 = vSize.x / 3.0f;
  float const fY3 = vSize.y / 3.0f;

  ImVec2 const vStart = oRect.Min;

  ImVec2 aPositions[] = {
      ImVec2(vStart.x, vStart.y + fX3),
      ImVec2(vStart.x, vStart.y + vSize.y),
      ImVec2(vStart.x + 2.0f * fX3, vStart.y + vSize.y),
      ImVec2(vStart.x + vSize.x, vStart.y + 2.0f * fY3),
      ImVec2(vStart.x + vSize.x, vStart.y),
      ImVec2(vStart.x + fX3, vStart.y)};

  pDrawList->AddPolyline(&aPositions[0], 6, uFrameCol2, true, 1.0f);

  // Cube Shape
  pDrawList->AddLine(
      oRect.Min + ImVec2(0.0f, vSize.y),
      oRect.Min + ImVec2(fX3, 2.0f * fY3), uFrameCol3, 1.0f);
  pDrawList->AddLine(
      oRect.Min + ImVec2(fX3, 2.0f * fY3),
      oRect.Min + ImVec2(vSize.x, 2.0f * fY3), uFrameCol3, 1.0f);
  pDrawList->AddLine(
      oRect.Min + ImVec2(fX3, 0.0f),
      oRect.Min + ImVec2(fX3, 2.0f * fY3), uFrameCol3, 1.0f);

  ImGui::PopID();

  constexpr float fDragZOffsetX = 64.0f;

  ImRect oZDragRect(ImVec2(vStart.x + 2.0f * fX3 + fDragZOffsetX, vStart.y + 2.0f * fY3), ImVec2(vStart.x + vSize.x + fDragZOffsetX, vStart.y + vSize.y));

  ImVec2 const vMousePos = ImGui::GetMousePos();
  ImVec2 const vSecurity(7.0f, 7.0f);
  ImVec2 const vDragStart(oZDragRect.Min.x, oZDragRect.Max.y);
  ImVec2 const vDragEnd(oZDragRect.Max.x, oZDragRect.Min.y);
  // ImRect frame_bb = ImRect(oRect.Min - vSecurity, oRect.Max + vSecurity);
  bool bModified = false;
  //bool hovered;
  // bool held;
  if (ImGui::IsMouseHoveringRect(oZDragRect.Min - vSecurity, oZDragRect.Max + vSecurity) && ImGui::IsMouseDown(ImGuiMouseButton_Left))
  //bool pressed = ImGui::ButtonBehavior(oZDragRect, ImGui::GetID("##HandleZ"), &hovered, &held);
  //if (hovered && held)
  {
    if (DistToSegmentSqr(vMousePos, vDragStart, vDragEnd) < 100.0f) // 100 is arbitrary threshold
    {
      float const fMaxDist = ImSqrt(Dist2(vDragStart, vDragEnd));
      float const fDist = ImSaturate(ImSqrt(DistOnSegmentSqr(vMousePos, vDragStart, vDragEnd)) / fMaxDist);

      fZ = fDist * fDeltaZ * fDist + fMinZ;

      bModified = true;
    }
  }

  float const fScaleZ = (fZ - fMinZ) / fDeltaZ;

  ImVec2 const vRectStart(vStart.x, vStart.y + fX3);
  ImVec2 const vRectEnd(vStart.x + fX3, vStart.y);
  ImRect const oXYDrag((vRectEnd - vRectStart) * fScaleZ + vRectStart,
                       (vRectEnd - vRectStart) * fScaleZ + vRectStart + ImVec2(2.0f * fX3, 2.0f * fY3));
  if (ImGui::IsMouseHoveringRect(oXYDrag.Min - vSecurity, oXYDrag.Max + vSecurity) && ImGui::IsMouseDown(ImGuiMouseButton_Left))
  // pressed = ImGui::ButtonBehavior(oXYDrag, ImGui::GetID("##Zone"), &hovered, &held);
  // if (hovered && held)
  {
    ImVec2 const vLocalPos = ImGui::GetMousePos() - oXYDrag.Min;

    fX = vLocalPos.x / (oXYDrag.Max.x - oXYDrag.Min.x) * fDeltaX + fMinX;
    fY = fDeltaY - vLocalPos.y / (oXYDrag.Max.y - oXYDrag.Min.y) * fDeltaY + fMinY;

    bModified = true;
  }

  fX = ImClamp((float)fX, fMinX, fMaxX);
  fY = ImClamp((float)fY, fMinY, fMaxY);
  fZ = ImClamp((float)fZ, fMinZ, fMaxZ);

  int16_t fx = static_cast<int16_t>(fX);
  fX =  (int16_t)fx;

  int16_t fy = static_cast<int16_t>(fY);
  fY =  (int16_t)fy;

  int16_t fz = static_cast<int16_t>(fZ);
  fZ =  (int16_t)fz;

  float const fScaleX = (fX - fMinX) / fDeltaX;
  float const fScaleY = 1.0f - (fY - fMinY) / fDeltaY;

  ImVec4 const vBlue(70.0f / 255.0f, 50.0f / 255.0f, 230.0f / 255.0f, 1.0f);
  ImVec4 const vOrange(0.0f / 255.0f, 200.0f / 255.0f, 0.0f / 255.0f, 1.0f);

  ImS32 const uBlue = ImGui::GetColorU32(vBlue); 
  ImS32 const uOrange = ImGui::GetColorU32(vOrange); // not actually orange, there is too many uOrange reference in the code to change it now

  constexpr float fBorderThickness = 2.0f;  // TODO: move as Style
  constexpr float fLineThickness = 3.0f;    // TODO: move as Style
  constexpr float fHandleRadius = 7.0f;     // TODO: move as Style
  constexpr float fHandleOffsetCoef = 2.0f; // TODO: move as Style

  pDrawList->AddLine(
      vDragStart,
      vDragEnd, uFrameCol2, 1.0f);
  pDrawList->AddRectFilled(
      oXYDrag.Min, oXYDrag.Max, uFrameCol);

  constexpr float fCursorOff = 10.0f;
  float const fXLimit = fCursorOff / oXYDrag.GetWidth();
  float const fYLimit = fCursorOff / oXYDrag.GetHeight();
  float const fZLimit = fCursorOff / oXYDrag.GetWidth();

  char pBufferX[16];
  char pBufferY[16];
  char pBufferZ[16];

  ImFormatString(pBufferX, IM_ARRAYSIZE(pBufferX), "%d", *(int const *)&fX);
  ImFormatString(pBufferY, IM_ARRAYSIZE(pBufferY), "%d", *(int const *)&fY);
  ImFormatString(pBufferZ, IM_ARRAYSIZE(pBufferZ), "%d", *(int const *)&fZ);


  ImU32 const uTextCol = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);

  ImVec2 const vCursorPos((oXYDrag.Max.x - oXYDrag.Min.x) * fScaleX + oXYDrag.Min.x, (oXYDrag.Max.y - oXYDrag.Min.y) * fScaleY + oXYDrag.Min.y);

  ImGui::SetWindowFontScale(0.75f);

  ImVec2 const vXSize = ImGui::CalcTextSize(pBufferX);
  ImVec2 const vYSize = ImGui::CalcTextSize(pBufferY);
  // ImVec2 const vZSize = ImGui::CalcTextSize(pBufferZ);

  ImVec2 const vTextSlideXMin = oRect.Min + ImVec2(0.0f, vSize.y);
  ImVec2 const vTextSlideXMax = oRect.Min + ImVec2(2.0f * fX3, vSize.y);
  ImVec2 const vXTextPos((vTextSlideXMax - vTextSlideXMin) * fScaleX + vTextSlideXMin);

  ImVec2 const vTextSlideYMin = oRect.Min + ImVec2(vSize.x, 2.0f * fY3);
  ImVec2 const vTextSlideYMax = oRect.Min + ImVec2(vSize.x, 0.0f);
  ImVec2 const vYTextPos((vTextSlideYMax - vTextSlideYMin) * (1.0f - fScaleY) + vTextSlideYMin);

  ImVec2 const vTextSlideZMin = vDragStart + ImVec2(fCursorOff, fCursorOff);
  ImVec2 const vTextSlideZMax = vDragEnd + ImVec2(fCursorOff, fCursorOff);
  ImVec2 const vZTextPos((vTextSlideZMax - vTextSlideZMin) * fScaleZ + vTextSlideZMin);

  ImVec2 const vHandlePosX = vXTextPos + ImVec2(0.0f, vXSize.y + fHandleOffsetCoef * fCursorOff);
  ImVec2 const vHandlePosY = vYTextPos + ImVec2(vYSize.x + (fHandleOffsetCoef + 1.0f) * fCursorOff, 0.0f);

  // ImRect handle_x_bb = ImRect(vHandlePosX - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosX + ImVec2(fHandleRadius, fHandleRadius) + vSecurity);
  // ImRect handle_y_bb = ImRect(vHandlePosY - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosY + ImVec2(fHandleRadius, fHandleRadius) + vSecurity);
  // ImRect handle_z_bb = ImRect(vHandlePosY - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosY + ImVec2(fHandleRadius, fHandleRadius) + vSecurity);

  if (ImGui::IsMouseHoveringRect(vHandlePosX - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosX + ImVec2(fHandleRadius, fHandleRadius) + vSecurity) &&
  	ImGui::IsMouseDown(ImGuiMouseButton_Left))
  // pressed = ImGui::ButtonBehavior(handle_x_bb, ImGui::GetID("##HandleX"), &hovered, &held);
  // if (hovered && held)
  {
    float const fCursorPosX = ImGui::GetMousePos().x - vStart.x;

    fX = fDeltaX * fCursorPosX / (2.0f * fX3) + fMinX;

    bModified = true;
  }
  else if (ImGui::IsMouseHoveringRect(vHandlePosY - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosY + ImVec2(fHandleRadius, fHandleRadius) + vSecurity) &&
  	ImGui::IsMouseDown(ImGuiMouseButton_Left))
  // pressed = ImGui::ButtonBehavior(handle_y_bb, ImGui::GetID("##HandleY"), &hovered, &held);
  // if (hovered && held)
  {
    float const fCursorPosY = ImGui::GetMousePos().y - vStart.y;

    fY = fDeltaY * (1.0f - fCursorPosY / (2.0f * fY3)) + fMinY;

    bModified = true;
  }

  pDrawList->AddText(
      ImVec2(
          ImMin(ImMax(vXTextPos.x - vXSize.x * 0.5f, vTextSlideXMin.x), vTextSlideXMax.x - vXSize.x),
          vXTextPos.y + fCursorOff),
      uTextCol,
      pBufferX);
  pDrawList->AddText(
      ImVec2(
          vYTextPos.x + fCursorOff,
          ImMin(ImMax(vYTextPos.y - vYSize.y * 0.5f, vTextSlideYMax.y), vTextSlideYMin.y - vYSize.y)),
      uTextCol,
      pBufferY);
  pDrawList->AddText(
      vZTextPos,
      uTextCol,
      pBufferZ);
  ImGui::SetWindowFontScale(1.0f);

  // Handles
  pDrawList->AddNgonFilled(vHandlePosX, fHandleRadius, uBlue, 4);
  pDrawList->AddNgonFilled(vHandlePosY, fHandleRadius, uBlue, 4);

  // Vertical Line
  if (fScaleY > 2.0f * fYLimit)
    pDrawList->AddLine(ImVec2(vCursorPos.x, oXYDrag.Min.y + fCursorOff), ImVec2(vCursorPos.x, vCursorPos.y - fCursorOff), uOrange, fLineThickness);
  if (fScaleY < 1.0f - 2.0f * fYLimit)
    pDrawList->AddLine(ImVec2(vCursorPos.x, oXYDrag.Max.y - fCursorOff), ImVec2(vCursorPos.x, vCursorPos.y + fCursorOff), uOrange, fLineThickness);

  // Horizontal Line
  if (fScaleX > 2.0f * fXLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Min.x + fCursorOff, vCursorPos.y), ImVec2(vCursorPos.x - fCursorOff, vCursorPos.y), uOrange, fLineThickness);
  if (fScaleX < 1.0f - 2.0f * fYLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Max.x - fCursorOff, vCursorPos.y), ImVec2(vCursorPos.x + fCursorOff, vCursorPos.y), uOrange, fLineThickness);

  // Line To Text
  // X
  if (fScaleZ > 2.0f * fZLimit)
    pDrawList->AddLine(
        ImVec2(vCursorPos.x - 0.5f * fCursorOff, oXYDrag.Max.y + 0.5f * fCursorOff),
        ImVec2(vXTextPos.x + 0.5f * fCursorOff, vXTextPos.y - 0.5f * fCursorOff), uOrange, fLineThickness);
  else
    pDrawList->AddLine(
        ImVec2(vCursorPos.x, oXYDrag.Max.y),
        ImVec2(vXTextPos.x, vXTextPos.y), uOrange, 1.0f);
  pDrawList->AddCircleFilled(vXTextPos, 2.0f, uOrange, 3);
  // Y
  if (fScaleZ < 1.0f - 2.0f * fZLimit)
    pDrawList->AddLine(
        ImVec2(oXYDrag.Max.x + 0.5f * fCursorOff, vCursorPos.y - 0.5f * fCursorOff),
        ImVec2(vYTextPos.x - 0.5f * fCursorOff, vYTextPos.y + 0.5f * fCursorOff), uOrange, fLineThickness);
  else
    pDrawList->AddLine(
        ImVec2(oXYDrag.Max.x, vCursorPos.y),
        ImVec2(vYTextPos.x, vYTextPos.y), uOrange, 1.0f);
  pDrawList->AddCircleFilled(vYTextPos, 2.0f, uOrange, 3);

  // Borders::Right
  pDrawList->AddCircleFilled(ImVec2(oXYDrag.Max.x, vCursorPos.y), 2.0f, uOrange, 3);
  if (fScaleY > fYLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Min.y), ImVec2(oXYDrag.Max.x, vCursorPos.y - fCursorOff), uBlue, fBorderThickness);
  if (fScaleY < 1.0f - fYLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Max.y), ImVec2(oXYDrag.Max.x, vCursorPos.y + fCursorOff), uBlue, fBorderThickness);
  // Borders::Top
  pDrawList->AddCircleFilled(ImVec2(vCursorPos.x, oXYDrag.Min.y), 2.0f, uOrange, 3);
  if (fScaleX > fXLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Min.y), ImVec2(vCursorPos.x - fCursorOff, oXYDrag.Min.y), uBlue, fBorderThickness);
  if (fScaleX < 1.0f - fXLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Min.y), ImVec2(vCursorPos.x + fCursorOff, oXYDrag.Min.y), uBlue, fBorderThickness);
  // Borders::Left
  pDrawList->AddCircleFilled(ImVec2(oXYDrag.Min.x, vCursorPos.y), 2.0f, uOrange, 3);
  if (fScaleY > fYLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Min.y), ImVec2(oXYDrag.Min.x, vCursorPos.y - fCursorOff), uBlue, fBorderThickness);
  if (fScaleY < 1.0f - fYLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Max.y), ImVec2(oXYDrag.Min.x, vCursorPos.y + fCursorOff), uBlue, fBorderThickness);
  // Borders::Bottom
  pDrawList->AddCircleFilled(ImVec2(vCursorPos.x, oXYDrag.Max.y), 2.0f, uOrange, 3);
  if (fScaleX > fXLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Max.y), ImVec2(vCursorPos.x - fCursorOff, oXYDrag.Max.y), uBlue, fBorderThickness);
  if (fScaleX < 1.0f - fXLimit)
    pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Max.y), ImVec2(vCursorPos.x + fCursorOff, oXYDrag.Max.y), uBlue, fBorderThickness);

  pDrawList->AddLine(
      oRect.Min + ImVec2(0.0f, fY3),
      oRect.Min + ImVec2(2.0f * fX3, fY3), uFrameCol2, 1.0f);
  pDrawList->AddLine(
      oRect.Min + ImVec2(2.0f * fX3, fY3),
      oRect.Min + ImVec2(vSize.x, 0.0f), uFrameCol2, 1.0f);
  pDrawList->AddLine(
      oRect.Min + ImVec2(2.0f * fX3, fY3),
      oRect.Min + ImVec2(2.0f * fX3, vSize.y), uFrameCol2, 1.0f);

  // Cursor
  pDrawList->AddCircleFilled(vCursorPos, 5.0f, uBlue, 16);
  // CursorZ
  pDrawList->AddNgonFilled((vDragEnd - vDragStart) * fScaleZ + vDragStart, fHandleRadius, uBlue, 4);

  //ImGui::Dummy(vHeightOffset);
  //ImGui::Dummy(vHeightOffset * 1.25f);
  //ImGui::Dummy(vSize);

  return bModified;
}