#pragma once
#include"config.h"

namespace vkInit {

	struct descriptorSetLayoutData {

		int count;
		std::vector<int> indices;
		std::vector<vk::DescriptorType> types;
		std::vector<int> counts;
		std::vector<vk::ShaderStageFlags> stages;

	};

	vk::DescriptorSetLayout make_descriptor_set_layout(vk::Device device, const descriptorSetLayoutData& bindings) {

		std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.reserve(bindings.count);

		for (int i = 0; i < bindings.count; i++) {


			vk::DescriptorSetLayoutBinding layoutBinding;
			layoutBinding.binding = bindings.indices[i];
			layoutBinding.descriptorType = bindings.types[i];
			layoutBinding.descriptorCount = bindings.counts[i];
			layoutBinding.stageFlags = bindings.stages[i];
			layoutBindings.push_back(layoutBinding);
		}

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
		layoutInfo.flags = vk::DescriptorSetLayoutCreateFlagBits();
		layoutInfo.bindingCount = bindings.count;
		layoutInfo.pBindings = layoutBindings.data();

		try {
			return device.createDescriptorSetLayout(layoutInfo);
		}
		catch (vk::SystemError err) {
			return nullptr;
		}
	}

	vk::DescriptorPool make_descriptor_pool(vk::Device device, uint32_t size,  const descriptorSetLayoutData& bindings) {

		std::vector<vk::DescriptorPoolSize> poolSizes;

		for (int i = 0; i < bindings.count; i++) {

			vk::DescriptorPoolSize poolSize;
			poolSize.type = bindings.types[i];
			poolSize.descriptorCount = size;
			poolSizes.push_back(poolSize);
		}

		vk::DescriptorPoolCreateInfo poolInfo;

		poolInfo.flags = vk::DescriptorPoolCreateFlags();
		poolInfo.maxSets = size;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();

		try {

			return device.createDescriptorPool(poolInfo);

		}
		catch (vk::SystemError err) {
			std::cout << "Failed to make descriptor pool"<<std::endl;
			return nullptr;

		}
	}

	vk::DescriptorSet allocate_descriptor_set(
		vk::Device device, vk::DescriptorPool descriptorPool,
		vk::DescriptorSetLayout layout) {

		vk::DescriptorSetAllocateInfo allocationInfo;

		allocationInfo.descriptorPool = descriptorPool;
		allocationInfo.descriptorSetCount = 1;
		allocationInfo.pSetLayouts = &layout;

		try {
			return device.allocateDescriptorSets(allocationInfo)[0];
		}
		catch (vk::SystemError err) {
			std::cout << "Failed to allocate descriptor set from pool"<<std::endl;
			return nullptr;
		}
	}
}