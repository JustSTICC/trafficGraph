#pragma once
#include "config.h"
#include"shaders.h"
#include"render_structs.h"
#include "mesh.h"

namespace vkInit {

	struct GraphicsPipeLineInBundle {
		vk::Device device;
		std::string vertexFilepath;
		std::string fragmentFilepath;
		vk::Extent2D swapchainExtent;
		vk::Format swapchainImageFormat;
		vk::DescriptorSetLayout descriptorSetLayout;

	};

	struct  GraphicsPipeLineOutBundle {
		vk::PipelineLayout layout;
		vk::RenderPass renderpass;
		vk::Pipeline pipeline;
	};

	vk::PipelineLayout make_pipeline_layout(vk::Device device, vk::DescriptorSetLayout layout, bool debug) {

		vk::PipelineLayoutCreateInfo layaoutInfo;
		layaoutInfo.flags = vk::PipelineLayoutCreateFlags();

		layaoutInfo.setLayoutCount = 1;
		layaoutInfo.pSetLayouts = &layout;

		layaoutInfo.pushConstantRangeCount = 0;


		try {
			return device.createPipelineLayout(layaoutInfo);
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create pipeline layout!" << std::endl;
			}
		}

	}

	vk::RenderPass make_renderpass(vk::Device device, vk::Format swapchainImageFormat, bool debug) {

		vk::AttachmentDescription colorAttachment = {};
		colorAttachment.flags = vk::AttachmentDescriptionFlags();
		colorAttachment.format = swapchainImageFormat;
		colorAttachment.samples = vk::SampleCountFlagBits::e1;
		colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
		colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
		colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

		vk::AttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

		vk::SubpassDescription subpass = {};
		subpass.flags = vk::SubpassDescriptionFlags();
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		vk::RenderPassCreateInfo renderpassInfo = {};
		renderpassInfo.flags = vk::RenderPassCreateFlags();
		renderpassInfo.attachmentCount = 1;
		renderpassInfo.pAttachments = &colorAttachment;
		renderpassInfo.subpassCount = 1;
		renderpassInfo.pSubpasses = &subpass;

		try {
			return device.createRenderPass(renderpassInfo);
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create renderpass!" << std::endl;
			}
		}
	}

	vk::PipelineVertexInputStateCreateInfo make_vertex_input_info(vk::VertexInputBindingDescription bindingDescription, std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions) {
		
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = 2;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		return vertexInputInfo;
	}

	vk::PipelineInputAssemblyStateCreateInfo make_input_assemply_info() {

		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
		inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;

		return inputAssemblyInfo;

	}

	vk::PipelineShaderStageCreateInfo make_vertex_shader_info(vk::ShaderModule vertexShader) {

		vk::PipelineShaderStageCreateInfo vertexShaderInfo;
		vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
		vertexShaderInfo.module = vertexShader;
		vertexShaderInfo.pName = "main";

		return vertexShaderInfo;
	}

	vk::Viewport make_viewport(GraphicsPipeLineInBundle specification) {

		vk::Viewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = specification.swapchainExtent.width;
		viewport.height = specification.swapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		return viewport;
	}

	vk::Rect2D make_scissor(GraphicsPipeLineInBundle specification) {
		
		vk::Rect2D scissor;
		scissor.offset.x = 0.0f;
		scissor.offset.y = 0.0f;
		scissor.extent = specification.swapchainExtent;

		return scissor;
	}

	vk::PipelineViewportStateCreateInfo make_viewport_state(vk::Viewport viewport, vk::Rect2D scissor) {

		vk::PipelineViewportStateCreateInfo viewportState;
		viewportState.flags = vk::PipelineViewportStateCreateFlags();
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		return viewportState;
	}

	vk::PipelineRasterizationStateCreateInfo make_rasterizer() {

		vk::PipelineRasterizationStateCreateInfo rasterizer;
		rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = vk::PolygonMode::eFill;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = vk::CullModeFlagBits::eBack;
		rasterizer.frontFace = vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE;

		return rasterizer;
	}

	vk::PipelineShaderStageCreateInfo make_fragment_shader_info(vk::ShaderModule fragmentShader) {

		vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
		fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
		fragmentShaderInfo.module = fragmentShader;
		fragmentShaderInfo.pName = "main";

		return fragmentShaderInfo;

	}

	vk::PipelineMultisampleStateCreateInfo make_multisampling() {

		vk::PipelineMultisampleStateCreateInfo multisampling;
		multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

		return multisampling;

	}

	vk::PipelineColorBlendStateCreateInfo make_color_blending(vk::PipelineColorBlendAttachmentState colorBlendAttachment) {

		vk::PipelineColorBlendStateCreateInfo colorBlending;
		colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = vk::LogicOp::eCopy;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		return colorBlending;
	}

	GraphicsPipeLineOutBundle create_graphics_pipeline(GraphicsPipeLineInBundle specification, bool debug) {

		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.flags = vk::PipelineCreateFlags();
		
		//Shader stages, to be populated later
		std::vector <vk::PipelineShaderStageCreateInfo> shaderStages;
		
		//Vertex input
		vk::VertexInputBindingDescription bindingDescription = vkMesh::getPosColorBindingDescription();
		std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = vkMesh::getPosColorAttibuteDescription();
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = make_vertex_input_info(bindingDescription, attributeDescriptions);
		pipelineInfo.pVertexInputState = &vertexInputInfo;

		//Input Assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = make_input_assemply_info();
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

		//Vertex Shader
		if (debug) {
			std::cout << "Create vertex shader module" << std::endl;
		}
		vk::ShaderModule vertexShader = vkUtil::createModule(specification.vertexFilepath, specification.device, debug);
		vk::PipelineShaderStageCreateInfo vertexShaderInfo = make_vertex_shader_info(vertexShader);
		shaderStages.push_back(vertexShaderInfo);

		//Viewport and Scissor
		vk::Viewport viewport = make_viewport(specification);
		vk::Rect2D scissor = make_scissor(specification);

		vk::PipelineViewportStateCreateInfo viewportState = make_viewport_state(viewport,scissor);
		pipelineInfo.pViewportState = &viewportState;

		//Rasterizer
		vk::PipelineRasterizationStateCreateInfo rasterizer = make_rasterizer();
		pipelineInfo.pRasterizationState = &rasterizer;

		//Fragment Shader
		if (debug) {
			std::cout << "Create vertex shader module" << std::endl;
		}
		vk::ShaderModule fragmentShader = vkUtil::createModule(specification.fragmentFilepath, specification.device, debug);
		vk::PipelineShaderStageCreateInfo fragmentShaderInfo = make_fragment_shader_info(fragmentShader);
		shaderStages.push_back(fragmentShaderInfo);
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();

		//Multisampling
		vk::PipelineMultisampleStateCreateInfo multisampling = make_multisampling();
		pipelineInfo.pMultisampleState = &multisampling;

		//Color Blend
		vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		colorBlendAttachment.blendEnable = VK_FALSE;
		vk::PipelineColorBlendStateCreateInfo colorBlending = make_color_blending(colorBlendAttachment);
		pipelineInfo.pColorBlendState = &colorBlending;

		//Pipeline Layout
		if(debug) {
			std::cout << "Create Pipeline Layout" << std::endl;
		}
		vk::PipelineLayout layout = make_pipeline_layout(specification.device, specification.descriptorSetLayout, debug);
		pipelineInfo.layout = layout;

		//Renderpass
		if (debug) {
			std::cout << "Create RenderPass" << std::endl;
		}
		vk::RenderPass renderpass = make_renderpass(specification.device, specification.swapchainImageFormat, debug);

		//Extra stuff
		pipelineInfo.renderPass = renderpass;

		//Make the pipeline
		if (debug) {
			std::cout << "Create Graphics Pipeline" << std::endl;
		}
		vk::Pipeline graphicsPipeline;
		try {
			graphicsPipeline = (specification.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create Graphics Pipeline!" << std::endl;
			}
		}


		GraphicsPipeLineOutBundle output = {};
		output.layout = layout;
		output.renderpass = renderpass;
		output.pipeline = graphicsPipeline;

		specification.device.destroyShaderModule(vertexShader);
		specification.device.destroyShaderModule(fragmentShader);
		return output;

	}
}
