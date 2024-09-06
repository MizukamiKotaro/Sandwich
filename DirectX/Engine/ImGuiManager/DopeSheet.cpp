#include "DopeSheet.h"
#ifdef _DEBUG
#include "Externals/imgui/imgui.h"
#include "Editor/KeyframeData.h"
#include <functional>

namespace ImGuiCommon {
    inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }
    inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    void DrawTimelineLabels(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, float scale, float offset) {
        for (int i = 0; i < endPos.x / (10.0f * scale); ++i) {
            float linePos = i * 10.0f * scale - offset;
            ImVec2 lineStart = startPos + ImVec2(linePos, 0.0f);
            ImVec2 lineEnd = lineStart + ImVec2(0.0f, endPos.y - startPos.y);
            if (i % 10 == 0) {
                drawList->AddText(lineStart, IM_COL32(255, 255, 255, 255), std::to_string(i).c_str());
            }
        }
    }

    void DrawTimelineLines(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, float scale, float offset) {
        for (int i = 0; i < endPos.x / (10.0f * scale); ++i) {
            float linePos = i * 10.0f * scale - offset;
            ImVec2 lineStart = startPos + ImVec2(linePos, 0.0f);
            ImVec2 lineEnd = lineStart + ImVec2(0.0f, endPos.y - startPos.y);
            if (i % 10 == 0) {
                drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 255), 2.0f);
            }
            else if (i % 5 == 0) {
                drawList->AddLine(lineStart, lineEnd, IM_COL32(200, 200, 200, 255), 1.0f);
            }
            else {
                drawList->AddLine(lineStart, lineEnd, IM_COL32(150, 150, 150, 255), 0.5f);
            }
        }
    }

    void CloseTree(TimerTrack& track) {
        track.isOpen = false;
        for (TimerTrack& child : track.children) {
            CloseTree(child);
        }
    }

    void DrawTrackName(TimerTrack& track, int& trackIndex) {
        track.isOpen = true;
        // トラック名を描画
        if (track.children.empty()) {
            ImGui::Text(track.name.c_str());
        }
        else {
            if (ImGui::TreeNodeEx((void*)(intptr_t)trackIndex, ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth, "  %s", track.name.c_str())) {
                track.isOpen = true;
                // 子トラックを描画
                for (TimerTrack& child : track.children) {
                    trackIndex++;
                    DrawTrackName(child, trackIndex);
                }
                ImGui::TreePop();
            }
            else {
                for (TimerTrack& child : track.children) {
                    CloseTree(child);
                }
            }
        }
    }

    void DrawTrack(ImDrawList* drawList, TimerTrack& track, ImVec2& cursorScreenPos, ImVec2 contentRegion, float timelineScale, float scrollOffset, float& currentY, int depth, ImVec2& rightClickPos, int& clickedTrackIndex, int& trackIndex) {
        float trackHeight = 20.0f;
        ImVec2 trackStart = cursorScreenPos + ImVec2(0.0f, currentY);
        ImVec2 keyframeStart = trackStart + ImVec2(100.0f, 0.0f); // 階層に応じてインデント

        ImGui::SetCursorScreenPos(trackStart);
        if (track.isOpen) {
            if (track.children.empty()) {
                // キーフレーム行を描画
                drawList->AddRectFilled(keyframeStart, keyframeStart + ImVec2(contentRegion.x - 150.0f, trackHeight), IM_COL32(80, 80, 80, 255));

                // トラックの境界線を描画
                drawList->AddLine(ImVec2(trackStart.x, trackStart.y + trackHeight), ImVec2(trackStart.x + contentRegion.x, trackStart.y + trackHeight), IM_COL32(100, 100, 100, 255));

                // タイムラインの目盛りを描画
                DrawTimelineLines(drawList, keyframeStart, keyframeStart + ImVec2(contentRegion.x - 150.0f, trackHeight), timelineScale, scrollOffset);

                bool rightClickOnEmptySpace = true;
                for (TimerKeyframe& keyframe : track.keyframes) {
                    float keyframePos = keyframe.frame * 10.0f * timelineScale - scrollOffset;
                    ImVec2 keyframeCenter = keyframeStart + ImVec2(keyframePos, trackHeight / 2);
                    drawList->AddCircleFilled(keyframeCenter, 5.0f, keyframe.selected ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));

                    ImGui::SetCursorScreenPos(keyframeCenter - ImVec2(5.0f, 5.0f));
                    ImGui::InvisibleButton("##Keyframe", ImVec2(10, 10));
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                        keyframe.selected = !keyframe.selected;
                        ImGui::OpenPopup("KeyframeContextMenu");
                        rightClickOnEmptySpace = false;
                    }
                    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                        if (keyframe.selected) {
                            keyframe.subFrame += ImGui::GetIO().MouseDelta.x / (10.0f * timelineScale);
                            keyframe.frame = float(int(keyframe.subFrame));
                            if (keyframe.subFrame < 0) {
                                keyframe.frame = 0;
                                keyframe.subFrame = 0;
                            }
                        }
                    }
                }

                if (rightClickOnEmptySpace) {
                    ImGui::SetCursorScreenPos(keyframeStart);
                    ImGui::InvisibleButton("##EmptySpace", ImVec2(contentRegion.x - 150.0f, trackHeight));
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                        rightClickPos = ImGui::GetMousePos();
                        clickedTrackIndex = trackIndex;
                        ImGui::OpenPopup("TrackContextMenu");
                    }
                }
                currentY += trackHeight + 2.0f;
            }
            else {
                float timelineWidth = (contentRegion.x - 100.0f) * timelineScale;
                DrawTimelineLabels(drawList, keyframeStart, keyframeStart + ImVec2(100.0f + timelineWidth, 20.0f), timelineScale, scrollOffset);

                currentY += trackHeight + 2.0f;
                for (TimerTrack& child : track.children) {
                    trackIndex++;
                    DrawTrack(drawList, child, cursorScreenPos, contentRegion, timelineScale, scrollOffset, currentY, depth + 1, rightClickPos, clickedTrackIndex, trackIndex);
                }
            }
        }
    }

    void EditKeyframes(TimerTrack& track) {
        for (TimerKeyframe& kf : track.keyframes) {
            if (kf.selected) {
                kf.isOpenImGui = true;
            }
        }
        for (TimerTrack& child : track.children) {
            EditKeyframes(child);
        }
    }

    void DeleteKeyframes(TimerTrack& track) {
        track.keyframes.erase(
            std::remove_if(track.keyframes.begin(), track.keyframes.end(), [](const TimerKeyframe& kf) { return kf.selected; }),
            track.keyframes.end()
        );
        for (TimerTrack& child : track.children) {
            DeleteKeyframes(child);
        }
    }

    void CopyKeyframes(TimerTrack& track) {
        bool isSelected = false;
        int no = 0;
        for (TimerKeyframe& kf : track.keyframes) {
            if (kf.selected) {
                track.copyNum = no;
                isSelected = true;
                break;
            }
            no++;
        }
        if (isSelected) {
            return;
        }
        for (TimerTrack& child : track.children) {
            CopyKeyframes(child);
        }
    }

    void AddKeyframe(TimerTrack& track, int clickedTrackIndex, int& currentIndex, float framePos) {
        if (currentIndex == clickedTrackIndex) {
            track.keyframes.push_back({ framePos, framePos, false, false });
            if (track.copyNum == -1) {
                track.copyNum = int(track.keyframes.size()) - 1;
            }
        }
        currentIndex++;
        if (track.isOpen) {
            for (TimerTrack& child : track.children) {
                AddKeyframe(child, clickedTrackIndex, currentIndex, framePos);
            }
        }
    }

    void DopeSheet() {
        static float timelineScale = 1.0f;
        static float scrollOffsetX = 0.0f;
        static float scrollOffsetY = 0.0f;
        static int currentFrame = 0;
        static std::vector<TimerTrack> tracks = {
            {"Position", 0, {{10,10, false, false}, {20,20, false, false}, {30,30, false, false}}, {
                {"X", 0, {{5,5, false, false}, {15,15, false, false}, {25,25, false, false}}},
                {"Y", 0, {{8,8, false, false}, {18,18, false, false}, {28,28, false, false}}}
            }},
            {"Rotation", 0, {{15,15, false, false}, {25,25, false, false}, {35,35, false, false}}},
            {"Scale", 0, {{12,12, false, false}, {22,22, false, false}, {32,32, false, false}}}
        };

        int trackIndex = 0;

        ImGui::Begin("Dope Sheet Names");
        ImGui::Text(" ");
        ImGui::Text(" ");
        for (TimerTrack& track : tracks) {
            DrawTrackName(track, trackIndex);
            trackIndex++;
        }
        ImGui::End();

        ImGui::Begin("Dope Sheet Keyframes");

        ImGui::SliderFloat("ズーム", &timelineScale, 0.1f, 10.0f, "Zoom %.1f");

        ImVec2 contentRegion = ImGui::GetContentRegionAvail();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();

        drawList->AddRectFilled(cursorScreenPos, cursorScreenPos + contentRegion, IM_COL32(50, 50, 50, 255));

        float timelineWidth = (contentRegion.x - 100.0f) * timelineScale;
        DrawTimelineLabels(drawList, cursorScreenPos + ImVec2(100.0f, 0.0f), cursorScreenPos + ImVec2(100.0f + timelineWidth, 20.0f), timelineScale, scrollOffsetX);

        // Adjust ImGui::BeginChild to enable both horizontal and vertical scrolling
        ImGui::BeginChild("TimelineRegion", ImVec2(contentRegion.x, contentRegion.y), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
        scrollOffsetX = ImGui::GetScrollX();
        scrollOffsetY = ImGui::GetScrollY();

        static ImVec2 rightClickPos;
        static int clickedTrackIndex = -1;
        trackIndex = 0;

        float currentY = 20.0f - scrollOffsetY; // Adjust currentY by vertical scroll offset

        for (TimerTrack& track : tracks) {
            DrawTrack(drawList, track, cursorScreenPos, contentRegion, timelineScale, scrollOffsetX, currentY, 0, rightClickPos, clickedTrackIndex, trackIndex);
            trackIndex++;
        }

        if (ImGui::BeginPopup("KeyframeContextMenu")) {
            if (ImGui::MenuItem("編集")) {
                for (TimerTrack& track : tracks) {
                    EditKeyframes(track);
                }
            }
            if (ImGui::MenuItem("削除")) {
                for (TimerTrack& track : tracks) {
                    DeleteKeyframes(track);
                }
            }
            if (ImGui::MenuItem("コピー")) {
                for (TimerTrack& track : tracks) {
                    CopyKeyframes(track);
                }
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("TrackContextMenu")) {
            if (ImGui::MenuItem("キーフレーム追加")) {
                if (clickedTrackIndex >= 0) {
                    int index = 0;
                    float framePos = (rightClickPos.x - cursorScreenPos.x - 100.0f + scrollOffsetX) / (10.0f * timelineScale);
                    for (TimerTrack& track : tracks) {
                        AddKeyframe(track, clickedTrackIndex, index, framePos);
                    }
                }
            }
            ImGui::EndPopup();
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
#endif // _DEBUG