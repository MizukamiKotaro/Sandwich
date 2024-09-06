#pragma once
#include <string>
#include <vector>
#include <map>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

struct TimerKeyframe {
    float frame;
    float subFrame;
    bool selected;
    bool isOpenImGui;
};

struct TimerTrack {
    std::string name;
    size_t copyNum;
    std::vector<TimerKeyframe> keyframes;
    std::vector<TimerTrack> children;
    bool isOpen = true; // 子トラックの展開状態
};

//template <typename T>
//struct TimerAnimationCurve
//{
//	float currentTime;
//	float lastTime;
//	T value;
//	std::vector<T> keyframes;
//};
//
//struct TimerAnimationData
//{
//    float currentTime;
//    std::string name;
//    std::map<std::string, TimerAnimationCurve<int32_t>> intDataMap;
//    std::map<std::string, TimerAnimationCurve<float>> floatDataMap;
//    std::map<std::string, TimerAnimationCurve<Vector2>> vector2DataMap;
//    std::map<std::string, TimerAnimationCurve<Vector3>> vector3DataMap;
//    std::map<std::string, TimerAnimationCurve<Vector4>> vector4DataMap;
//    std::map<std::string, TimerAnimationCurve<bool>> boolDataMap;
//};