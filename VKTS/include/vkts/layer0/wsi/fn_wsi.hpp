/**
 * VKTS - VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef VKTS_FN_WSI_HPP_
#define VKTS_FN_WSI_HPP_

#include <vkts/vkts.hpp>

// VK_KHR_surface

VKTS_APICALL PFN_vkDestroySurfaceKHR vktsDestroySurfaceKHR;
VKTS_APICALL PFN_vkGetPhysicalDeviceSurfaceSupportKHR vktsGetPhysicalDeviceSurfaceSupportKHR;
VKTS_APICALL PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vktsGetPhysicalDeviceSurfaceCapabilitiesKHR;
VKTS_APICALL PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vktsGetPhysicalDeviceSurfaceFormatsKHR;
VKTS_APICALL PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vktsGetPhysicalDeviceSurfacePresentModesKHR;

// VK_KHR_swapchain

VKTS_APICALL PFN_vkCreateSwapchainKHR vktsCreateSwapchainKHR;
VKTS_APICALL PFN_vkDestroySwapchainKHR vktsDestroySwapchainKHR;
VKTS_APICALL PFN_vkGetSwapchainImagesKHR vktsGetSwapchainImagesKHR;
VKTS_APICALL PFN_vkAcquireNextImageKHR vktsAcquireNextImageKHR;
VKTS_APICALL PFN_vkQueuePresentKHR vktsQueuePresentKHR;

namespace vkts
{

VKTS_APICALL VkBool32 VKTS_APIENTRY wsiGatherNeededInstanceExtensions();

VKTS_APICALL uint32_t VKTS_APIENTRY wsiGetNeededInstanceExtensionCount();

VKTS_APICALL const char** VKTS_APIENTRY wsiGetNeededInstanceExtensionNames();

VKTS_APICALL VkBool32 VKTS_APIENTRY wsiInitInstanceExtensions(const VkInstance instance);

//

VKTS_APICALL VkBool32 VKTS_APIENTRY wsiGatherNeededDeviceExtensions(const VkPhysicalDevice physicalDevice);

VKTS_APICALL uint32_t VKTS_APIENTRY wsiGetNeededDeviceExtensionCount();

VKTS_APICALL const char** VKTS_APIENTRY wsiGetNeededDeviceExtensionNames();

VKTS_APICALL VkBool32 VKTS_APIENTRY wsiInitDeviceExtensions(const VkDevice device);

VKTS_APICALL VkResult VKTS_APIENTRY wsiGetPhysicalDeviceSurfaceSupport(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface, const uint32_t queueFamilyIndexCount, std::vector<VkBool32>& supported);

VKTS_APICALL ISurfaceSP VKTS_APIENTRY wsiSurfaceCreate(const VkInstance instance, VKTS_NATIVE_DISPLAY nativeDisplay, VKTS_NATIVE_WINDOW nativeWindow);

VKTS_APICALL ISwapchainSP VKTS_APIENTRY wsiSwapchainCreate(const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSwapchainCreateFlagsKHR flags, const VkSurfaceKHR surface, const uint32_t minImageCount, const VkExtent2D& imageExtent, const uint32_t imageArrayLayers, const VkImageUsageFlags imageUsage, const VkSharingMode imageSharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkCompositeAlphaFlagBitsKHR compositeAlpha, const VkBool32 clipped, const VkSwapchainKHR oldSwapchain);

}

#endif /* VKTS_FN_WSI_HPP_ */
