#pragma once
#include "config.h"
#include "memory.h"

class SquareMesh {
public:
	SquareMesh(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	~SquareMesh();
	Buffer vertexBuffer;
private:
	vk::Device logicalDevice;
};