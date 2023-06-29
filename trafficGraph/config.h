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

struct BufferInput {

	size_t size;
	vk::BufferUsageFlags usage;
	vk::Device logicalDevice;
	vk::PhysicalDevice physicalDevice;
};

struct Buffer {

	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;

};

//------Assets-------///
enum class meshTypes {
	TRIANGLE,
	SQUARE,
	STAR
};