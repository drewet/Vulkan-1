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

#include "Context.hpp"

namespace vkts
{

Context::Context(const VkBool32 replace, const IInitialResourcesSP& initialResources, const ICommandBuffersSP& cmdBuffer, const VkSamplerCreateInfo& samplerCreateInfo, const VkImageViewCreateInfo& imageViewCreateInfo, const IDescriptorSetLayoutSP& descriptorSetLayout) :
    IContext(), replace(replace), initialResources(initialResources), cmdBuffer(cmdBuffer), samplerCreateInfo(samplerCreateInfo), imageViewCreateInfo(imageViewCreateInfo), descriptorSetLayout(descriptorSetLayout), allObjects(), allMeshes(), allSubMeshes(), allAnimations(), allChannels(), allBRDFMaterials(), allPhongMaterials(), allTextures(), allImageDatas()
{
}

Context::~Context()
{
    destroy();
}

//
// IContext
//

const IInitialResourcesSP& Context::getInitialResources() const
{
    return initialResources;
}

const ICommandBuffersSP& Context::getCommandBuffer() const
{
    return cmdBuffer;
}

const VkSamplerCreateInfo& Context::getSamplerCreateInfo() const
{
    return samplerCreateInfo;
}

const VkImageViewCreateInfo& Context::getImageViewCreateInfo() const
{
    return imageViewCreateInfo;
}

const IDescriptorSetLayoutSP& Context::getDescriptorSetLayout() const
{
    return descriptorSetLayout;
}

//

const IObjectSP& Context::getObject(const std::string& name) const
{
    return get(name, allObjects);
}

VkBool32 Context::addObject(const IObjectSP& object)
{
    if (!object.get())
    {
        return VK_FALSE;
    }

    return add(object->getName(), object, allObjects);
}

VkBool32 Context::removeObject(const IObjectSP& object)
{
    if (!object.get())
    {
        return VK_FALSE;
    }

    return remove(object->getName(), allObjects);
}

//

const IMeshSP& Context::getMesh(const std::string& name) const
{
    return get(name, allMeshes);
}

VkBool32 Context::addMesh(const IMeshSP& mesh)
{
    if (!mesh.get())
    {
        return VK_FALSE;
    }

    return add(mesh->getName(), mesh, allMeshes);
}

VkBool32 Context::removeMesh(const IMeshSP& mesh)
{
    if (!mesh.get())
    {
        return VK_FALSE;
    }

    return remove(mesh->getName(), allMeshes);
}

const ISubMeshSP& Context::getSubMesh(const std::string& name) const
{
    return get(name, allSubMeshes);
}

VkBool32 Context::addSubMesh(const ISubMeshSP& subMesh)
{
    if (!subMesh.get())
    {
        return VK_FALSE;
    }

    return add(subMesh->getName(), subMesh, allSubMeshes);
}

VkBool32 Context::removeSubMesh(const ISubMeshSP& subMesh)
{
    if (!subMesh.get())
    {
        return VK_FALSE;
    }

    return remove(subMesh->getName(), allSubMeshes);
}

//

const IAnimationSP& Context::getAnimation(const std::string& name) const
{
    return get(name, allAnimations);
}

VkBool32 Context::addAnimation(const IAnimationSP& animation)
{
    if (!animation.get())
    {
        return VK_FALSE;
    }

    return add(animation->getName(), animation, allAnimations);
}

VkBool32 Context::removeAnimation(const IAnimationSP& animation)
{
    if (!animation.get())
    {
        return VK_FALSE;
    }

    return remove(animation->getName(), allAnimations);
}

const IChannelSP& Context::getChannel(const std::string& name) const
{
    return get(name, allChannels);
}

VkBool32 Context::addChannel(const IChannelSP& channel)
{
    if (!channel.get())
    {
        return VK_FALSE;
    }

    return add(channel->getName(), channel, allChannels);
}

VkBool32 Context::removeChannel(const IChannelSP& channel)
{
    if (!channel.get())
    {
        return VK_FALSE;
    }

    return remove(channel->getName(), allChannels);
}

//

const IBRDFMaterialSP& Context::getBRDFMaterial(const std::string& name) const
{
    return get(name, allBRDFMaterials);
}

VkBool32 Context::addBRDFMaterial(const IBRDFMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    return add(material->getName(), material, allBRDFMaterials);
}

VkBool32 Context::removeBRDFMaterial(const IBRDFMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    return remove(material->getName(), allBRDFMaterials);
}

const IPhongMaterialSP& Context::getPhongMaterial(const std::string& name) const
{
    return get(name, allPhongMaterials);
}

VkBool32 Context::addPhongMaterial(const IPhongMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    return add(material->getName(), material, allPhongMaterials);
}

VkBool32 Context::removePhongMaterial(const IPhongMaterialSP& material)
{
    if (!material.get())
    {
        return VK_FALSE;
    }

    return remove(material->getName(), allPhongMaterials);
}

//

const ITextureSP& Context::getTexture(const std::string& name) const
{
    return get(name, allTextures);
}

VkBool32 Context::addTexture(const ITextureSP& texture)
{
    if (!texture.get())
    {
        return VK_FALSE;
    }

    return add(texture->getName(), texture, allTextures);
}

VkBool32 Context::removeTexture(const ITextureSP& texture)
{
    if (!texture.get())
    {
        return VK_FALSE;
    }

    return remove(texture->getName(), allTextures);
}

//

const IImageDataSP& Context::getImageData(const std::string& name) const
{
    return get(name, allImageDatas);
}

VkBool32 Context::addImageData(const IImageDataSP& imageData)
{
    if (!imageData.get())
    {
        return VK_FALSE;
    }

    return add(imageData->getName(), imageData, allImageDatas);
}

VkBool32 Context::removeImageData(const IImageDataSP& imageData)
{
    if (!imageData.get())
    {
        return VK_FALSE;
    }

    return remove(imageData->getName(), allImageDatas);
}

//

void Context::addStageImage(const IImageSP& stageImage)
{
    if (stageImage.get())
    {
        allStageImages.append(stageImage);
    }
}

void Context::addStageBuffer(const IBufferSP& stageBuffer)
{
    if (stageBuffer.get())
    {
        allStageBuffers.append(stageBuffer);
    }
}

void Context::addStageDeviceMemory(const IDeviceMemorySP& stageDeviceMemory)
{
    if (stageDeviceMemory.get())
    {
        allStageDeviceMemories.append(stageDeviceMemory);
    }
}

//
// IDestroyable
//

void Context::destroy()
{
    // Only free resources, but do not destroy them.

    allObjects.clear();

    allMeshes.clear();

    allSubMeshes.clear();

    allAnimations.clear();

    allChannels.clear();

    allBRDFMaterials.clear();

    allPhongMaterials.clear();

    allTextures.clear();

    allImageDatas.clear();

    // Stage resources have to be deleted.

    for (uint32_t i = 0; i < allStageImages.size(); i++)
    {
        if (allStageImages[i].get())
        {
            allStageImages[i]->destroy();
        }
    }
    allStageImages.clear();

    for (uint32_t i = 0; i < allStageBuffers.size(); i++)
    {
        if (allStageBuffers[i].get())
        {
            allStageBuffers[i]->destroy();
        }
    }
    allStageBuffers.clear();

    for (uint32_t i = 0; i < allStageDeviceMemories.size(); i++)
    {
        if (allStageDeviceMemories[i].get())
        {
            allStageDeviceMemories[i]->destroy();
        }
    }
    allStageDeviceMemories.clear();
}

} /* namespace vkts */
