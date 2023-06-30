#pragma once
#include"config.h"
#include"memory.h"

struct FinalizationChunk {

	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::Queue queue;
	vk::CommandBuffer commandBuffer;

};

class VertexMenagerie {
public:
	VertexMenagerie();
	~VertexMenagerie();
	void consume(meshTypes type, std::vector<float> vertexData);
	void finalize(FinalizationChunk finalizationChunk);
	Buffer vertexBuffer;
	std::unordered_map<meshTypes, int> offsets;
	std::unordered_map<meshTypes, int> sizes;

private:
	int offset;
	vk::Device logicalDevice;
	std::vector<float> lump;

};
