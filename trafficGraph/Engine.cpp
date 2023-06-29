#include "engine.h"
#include "instance.h"
#include "logging.h"
#include"device.h"
#include"swapchain.h"
#include"pipeline.h"
#include"framebuffer.h"
#include "commands.h"
#include "sync.h"




Engine::Engine(int width, int height, GLFWwindow* window, bool debugMode ) {

	this->width = width;
	this->height = height;
	this->window = window;
	this->debugMode = debugMode;

	if (debugMode) {
		std::cout << "Making a graphics engine\n";
	}


	make_instance();

	make_device();

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
	make_frame_sync_objects();

	vkInit::commandBufferInputChunk commandbufferInput = { device, commandPool, swapchainFrames };
	vkInit::make_frame_command_buffers(commandbufferInput, debugMode);

}

void Engine::make_pipeline() {

	vkInit::GraphicsPipeLineInBundle specification = {};
	specification.device = device;
	specification.vertexFilepath = "shaders/vertex.spv";
	specification.fragmentFilepath = "shaders/fragment.spv";
	specification.swapchainExtent = swapchainExtent;
	specification.swapchainImageFormat = swapchainFormat;


	vkInit::GraphicsPipeLineOutBundle output = vkInit::make_graphics_pipeline(specification, debugMode);
	layout = output.layout;
	renderpass = output.renderpass;
	pipeline = output.pipeline;
}

void Engine::make_framebuffers() {

	vkInit::framebufferInput framebufferInput;
	framebufferInput.device = device;
	framebufferInput.renderpass = renderpass;
	framebufferInput.swapchainExtent = swapchainExtent;
	vkInit::make_framebuffers(framebufferInput, swapchainFrames, debugMode);

}

void Engine::make_frame_sync_objects() {

	for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
		frame.inFlight = vkInit::make_fence(device, debugMode);
		frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
		frame.renderFinished = vkInit::make_semaphore(device, debugMode);
	}
}

void Engine::finalize_setup() {

	make_framebuffers();

	commandPool = vkInit::make_command_pool(device, physicalDevice, surface, debugMode);

	vkInit::commandBufferInputChunk commandbufferInput = { device, commandPool, swapchainFrames };
	mainCommandBuffer = vkInit::make_command_buffer(commandbufferInput, debugMode);
	vkInit::make_frame_command_buffers(commandbufferInput, debugMode);

	make_frame_sync_objects();

}

void Engine::make_assets() {

	triangleMesh = new TriangleMesh(device, physicalDevice);
}

void Engine::prepare_scene(vk::CommandBuffer commandBuffer) {

	vk::Buffer vertexBuffer[] = { triangleMesh->vertexBuffer.buffer };
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
	vk::ClearValue clearColor = { std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

	prepare_scene(commandBuffer);

	for (glm::vec3 position : scene->trianglePosition) {

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
		vkUtil::ObjectData objectData;
		objectData.model = model;
		commandBuffer.pushConstants(layout, vk::ShaderStageFlagBits::eVertex, 0 , sizeof(objectData), & objectData);
		commandBuffer.draw(3, 1, 0, 0);
	}

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
	}

	device.destroySwapchainKHR(swapchain);
}

Engine::~Engine() {

	device.waitIdle();

	if (debugMode) {
		std::cout << "Goodbye see you!\n";
	}
	

	device.destroyCommandPool(commandPool);

	device.destroyPipeline(pipeline);
	device.destroyPipelineLayout(layout);
	device.destroyRenderPass(renderpass);

	cleanup_swapchain();

	delete triangleMesh;
	
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

