#pragma once
#include <memory>
#include <unordered_map>

class ComputePipeline;
enum class ComputePipelineType;
class IComputePipelineFactory;

class ComputePipelineManager
{
public:

	static ComputePipelineManager* GetInstance();

	void Initialize();
	void PreCompute(ComputePipelineType type);

private:
	ComputePipelineManager() = default;
	~ComputePipelineManager() = default;
	ComputePipelineManager(const ComputePipelineManager&) = delete;
	ComputePipelineManager& operator=(const ComputePipelineManager&) = delete;

private:
	std::unordered_map<ComputePipelineType, std::unique_ptr<ComputePipeline>> pipelineMap_;

	ComputePipelineType currentPiplineType_;

	std::unique_ptr<IComputePipelineFactory> pipelineFactory_;
};