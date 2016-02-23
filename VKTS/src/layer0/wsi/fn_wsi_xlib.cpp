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

#include <vkts/vkts.hpp>

#include "fn_wsi_internal.hpp"

#include <X11/Xlib-xcb.h>

PFN_vkCreateXcbSurfaceKHR vktsCreateXcbSurfaceKHR;
PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vktsGetPhysicalDeviceXcbPresentationSupportKHR;

PFN_vkCreateXlibSurfaceKHR vktsCreateXlibSurfaceKHR;
PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vktsGetPhysicalDeviceXlibPresentationSupportKHR;

namespace vkts
{

static VkBool32 g_hasXcb = VK_FALSE;
static VkBool32 g_hasXlib = VK_FALSE;

VkBool32 VKTS_APIENTRY _wsiGatherNeededInstanceExtensions(const std::vector<VkExtensionProperties>& allInstanceExtensionProperties, const char* instanceExtensionNames[], uint32_t& instanceExtensionCount)
{
	g_hasXcb = VK_FALSE;

    for (uint32_t i = 0; i < allInstanceExtensionProperties.size(); i++)
    {
        if (strcmp(VK_KHR_XCB_SURFACE_EXTENSION_NAME, allInstanceExtensionProperties[i].extensionName) == 0)
        {
            instanceExtensionNames[instanceExtensionCount] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;

            instanceExtensionCount++;

            g_hasXcb = VK_TRUE;

            break;
        }
    }

    if (!g_hasXcb)
    {
    	g_hasXlib = VK_FALSE;

        for (uint32_t i = 0; i < allInstanceExtensionProperties.size(); i++)
        {
            if (strcmp(VK_KHR_XLIB_SURFACE_EXTENSION_NAME, allInstanceExtensionProperties[i].extensionName) == 0)
            {
                instanceExtensionNames[instanceExtensionCount] = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;

                instanceExtensionCount++;

                g_hasXlib = VK_TRUE;

                break;
            }
        }

        if (!g_hasXlib)
        {
            return VK_FALSE;
        }
    }

    //

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY _wsiInitInstanceExtensions(const VkInstance instance)
{
    if (!instance || !(g_hasXcb || g_hasXlib))
    {
        return VK_FALSE;
    }

    if (g_hasXcb)
    {
        GET_INSTANCE_PROC_ADDR(instance, CreateXcbSurfaceKHR);
        GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceXcbPresentationSupportKHR);
    }

    if (g_hasXlib)
    {
        GET_INSTANCE_PROC_ADDR(instance, CreateXlibSurfaceKHR);
        GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceXlibPresentationSupportKHR);
    }

    return VK_TRUE;
}

VkSurfaceKHR VKTS_APIENTRY _wsiSurfaceCreate(const VkInstance instance, VKTS_NATIVE_DISPLAY nativeDisplay, VKTS_NATIVE_WINDOW nativeWindow)
{
    if (!instance)
    {
        return VK_NULL_HANDLE;
    }

    //

    VkResult result;

    VkSurfaceKHR surface;

    if (g_hasXcb)
    {
        VkXcbSurfaceCreateInfoKHR xcbSurfaceCreateInfoKHR;

        memset(&xcbSurfaceCreateInfoKHR, 0, sizeof(VkXcbSurfaceCreateInfoKHR));

        xcbSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;

        xcbSurfaceCreateInfoKHR.flags = 0;
        xcbSurfaceCreateInfoKHR.connection = XGetXCBConnection(nativeDisplay);
        xcbSurfaceCreateInfoKHR.window = (xcb_window_t)nativeWindow;

        result = vktsCreateXcbSurfaceKHR(instance, &xcbSurfaceCreateInfoKHR, nullptr, &surface);

        if (result != VK_SUCCESS)
        {
            return VK_NULL_HANDLE;
        }

        return surface;
    }

    if (g_hasXlib)
    {
    	VkXlibSurfaceCreateInfoKHR xlibSurfaceCreateInfoKHR;

    	memset(&xlibSurfaceCreateInfoKHR, 0, sizeof(VkXlibSurfaceCreateInfoKHR));

    	xlibSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;

    	xlibSurfaceCreateInfoKHR.flags = 0;
    	xlibSurfaceCreateInfoKHR.dpy = nativeDisplay;
    	xlibSurfaceCreateInfoKHR.window = nativeWindow;

        result = vktsCreateXlibSurfaceKHR(instance, &xlibSurfaceCreateInfoKHR, nullptr, &surface);

        if (result != VK_SUCCESS)
        {
            return VK_NULL_HANDLE;
        }

        return surface;
    }

    return VK_NULL_HANDLE;
}

}
