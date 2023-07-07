#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include<list>
#include<unordered_map>
#include<stack>
#include<string.h>
#include<set>
#include<vector>
#include<chrono>
#include<thread>
#include<queue>
#include<optional>
#include<unordered_map>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


#define DEBUG true
#define BACKGROUND_COLOR std::array<float, 4>{0.55f, 0.8f, 1.00f, 1.0f}

struct BufferInput {

	size_t size;
	vk::BufferUsageFlags usage;
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
	vk::MemoryPropertyFlags memoryProperty;
};

struct Buffer {

	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;

};

//------Assets-------///
enum class meshTypes {
	TRIANGLE,
	SQUARE,
	LINE
};