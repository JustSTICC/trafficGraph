#pragma once
#include "config.h"

namespace vkUtil {

	

	Buffer createBuffer(BufferInput input);
	uint32_t findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, uint32_t supportedMemoryIndices, vk::MemoryPropertyFlags requestProperties);;
	void allocateBufferMemory(Buffer& buffer, const BufferInput& input);

}