//================ Copyright (c) 2016, PG, All rights reserved. =================//
//
// Purpose:		vulkan wrapper
//
// $NoKeywords: $vk
//===============================================================================//

#include "VulkanInterface.h"
#include "Engine.h"

#include <string.h>

VulkanInterface *vulkan = NULL;

VulkanInterface::VulkanInterface()
{
	vulkan = this;
	m_bReady = false;

#ifdef MCENGINE_FEATURE_VULKAN

	// OS dependent required extensions for anything to work at all
	std::vector<const char *> enabledExtensions;
	enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__CYGWIN__) || defined(__CYGWIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
		enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
	#error "TODO: add correct extension here"
		enabledExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

	// application settings
	VkApplicationInfo appInfo = VkApplicationInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = "Renderer";
	appInfo.pEngineName = "McEngine";
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 4);

	// instance settings
	VkInstanceCreateInfo createInfo = VkInstanceCreateInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = enabledExtensions.size();
	createInfo.ppEnabledExtensionNames = &enabledExtensions[0];

	// create instance
	VkResult res = vkCreateInstance(&createInfo, NULL, &m_instance);

	if (res != VK_SUCCESS)
	{
		if (res == VK_ERROR_EXTENSION_NOT_PRESENT)
			engine->showMessageError("Vulkan Error", UString::format("Couldn't vkCreateInstance(), returned %d. Required Vulkan extensions are not present!", res));
		else
			engine->showMessageError("Vulkan Error", UString::format("Couldn't vkCreateInstance(), returned %d", res));
		return;
	}

	// iterate over all available devices
	unsigned int numDevices = 0;
	res = vkEnumeratePhysicalDevices(m_instance, &numDevices, NULL);

	if (res != VK_SUCCESS)
	{
		engine->showMessageError("Vulkan Error", UString::format("Couldn't vkEnumeratePhysicalDevices(), returned %d", res));
		return;
	}

	if (numDevices < 1)
	{
		engine->showMessageError("Vulkan Error", "Couldn't detect any Vulkan compatible devices!");
		return;
	}

	debugLog("Vulkan: Found %i compatible device(s)\n", numDevices);
	std::vector<VkPhysicalDevice> devices(numDevices);
	res = vkEnumeratePhysicalDevices(m_instance, &numDevices, &devices[0]);

	if (res != VK_SUCCESS)
	{
		engine->showMessageError("Vulkan Error", UString::format("Couldn't enumerate vkEnumeratePhysicalDevices(), returned %d", res));
		return;
	}

	// Enumerate over device properties. Output information about each device
	// just for logging purposes. The API version shifts at the end are from
	// the Vulkan header file (VK_VER_MAJOR, VK_VER_MINOR, VK_VER_PATCH).
	VkPhysicalDeviceProperties deviceProperties;
	for (unsigned int i=0; i<numDevices; i++)
	{
		memset(&deviceProperties, 0, sizeof deviceProperties);
		vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
		debugLog("Vulkan Device #%i: Driver Version: %d\n", i, deviceProperties.driverVersion);
		debugLog("Vulkan Device #%i: Device Name:    %s\n", i, deviceProperties.deviceName);
		debugLog("Vulkan Device #%i: Device Type:    %d\n", i, deviceProperties.deviceType);
		debugLog("Vulkan Device #%i: API Version:    %d.%d.%d\n", i,
		(uint32_t)deviceProperties.apiVersion >> 22,
		((uint32_t)deviceProperties.apiVersion >> 12) & 0x3ff,
		(uint32_t)deviceProperties.apiVersion & 0xfff);
	}

	debugLog("Vulkan: Selecting device #%i as default device.\n", 0);
	m_physicalDevice = devices[0];

	// create device
	VkDeviceCreateInfo deviceInfo;

	// mandatory fields
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = NULL;
	deviceInfo.flags = 0;

	// we won't bother with extensions or layers
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = NULL;
	deviceInfo.enabledExtensionCount = 0;
	deviceInfo.ppEnabledExtensionNames = NULL;

	// we don't want any any features,:the wording in the spec for DeviceCreateInfo
	// excludes that you can pass NULL to disable features, which GetPhysicalDeviceFeatures
	// tells you is a valid value. This must be supplied - NULL is legal here.
	deviceInfo.pEnabledFeatures = NULL;

	// here's where we initialize our queues
	VkDeviceQueueCreateInfo deviceQueueInfo;
	deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueInfo.pNext = NULL;
	deviceQueueInfo.flags = 0;
	// use the first queue family in the family list
	deviceQueueInfo.queueFamilyIndex = 0;

	// create only one queue
	float queuePriorities[] = { 1.0f };
	deviceQueueInfo.queueCount = 1;
	deviceQueueInfo.pQueuePriorities = queuePriorities;
	// set queue(s) into the device
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &deviceQueueInfo;

	res = vkCreateDevice(m_physicalDevice, &deviceInfo, NULL, &m_device);
	if (res != VK_SUCCESS)
	{
		engine->showMessageError("Vulkan Error", UString::format("Couldn't vkCreateDevice(), returned %d", res));
		return;
	}

	m_bReady = true;

#endif
}

VulkanInterface::~VulkanInterface()
{
#ifdef MCENGINE_FEATURE_VULKAN

	if (m_bReady)
	{
		vkDestroyDevice(m_device, NULL);
		vkDestroyInstance(m_instance, NULL);
	}

#endif

	m_bReady = false;
	vulkan = NULL; // deref
}

void VulkanInterface::finish()
{
#ifdef MCENGINE_FEATURE_VULKAN

	vkDeviceWaitIdle(m_device);

#endif
}
