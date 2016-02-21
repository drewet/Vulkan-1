LOCAL_PATH			:= $(call my-dir)

#
# Vulkan.
#

include $(CLEAR_VARS)

LOCAL_MODULE := vulkan

# TODO: Remove, as soon as Vulkan is added to the NDK.
LOCAL_SRC_FILES := C:/VulkanSDK/Android/$(TARGET_ARCH_ABI)/libvulkan.so

include $(PREBUILT_SHARED_LIBRARY)

#
# VKTS.
#

include $(CLEAR_VARS)

LOCAL_MODULE := VKTS

LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../VKTS/Android/obj/local/$(TARGET_ARCH_ABI)/libVKTS.a

include $(PREBUILT_STATIC_LIBRARY)

#
# Test.
#

include $(CLEAR_VARS)

LOCAL_MODULE := VKTS_Test_General

# All files.

PROJECT_FILES := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)

# Generate the final list.

PROJECT_FILES := $(PROJECT_FILES:$(LOCAL_PATH)/%=%)

# Enable C++11.

LOCAL_CPPFLAGS := -std=c++11
LOCAL_CPPFLAGS += -DVK_PROTOTYPES
LOCAL_CPPFLAGS += -fexceptions

# Includes.

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../../../VKTS/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../VKTS_External/include

# TODO: Remove, as soon as Vulkan is added to the NDK.
LOCAL_C_INCLUDES += C:/VulkanSDK/1.0.3.1/Include 

# Sources.

LOCAL_SRC_FILES := $(PROJECT_FILES)

# Libs.

# TODO: Later remove EGL.
LOCAL_LDLIBS    := -landroid

LOCAL_STATIC_LIBRARIES := VKTS
LOCAL_STATIC_LIBRARIES += android_native_app_glue
LOCAL_STATIC_LIBRARIES += cpufeatures

LOCAL_SHARED_LIBRARIES := vulkan

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)
