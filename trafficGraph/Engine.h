#pragma once

#include"config.h"
#include"frame.h"
#include"scene.h"
#include"vertex_menagerie.h"
#include"Location.h"


class Engine
{

	public:

		Engine(int width, int height, GLFWwindow* window, std::list<location::Location> graph, bool debugMode);

		~Engine();

		void render(Scene* scene);

	private:

		//whether to print debug messages in functions
		bool debugMode;

		//glfw window parameters
		int width;
		int height;
		GLFWwindow* window;

		//graph
		std::list<location::Location> graph;

		//vulkan instance -related variable
		vk::Instance instance{ nullptr };
		vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
		vk::DispatchLoaderDynamic dldi;
		vk::SurfaceKHR surface;

		vk::PhysicalDevice physicalDevice{nullptr};
		vk::Device device{nullptr};
		vk::Queue graphicsQueue{ nullptr };
		vk::Queue presentQueue{ nullptr };
		vk::SwapchainKHR swapchain;
		std::vector<vkUtil::SwapChainFrame> swapchainFrames;
		vk::Format swapchainFormat;
		vk::Extent2D swapchainExtent;

		//pipeline variables
		vk::PipelineLayout layout;
		vk::RenderPass renderpass;
		vk::Pipeline pipelineTriangle;
		vk::Pipeline pipelineLine;

		//command variables
		vk::CommandPool commandPool;
		vk::CommandBuffer mainCommandBuffer;

		//synchonization variables
		int maxFramesInFlight, frameNumber;

		//Descriptor objects
		vk::DescriptorSetLayout descriptorSetLayout;
		vk::DescriptorPool descriptorPool;

		//asset pointers
		VertexMenagerie* meshes;

		//glfw setup
		void build_glfw_window();

		//instance setup
		void make_instance();

		void make_device();
		void make_swapchain();
		void recreate_swapchain();

		void make_descriptor_set_layout();
		void make_pipeline();

		void finalize_setup();
		void make_framebuffers();
		void make_frame_resources();

		void make_assets();
		void prepare_frame(uint32_t imageIndex, Scene* scene);
		void prepare_scene(vk::CommandBuffer commandBuffer);

		void record_draw_commands( vk::CommandBuffer commandBuffer, uint32_t imageIndex, Scene* scene);

		void cleanup_swapchain();
	};




