//#pragma once
//#include "IEditor.h"
//#include <map>
//
//class TimerStageEditor : public IEditor
//{
//public:
//	TimerStageEditor(const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_");
//
//	void AddItem(const std::string& key, const int32_t& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_" )override;
//	void AddItem(const std::string& key, const float& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void AddItem(const std::string& key, const Vector2& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void AddItem(const std::string& key, const Vector3& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void AddItem(const std::string& key, const Vector4& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void AddItem(const std::string& key, const bool& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//
//	void AddItemColor(const std::string& key, const Vector4& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//
//	const int32_t& GetIntValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//	const float& GetFloatValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//	const Vector2& GetVector2Value(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//	const Vector3& GetVector3Value(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//	const Vector4& GetVector4Value(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//	const bool& GetBoolValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//
//	const Vector4& GetColor(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const override;
//
//	void SetVariable(const std::string& key, const int32_t& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void SetVariable(const std::string& key, const float& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void SetVariable(const std::string& key, const Vector2& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void SetVariable(const std::string& key, const Vector3& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void SetVariable(const std::string& key, const Vector4& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//	void SetVariable(const std::string& key, const bool& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") override;
//
//	void Update(const float& time);
//
//private:
//	TimerStageEditor() = default;
//
//	template <typename T>
//	void AddItem(const std::string& key, const T& value, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
//
//	template <typename T>
//	struct TimerKeyframe
//	{
//		float time;
//		T value;
//	};
//
//	template <typename T>
//	struct TimerAnimationCurve
//	{
//		float currentTime;
//		float lastTime;
//		T value;
//		std::vector<TimerKeyframe<T>> keyframes;
//	};
//
//	template <typename T>
//	struct TimerAnimationMap
//	{
//		std::map<std::string, TimerAnimationCurve<T>> aniamationMap;
//	};
//
//	float currentTime_;
//	TimerAnimationMap<int32_t> intDataMap_;
//	TimerAnimationMap<float> floatDataMap_;
//	TimerAnimationMap<Vector2> vector2DataMap_;
//	TimerAnimationMap<Vector3> vector3DataMap_;
//	TimerAnimationMap<Vector4> vector4DataMap_;
//	TimerAnimationMap<bool> boolDataMap_;
//};