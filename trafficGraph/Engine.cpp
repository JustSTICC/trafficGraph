#include "engine.h"
#include "instance.h"
#include "logging.h"
#include"device.h"
#include"swapchain.h"
#include"pipeline.h"
#include"framebuffer.h"
#include "commands.h"
#include "sync.h"
#include"descriptors.h"




Engine::Engine(int width, int height, GLFWwindow* window, std::list<location::Location> graph, bool debugMode ) {

	this->width = width;
	this->height = height;
	this->window = window;
	this->debugMode = debugMode;
	this->graph = graph;

	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}


	make_instance();

	make_device();

	make_descriptor_set_layout();

	make_pipeline();

	finalize_setup();

	make_assets();

}



void Engine::make_instance() {

	instance = vkInit::make_instance(debugMode, "ID Tech 12");
	dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);


	if (debugMode) {
		debugMessenger = vkInit::make_debug_messenger(instance, dldi);
	}
	VkSurfaceKHR c_style_surface;
	if(glfwCreateWindowSurface(instance, window, nullptr, &c_style_surface) != VK_SUCCESS) {
		if (debugMode) {
			std::cout << "Failed to abstact the glfw surface for Vulkan.\n";
		}
	}
	else if (debugMode) {
		std::cout << "Successfuly abstacted the glfw surface for Vulkan.\n";
	}
	surface = c_style_surface;
}

void Engine::make_device() {

	physicalDevice = vkInit::choose_physical_device(instance, debugMode);
	device = vkInit::create_logical_device(physicalDevice,surface, debugMode);
	std::array<vk::Queue, 2> queues = vkInit::get_queues(physicalDevice, device, surface, debugMode);
	graphicsQueue = queues[0];
	presentQueue = queues[1];
	make_swapchain();
	frameNumber = 0;
}

void Engine::make_swapchain() {

	vkInit::SwapChainBundle bundle = vkInit::create_swapchain(device, physicalDevice, surface, width, height, debugMode);
	swapchain = bundle.swapchain;
	swapchainFrames = bundle.frames;
	swapchainFormat = bundle.format;
	swapchainExtent = bundle.extent;
	maxFramesInFlight = static_cast<int>(swapchainFrames.size());

}

void Engine::recreate_swapchain() {

	width = 0;
	height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	device.waitIdle();

	cleanup_swapchain();
	make_swapchain();
	make_framebuffers();
	make_frame_resources();

	vkInit::commandBufferInputChunk commandbufferInput = { device, commandPool, swapchainFrames };
	vkInit::make_frame_command_buffers(commandbufferInput, debugMode);

}

void Engine::make_descriptor_set_layout() {

	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 2;

	bindings.indices.push_back(0);
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eVertex);

	bindings.indices.push_back(1);
	bindings.types.push_back(vk::DescriptorType::eStorageBuffer);
	bindings.counts.push_back(1);
	bindings.stages.push_back(vk::ShaderStageFlagBits::eVertex);

	descriptorSetLayout = vkInit::make_descriptor_set_layout(device, bindings);
}

void Engine::make_pipeline() {

	vkInit::GraphicsPipeLineInBundle specification = {};
	specification.device = device;
	specification.vertexFilepath = "shaders/vertex.spv";
	specification.fragmentFilepath = "shaders/fragment.spv";
	specification.swapchainExtent = swapchainExtent;
	specification.swapchainImageFormat = swapchainFormat;
	specification.descriptorSetLayout = descriptorSetLayout;


	vkInit::GraphicsPipeLineOutBundle output = vkInit::create_graphics_pipeline(specification, debugMode);
	layout = output.layout;
	renderpass = output.renderpass;
	pipelineTriangle = output.pipelineTriangle;
	pipelineLine = output.pipelineLine;
}

void Engine::make_framebuffers() {

	vkInit::framebufferInput framebufferInput;
	framebufferInput.device = device;
	framebufferInput.renderpass = renderpass;
	framebufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(framebufferInput, swapchainFrames, debugMode);

}

void Engine::make_frame_resources() {

	vkInit::descriptorSetLayoutData bindings;
	bindings.count = 1;
	bindings.types.push_back(vk::DescriptorType::eUniformBuffer);
	descriptorPool = vkInit::make_descriptor_pool(device, static_cast<uint32_t>(swapchainFrames.size()), bindings);

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {

		frame.inFlight = vkInit::make_fence(device, debugMode);
		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);

		frame.make_descriptor_resources(device, physicalDevice);

		frame.descriptorSet = vkInit::allocate_descriptor_set(device, descriptorPool, descriptorSetLayout);
	}
}

void Engine::finalize_setup() {

	make_framebuffers();

	commandPool = vkInit::make_command_pool(device, physicalDevice, surface, debugMode);

	vkInit::commandBufferInputChunk commandbufferInput = { device, commandPool, swapchainFrames };
	mainCommandBuffer = vkInit::make_command_buffer(commandbufferInput, debugMode);
	vkInit::make_frame_command_buffers(commandbufferInput, debugMode);

	make_frame_resources();

}

void Engine::make_assets() {

	meshes = new VertexMenagerie();

	std::vector<float> vertices = { {
		0.00f, 0.00f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.00f, -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,
	    0.00f, 0.05f, -0.05f, 0.0f, 0.0f, 1.0f
} };
	meshTypes type = meshTypes::TRIANGLE;
	meshes->consume(type, vertices);

	/*vertices = {{
		-0.05f,  0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.05f,  0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.05f,  0.05f, 0.0f, 1.0f, 0.0f, 0.0f
	} };
	type = meshTypes::SQUARE;
	meshes->consume(type, vertices);*/
	
	vertices.clear();
	for (location::Location location : graph) {
		float x1, y1, z1;
		x1 = static_cast<float>(location.get_x() / 10000);
		y1 = static_cast<float>(location.get_y() / 10000);
		z1 = static_cast<float>(location.get_z() / 100);
		for (connection::Connection connection : location.get_list()) {
			float x2, y2, z2;
			x2 = static_cast<float>(location.get_x() / 10000);
			y2 = static_cast<float>(location.get_y() / 10000);
			z2 = static_cast<float>(location.get_z() / 100);
			vertices.push_back(x1);
			vertices.push_back(y1);
			vertices.push_back(z1);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(x2);
			vertices.push_back(y2);
			vertices.push_back(z2);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}
	/*
	vertices = { {
	0.00f, 0.00f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.05f, -0.05f, 0.00f, 0.0f, 0.0f, 1.0f,
	0.00f, 0.00f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.05f, 0.05f, 0.00f, 0.0f, 0.0f, 1.0f,
} };*/
	type = meshTypes::LINE;
	meshes->consume(type, vertices);

	FinalizationChunk finalizationChunk;
	finalizationChunk.logicalDevice = device;
	finalizationChunk.physicalDevice = physicalDevice;
	finalizationChunk.queue = graphicsQueue;
	finalizationChunk.commandBuffer = mainCommandBuffer;
	meshes->finalize(finalizationChunk);
}

void Engine::prepare_frame(uint32_t imageIndex, Scene* scene) {

	vkUtil::SwapChainFrame& _frame = swapchainFrames[imageIndex];

	glm::vec3 eye = {-0.8f,-0.8f, -1.0f };
	glm::vec3 center = { 1.0f, 1.0f, 0.0f };
	glm::vec3 up = { 0.0f, 0.0f, -1.0f };
	glm::mat4 view = glm::lookAt(eye, center, up);

	glm::mat4 projection = glm::perspective(
		glm::radians(50.0f),
		static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height),
		0.1f, 10.0f);
	projection[1][1] *= -1;
	
	_frame.cameraData.view = view;
	_frame.cameraData.projection = projection;
	_frame.cameraData.viewProjection = projection * view;
	
	memcpy(
		_frame.cameraDataWriteLocation,
		&(_frame.cameraData),
		sizeof(vkUtil::UBO)
	);

	size_t i = 0;
	for (glm::vec3& position : scene->trianglePosition) {
		_frame.modelTransforms[i++] = glm::translate(glm::mat4(1.0f), position);
	}

	for (glm::vec3& position : scene->linePosition) {
		_frame.modelTransforms[i++] = glm::translate(glm::mat4(1.0f), position);
	}
	memcpy(_frame.modelBufferWriteLocation, _frame.modelTransforms.data(), i * sizeof(glm::mat4));

	_frame.write_descriptor_set(device);
}

void Engine::prepare_scene(vk::CommandBuffer commandBuffer) {

	vk::Buffer vertexBuffer[] = { meshes->vertexBuffer.buffer };
	vk::DeviceSize offsets[] = { 0 };
	commandBuffer.bindVertexBuffers(0, 1, vertexBuffer, offsets);

}

void Engine::record_draw_commands(vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene) {

	vk::CommandBufferBeginInfo beginInfo = {};

	try {
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to begin recording command buffer" << std::endl;
		}
	}

	vk::RenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.renderPass = renderpass;
	renderPassInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
	renderPassInfo.renderArea.offset.x = 0;
	renderPassInfo.renderArea.offset.y = 0;
	renderPassInfo.renderArea.extent = swapchainExtent;

	vk::ClearValue clearColor = { BACKGROUND_COLOR };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 0, swapchainFrames[imageIndex].descriptorSet, nullptr);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineTriangle);

	prepare_scene(commandBuffer);
	int firstVertex = meshes->offsets.find(meshTypes::TRIANGLE)->second;
	int vertextCount = meshes->sizes.find(meshTypes::TRIANGLE)->second;
	uint32_t startInstance = 0;
	uint32_t instanceCount = static_cast<uint32_t>(scene->trianglePosition.size());
	std::cout << "O-O> " << firstVertex << " " << vertextCount << " " << instanceCount << std::endl;
	commandBuffer.draw(vertextCount, instanceCount, firstVertex, startInstance);
	startInstance += instanceCount;

	/*firstVertex = meshes->offsets.find(meshTypes::SQUARE)->second;
	vertextCount = meshes->sizes.find(meshTypes::SQUARE)->second;
	instanceCount = static_cast<uint32_t>(scene->squarePosition.size());
	commandBuffer.draw(vertextCount, instanceCount, firstVertex, startInstance);
	startInstance += instanceCount;*/
	
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineLine);
	
	firstVertex = meshes->offsets.find(meshTypes::LINE)->second;
	vertextCount = meshes->sizes.find(meshTypes::LINE)->second;
	instanceCount = static_cast<uint32_t>(scene->linePosition.size());
	std::cout << "0-0> " << firstVertex << " " << vertextCount << " " << instanceCount << " " << startInstance << std::endl;
	commandBuffer.draw(vertextCount, instanceCount, firstVertex, startInstance);
	startInstance += instanceCount;


	commandBuffer.endRenderPass();

	try {
		commandBuffer.end();
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to finish recording command buffer" << std::endl;
		}
	}
}

void Engine::render(Scene* scene) {

	device.waitForFences(1, &swapchainFrames[frameNumber].inFlight, VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	try {
		vk::ResultValue acquire = device.acquireNextImageKHR(
			swapchain, UINT64_MAX, swapchainFrames[frameNumber].imageAvailable, nullptr
		);
		 imageIndex = acquire.value;
	}
	catch (vk::OutOfDateKHRError) {
		std::cout << "Recreate" << std::endl;
		recreate_swapchain();
		return;
	}


	vk::CommandBuffer commandBuffer = swapchainFrames[frameNumber].commandBuffer;

	commandBuffer.reset();

	prepare_frame(imageIndex, scene);

	record_draw_commands(commandBuffer, imageIndex, scene);

	vk::SubmitInfo submitInfo = {};
	vk::Semaphore waitSemaphore[] = { swapchainFrames[frameNumber].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	vk::Semaphore signalSemaphores[] = { swapchainFrames[frameNumber].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	device.resetFences(1, &(swapchainFrames[frameNumber].inFlight));
	try {
		graphicsQueue.submit(submitInfo, swapchainFrames[frameNumber].inFlight);
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Failed to submit draw command buffer." << std::endl;
		}
	}

	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	vk::SwapchainKHR swapchains[] = { swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;

	vk::Result present;
	try {
		present = presentQueue.presentKHR(presentInfo);
	}
	catch (vk::OutOfDateKHRError) {
		present = vk::Result::eErrorOutOfDateKHR;
	}
	if (present == vk::Result::eErrorOutOfDateKHR || present == vk::Result::eSuboptimalKHR) {

	}
	
	frameNumber = (frameNumber + 1) % maxFramesInFlight;

}

void Engine::cleanup_swapchain() {

	for (vkUtil::SwapChainFrame frame : swapchainFrames) {
		device.destroyImageView(frame.imageView);
		device.destroyFramebuffer(frame.framebuffer);
		device.destroyFence(frame.inFlight);
		device.destroySemaphore(frame.imageAvailable);
		device.destroySemaphore(frame.renderFinished);

		device.unmapMemory(frame.cameraDataBuffer.bufferMemory);
		device.freeMemory(frame.cameraDataBuffer.bufferMemory);
		device.destroyBuffer(frame.cameraDataBuffer.buffer);

		device.unmapMemory(frame.modelBuffer.bufferMemory);
		device.freeMemory(frame.modelBuffer.bufferMemory);
		device.destroyBuffer(frame.modelBuffer.buffer);
	}
	device.destroySwapchainKHR(swapchain);

	device.destroyDescriptorPool(descriptorPool);
}

Engine::~Engine() {

	device.waitIdle();

	if (debugMode) {
		std::cout << "Goodbye see you!\n";
	}
	

	device.destroyCommandPool(commandPool);

	device.destroyPipeline(pipelineTriangle);
	device.destroyPipeline(pipelineLine);
	device.destroyPipelineLayout(layout);
	device.destroyRenderPass(renderpass);

	cleanup_swapchain();

	device.destroyDescriptorSetLayout(descriptorSetLayout);

	delete meshes;
	
	device.destroy();
	instance.destroySurfaceKHR(surface);
	if (debugMode) {
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}
	/*
	* from vulkan_funcs.hpp:
	*
	* void Instance::destroy( Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr,
											Dispatch const & d = ::vk::getDispatchLoaderStatic())
	*/
	instance.destroy();

	//terminate glfw
	glfwTerminate();
}

