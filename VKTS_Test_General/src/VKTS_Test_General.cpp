#include <vkts/vkts.hpp>

#define VKTS_TEST_NAME "Test"

class Test: public vkts::IUpdateThread
{

private:

	std::string name;

	int32_t displayIndex;

	int32_t windowIndex;

public:

	Test(const std::string& name) :
			IUpdateThread(), name(name), displayIndex(-1), windowIndex(-1)
	{
	}

	virtual ~Test()
	{
	}

	virtual VkBool32 init(const vkts::IUpdateThreadContext& updateContext)
	{
		VkResult result;

		//

		vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Initialize done.",
				name.c_str());
		vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Thread %d from %d.",
				name.c_str(), updateContext.getThreadIndex(),
				updateContext.getThreadCount());

		const auto displayIndexWalker =
				updateContext.getAttachedDisplayIndices().begin();
		const auto windowIndexWalker =
				updateContext.getAttachedWindowIndices().begin();

		if (displayIndexWalker
				!= updateContext.getAttachedDisplayIndices().end()
				&& windowIndexWalker
						!= updateContext.getAttachedWindowIndices().end())
		{
			displayIndex = *displayIndexWalker;
			windowIndex = *windowIndexWalker;

			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Found display index = %d",
					name.c_str(), displayIndex);
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Width x Height = %d x %d",
					name.c_str(),
					updateContext.getDisplayDimension(displayIndex).x,
					updateContext.getDisplayDimension(displayIndex).y);
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Found window index = %d",
					name.c_str(), windowIndex);
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Width x Height = %d x %d",
					name.c_str(),
					updateContext.getWindowDimension(windowIndex).x,
					updateContext.getWindowDimension(windowIndex).y);

			//

			if (!vkts::wsiGatherNeededInstanceExtensions())
			{
				vkts::logPrint(
				VKTS_LOG_WARNING,
						"Test: Could not gather instance extensions. Skipping scene loading.");

				return VK_TRUE;
			}

			auto instance = vkts::instanceCreate(VKTS_TEST_NAME,
					VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION, 0, 0, nullptr,
					vkts::wsiGetNeededInstanceExtensionCount(),
					vkts::wsiGetNeededInstanceExtensionNames());

			if (!instance.get())
			{
				vkts::logPrint(
				VKTS_LOG_WARNING,
						"Test: Could not create instance. Skipping scene loading.");

				return VK_TRUE;
			}

			if (!vkts::wsiInitInstanceExtensions(instance->getInstance()))
			{
				vkts::logPrint(
				VKTS_LOG_ERROR,
						"Test: Could not initialize instance extension.");

				return VK_FALSE;
			}

			//

			auto physicalDevice = vkts::physicalDeviceCreate(
					instance->getInstance(), 0);
			if (!physicalDevice.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not get physical device.");

				return VK_FALSE;
			}

			if (!vkts::wsiGatherNeededDeviceExtensions(physicalDevice->getPhysicalDevice()))
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not gather device extension.");

				return VK_FALSE;
			}

			//

			float queuePriorities[1] = {0.0f};

			VkDeviceQueueCreateInfo deviceQueueCreateInfo;

			memset(&deviceQueueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));

			deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

			deviceQueueCreateInfo.flags = 0;
			deviceQueueCreateInfo.queueFamilyIndex = 0;
			deviceQueueCreateInfo.queueCount = 1;
			deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

			auto device = vkts::deviceCreate(physicalDevice->getPhysicalDevice(), 0, 1, &deviceQueueCreateInfo, 0, nullptr, vkts::wsiGetNeededDeviceExtensionCount(), vkts::wsiGetNeededDeviceExtensionNames(), nullptr);

			if (!device.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create device.");

				return VK_FALSE;
			}

			if (!vkts::wsiInitDeviceExtensions(device->getDevice()))
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not initialize device extension.");

				return VK_FALSE;
			}

			//

			auto surface = vkts::wsiSurfaceCreate(instance->getInstance(), updateContext.getNativeDisplay(displayIndex), updateContext.getNativeWindow(windowIndex));

			if (!surface.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create surface.");

				return VK_FALSE;
			}

			//

			std::vector<VkBool32> supportFilter;

			result = vkts::wsiGetPhysicalDeviceSurfaceSupport(physicalDevice->getPhysicalDevice(), surface->getSurface(), (uint32_t) physicalDevice->getAllQueueFamilyProperties().size(), supportFilter);

			if (result != VK_SUCCESS || supportFilter.size() == 0)
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not get physical device surface support.");

				return VK_FALSE;
			}

			//

			uint32_t queueFamilyIndex;

			if (!vkts::queueGetFamilyIndex(physicalDevice->getAllQueueFamilyProperties(), VK_QUEUE_GRAPHICS_BIT, 0, &supportFilter, queueFamilyIndex))
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not find queue family index.");

				return VK_FALSE;
			}

			auto queue = vkts::queueGet(device->getDevice(), queueFamilyIndex,
					0);

			if (!queue.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not get device queue.");

				return VK_FALSE;
			}

			//

			auto initialResources = vkts::initialResourcesCreate(instance,
					physicalDevice, device, queue);

			if (!initialResources.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not get create initial resources.");

				return VK_FALSE;
			}

			//

			auto commandPool = vkts::commandPoolCreate(device->getDevice(), queue->getQueueFamilyIndex(), 0);

			if (!commandPool.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not get command pool.");

				return VK_FALSE;
			}

			//

			VkSamplerCreateInfo samplerCreateInfo;

			memset(&samplerCreateInfo, 0, sizeof(VkSamplerCreateInfo));

			samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

			samplerCreateInfo.flags = 0;
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerCreateInfo.mipLodBias = 0.0f;
			samplerCreateInfo.maxAnisotropy = 1.0f;
			samplerCreateInfo.compareEnable = VK_FALSE;
			samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerCreateInfo.minLod = 0.0f;
			samplerCreateInfo.maxLod = 0.0f;
			samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

			VkImageViewCreateInfo imageViewCreateInfo;

			memset(&imageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = VK_NULL_HANDLE;	// Defined later.
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = VK_FORMAT_UNDEFINED;
			imageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
			imageViewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

			//

			VkDescriptorSetLayoutBinding descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BINDING_COUNT];

			memset(&descriptorSetLayoutBinding, 0, sizeof(descriptorSetLayoutBinding));

			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].binding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].descriptorCount = 1;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].pImmutableSamplers = nullptr;

			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].binding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].descriptorCount = 1;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].pImmutableSamplers = nullptr;

			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].binding = VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].descriptorCount = 1;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].pImmutableSamplers = nullptr;

            for (int32_t i = VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE; i < VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM; i++)
            {
    			descriptorSetLayoutBinding[i].binding = i;
    			descriptorSetLayoutBinding[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    			descriptorSetLayoutBinding[i].descriptorCount = 1;
    			descriptorSetLayoutBinding[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    			descriptorSetLayoutBinding[i].pImmutableSamplers = nullptr;
            }

            descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM].binding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM].descriptorCount = 1;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM].pImmutableSamplers = nullptr;

			auto descriptorSetLayout = vkts::descriptorSetLayoutCreate(device->getDevice(), 0, VKTS_BINDING_UNIFORM_BINDING_COUNT, descriptorSetLayoutBinding);

			if (!descriptorSetLayout.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create descriptor set layout.");

				return VK_FALSE;
			}

			//
			// Command buffer to allow resource update.
			//

			auto cmdBuffer = vkts::commandBuffersCreate(device->getDevice(),
					commandPool->getCmdPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY,
					1);

			if (!cmdBuffer.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not create command buffer.");

				return VK_FALSE;
			}

			result = cmdBuffer->beginCommandBuffer(0,
			VK_NULL_HANDLE, 0,
			VK_NULL_HANDLE,
			VK_FALSE, 0, 0);

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not begin command buffer.");

				return VK_FALSE;
			}

			//
			// First test scene.
			//

			auto context = vkts::scenegraphCreateContext(VK_FALSE,
					initialResources, cmdBuffer, samplerCreateInfo,
					imageViewCreateInfo, descriptorSetLayout);

			if (!context.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test '%s': Could not create context", name.c_str());

				return VK_FALSE;
			}

			auto scene = vkts::scenegraphLoadScene("test01.vkts", context);

			if (!scene.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test '%s': Could not load scene", name.c_str());

				return VK_FALSE;
			}

			//
			// Commit.
			//

			result = cmdBuffer->endCommandBuffer();

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not end command buffer.");

				return VK_FALSE;
			}

			VkSubmitInfo submitInfo;

			memset(&submitInfo, 0, sizeof(VkSubmitInfo));

			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.commandBufferCount = cmdBuffer->getCommandBufferCount();
			submitInfo.pCommandBuffers = cmdBuffer->getCommandBuffers();
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;

			result = queue->submit(1, &submitInfo, VK_NULL_HANDLE);

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not submit queue.");

				return VK_FALSE;
			}

			result = queue->waitIdle();

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not wait for idle queue.");

				return VK_FALSE;
			}

			//

			context->destroy();

			scene->destroy();

			//
			// Reuse command buffer again.
			//

			result = cmdBuffer->beginCommandBuffer(0,
			VK_NULL_HANDLE, 0,
			VK_NULL_HANDLE,
			VK_FALSE, 0, 0);

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not begin command buffer.");

				return VK_FALSE;
			}

			//
			// Second test scene.
			//

			auto anotherContext = vkts::scenegraphCreateContext(
			VK_FALSE, initialResources, cmdBuffer, samplerCreateInfo,
					imageViewCreateInfo, descriptorSetLayout);

			if (!anotherContext.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test '%s': Could not create another context",
						name.c_str());

				return VK_FALSE;
			}

			auto anotherScene = vkts::scenegraphLoadScene(
					"test_scene/test_scene.vkts", anotherContext);

			if (!anotherScene.get())
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test '%s': Could not load another scene",
						name.c_str());

				return VK_FALSE;
			}

			//
			// Commit.
			//

			result = cmdBuffer->endCommandBuffer();

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not end command buffer.");

				return VK_FALSE;
			}

			memset(&submitInfo, 0, sizeof(VkSubmitInfo));

			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.commandBufferCount = cmdBuffer->getCommandBufferCount();
			submitInfo.pCommandBuffers = cmdBuffer->getCommandBuffers();
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;

			result = queue->submit(1, &submitInfo, VK_NULL_HANDLE);

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not submit queue.");

				return VK_FALSE;
			}

			result = queue->waitIdle();

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR,
						"Test: Could not wait for idle queue.");

				return VK_FALSE;
			}

			//

			anotherContext->destroy();

			anotherScene->destroy();

			//

            cmdBuffer->destroy();

            descriptorSetLayout->destroy();

			commandPool->destroy();

			surface->destroy();

			device->destroy();

			instance->destroy();
		}

		return VK_TRUE;
	}

	virtual VkBool32 update(const vkts::IUpdateThreadContext& updateContext)
	{
		if (windowIndex >= 0)
		{
			vkts::logPrint(VKTS_LOG_DEBUG,
					"Test '%s': Width x Height = %d x %d", name.c_str(),
					updateContext.getWindowDimension(windowIndex).x,
					updateContext.getWindowDimension(windowIndex).y);

			for (int32_t currentGamepad = 0; currentGamepad < VKTS_MAX_GAMEPADS;
					currentGamepad++)
			{
				for (int32_t currentButtonIndex = 0;
						currentButtonIndex < VKTS_MAX_GAMEPAD_BUTTONS;
						currentButtonIndex++)
				{
					if (updateContext.getGamepadButton(windowIndex,
							currentGamepad, currentButtonIndex))
					{
						vkts::logPrint(
						VKTS_LOG_INFO,
								"Test '%s': Gamepad '%d' button '%d' pressed!",
								name.c_str(), currentGamepad,
								currentButtonIndex);
					}
				}
			}

			if (updateContext.getKey(windowIndex, VKTS_KEY_Q))
			{
				vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Quit pressed!",
						name.c_str());

				return VK_FALSE;
			}
		}

		vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': Updating %f", name.c_str(),
				updateContext.getTotalTime());

		vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': Before barrier.",
				name.c_str());

		if (!vkts::barrierSync())
		{
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Barrier killed.",
					name.c_str());

			return VK_FALSE;
		}

		vkts::logPrint(VKTS_LOG_DEBUG, "Test '%s': After barrier.",
				name.c_str());

		if (name == "a" && updateContext.getTotalTime() > 10.0)
		{
			vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Time reached!",
					name.c_str());

			return VK_FALSE;
		}

		return VK_TRUE;
	}

	virtual void terminate(const vkts::IUpdateThreadContext& updateContext)
	{
		vkts::logPrint(VKTS_LOG_INFO, "Test '%s': Terminate done.",
				name.c_str());
	}

};

int main()
{
	vkts::IUpdateThreadSP a = vkts::IUpdateThreadSP(new Test("a"));
	vkts::IUpdateThreadSP b = vkts::IUpdateThreadSP(new Test("b"));
	vkts::IUpdateThreadSP c = vkts::IUpdateThreadSP(new Test("c"));

	if (!a.get() || !b.get() || !b.get())
	{
		printf("Test: Could not create test application.");

		return -1;
	}

	vkts::engineInit();

	//vkts::logSetLevel(VKTS_LOG_NOTHING);
	//vkts::logSetLevel(VKTS_LOG_DEBUG);
	//vkts::logSetLevel(VKTS_LOG_SEVERE);

	vkts::logPrint(VKTS_LOG_INFO, "Test: Number of processors = %u.",
			vkts::processorGetNumber());

	//
	// Visual setup.
	//

	if (!vkts::visualInit())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not initialize visual.");

		vkts::engineTerminate();

		return -1;
	}

	vkts::logPrint(VKTS_LOG_INFO, "Test: Number of displays = %d",
			vkts::visualGetNumberDisplays());

	auto display = vkts::visualCreateDefaultDisplay().lock();

	if (!display.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create display.");

		vkts::visualTerminate();

		vkts::engineTerminate();

		return -1;
	}

	vkts::logPrint(VKTS_LOG_INFO, "Test: Display = %d Width x Height = %d x %d",
			display->getIndex(), display->getWidth(), display->getHeight());

	auto window = vkts::visualCreateWindow(display, "Test", 1024, 768,
	VK_FALSE,
	VK_TRUE,
	VK_FALSE).lock();

	if (!window.get())
	{
		window = vkts::visualGetWindow(VKTS_DEFAULT_WINDOW_INDEX).lock();

		if (!window.get())
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Test: Could not create window.");

			display->destroy();

			vkts::visualTerminate();

			vkts::engineTerminate();

			return -1;
		}
	}

	//
	// Engine setup.
	//

	vkts::engineAddUpdateThread(a);
	vkts::engineAddUpdateThread(b);
	vkts::engineAddUpdateThread(c);

	//
	// File test.
	//

	auto textBuffer = vkts::fileLoadText("input.txt");

	if (textBuffer.get())
	{
		if (!vkts::fileSaveText("output.txt", textBuffer))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save file.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load file.");
	}

	auto createdTextBuffer = vkts::textBufferCreate("");

	if (createdTextBuffer.get())
	{
		createdTextBuffer->puts("Created text by VKTS.");
		createdTextBuffer->puts("\nAppended text.");

		if (!vkts::fileSaveText("created.txt", createdTextBuffer))
		{
			vkts::logPrint(VKTS_LOG_WARNING,
					"Test: Could not save created text file.");
		}

		createdTextBuffer->seek(0, VKTS_SEARCH_ABSOLUTE);

		char buffer[256];

		while (createdTextBuffer->gets(buffer, 256))
		{
			vkts::logPrint(VKTS_LOG_INFO, "Test: Created text line: '%s'",
					buffer);
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not create text.");
	}

	//
	// Image test.
	//

	auto imageTga = vkts::imageDataLoad("crate_input.tga");

	if (imageTga.get())
	{
		if (!vkts::imageDataSave("crate_output.tga", imageTga))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save TGA image.");
		}

		auto mipMaps = vkts::imageDataMipmap(imageTga,
		VK_TRUE, "crate_output.tga");

		for (size_t i = 0; i < mipMaps.size(); i++)
		{
			if (!vkts::imageDataSave(mipMaps[i]->getName().c_str(), mipMaps[i]))
			{
				vkts::logPrint(VKTS_LOG_WARNING,
						"Test: Could not save mip map TGA image.");
			}
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load TGA image.");
	}

	//

	auto imageHdr = vkts::imageDataLoad("CraterLake_input.hdr");

	if (imageHdr.get())
	{
		if (!vkts::imageDataSave("CraterLake_output.hdr", imageHdr))
		{
			vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not save HDR image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load HDR image.");
	}

	//

	auto createdTga = vkts::imageDataCreate("created.tga", 16, 16, 1, 1.0f,
			0.0f, 0.0f, 0.0f, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8_UNORM);

	if (createdTga.get())
	{
		if (!vkts::imageDataSave(createdTga->getName().c_str(), createdTga))
		{
			vkts::logPrint(VKTS_LOG_WARNING,
					"Test: Could not save created TGA image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not create TGA image.");
	}

	//

	auto createdHdr = vkts::imageDataCreate("created.hdr", 16, 16, 1, 1.0f,
			0.0f, 0.0f, 0.0f, VK_IMAGE_TYPE_2D, VK_FORMAT_R32G32B32_SFLOAT);

	if (createdHdr.get())
	{
		if (!vkts::imageDataSave(createdHdr->getName().c_str(), createdHdr))
		{
			vkts::logPrint(VKTS_LOG_WARNING,
					"Test: Could not save created HDR image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not create HDR image.");
	}

	//

	auto imageRGB = vkts::imageDataLoad("parque_rgb.tga");

	if (imageRGB.get())
	{
		auto imageRGBA = vkts::imageDataConvert(imageRGB,
				VK_FORMAT_R8G8B8A8_UNORM, "parque_rgba.tga");

		if (imageRGBA.get())
		{
			if (!vkts::imageDataSave("parque_rgba.tga", imageRGBA))
			{
				vkts::logPrint(VKTS_LOG_WARNING,
						"Test: Could not save converted RGBA image.");
			}
		}
		else
		{
			vkts::logPrint(VKTS_LOG_WARNING,
					"Test: Could not convert RGB image.");
		}
	}
	else
	{
		vkts::logPrint(VKTS_LOG_WARNING, "Test: Could not load RGB image.");
	}

	//
	// Execution.
	//

	vkts::engineRun();

	//
	// Termination.
	//

	window->destroy();

	display->destroy();

	vkts::visualTerminate();

	vkts::engineTerminate();

	return 0;
}
