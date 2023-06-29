#pragma once
#include "config.h"

namespace vkMesh {

	vk::VertexInputBindingDescription getPosColorBindingDescription() {



		vk::VertexInputBindingDescription bindingDescription;
		bindingDescription.binding = 0;
		bindingDescription.stride = 5 * sizeof(float);
		bindingDescription.inputRate = vk::VertexInputRate::eVertex;

		return bindingDescription;

	}

	std::array<vk::VertexInputAttributeDescription, 2> getPosColorAttibuteDescription() {

		std::array<vk::VertexInputAttributeDescription, 2> attributes;

		attributes[0].binding;
		attributes[0].location;
		attributes[0].format = vk::Format::eR32G32Sfloat;
		attributes[0].offset = 0;

		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = vk::Format::eR32G32B32Sfloat;
		attributes[1].offset = 2 * sizeof(float);
		
		return attributes;
	}





}