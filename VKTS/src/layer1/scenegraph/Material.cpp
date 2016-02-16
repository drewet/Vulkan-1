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

#include "Material.hpp"

namespace vkts
{

void Material::resetMaterial()
{
    descriptorPool = IDescriptorPoolSP();
    descriptorSets = IDescriptorSetsSP();

    memset(&descriptorImageInfos, 0, sizeof(descriptorImageInfos));
    memset(&writeDescriptorSets, 0, sizeof(writeDescriptorSets));
}

void Material::destroyMaterial()
{
    if (emissive.get())
    {
        emissive->destroy();
    }

    if (alpha.get())
    {
        alpha->destroy();
    }

    if (displacement.get())
    {
        displacement->destroy();
    }

    if (normal.get())
    {
        normal->destroy();
    }
}

Material::Material() :
    name(""), emissive(), alpha(), displacement(), normal(), descriptorPool(), descriptorSets()
{
    resetMaterial();
}

Material::Material(const Material& other) :
    name(other.name), emissive(other.emissive), alpha(other.alpha), displacement(other.displacement), normal(other.normal), descriptorPool(), descriptorSets()
{
    // Textures cannot be cloned, just replaced.

    resetMaterial();

    //

    descriptorPool = descriptorPoolCreate(other.descriptorPool->getDevice(), other.descriptorPool->getFlags(), other.descriptorPool->getMaxSets(), other.descriptorPool->getPoolSizeCount(), other.descriptorPool->getPoolSizes());

    if (!descriptorPool.get())
    {
        resetMaterial();

        return;
    }

    descriptorSets = descriptorSetsCreate(descriptorPool->getDevice(), descriptorPool->getDescriptorPool(), other.descriptorSets->getDescriptorSetCount(), other.descriptorSets->getSetLayouts());

    if (!descriptorSets.get())
    {
        resetMaterial();

        return;
    }

    //

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->emissive->getSampler()->getSampler(), this->emissive->getImageView()->getImageView(), this->emissive->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_ALPHA - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->alpha->getSampler()->getSampler(), this->alpha->getImageView()->getImageView(), this->alpha->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_DISPLACEMENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->displacement->getSampler()->getSampler(), this->displacement->getImageView()->getImageView(), this->displacement->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_NORMAL - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->normal->getSampler()->getSampler(), this->normal->getImageView()->getImageView(), this->normal->getImage()->getImageLayout());
}

Material::~Material()
{
    destroyMaterial();
}

void Material::updateDescriptorImageInfo(const uint32_t colorIndex, const VkSampler sampler, const VkImageView imageView, const VkImageLayout imageLayout)
{
    if (colorIndex >= VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT)
    {
        return;
    }

    memset(&descriptorImageInfos[colorIndex], 0, sizeof(VkDescriptorImageInfo));

    descriptorImageInfos[colorIndex].sampler = sampler;
    descriptorImageInfos[colorIndex].imageView = imageView;
    descriptorImageInfos[colorIndex].imageLayout = imageLayout;

    memset(&writeDescriptorSets[colorIndex], 0, sizeof(VkWriteDescriptorSet));

    writeDescriptorSets[colorIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[colorIndex].dstSet = VK_NULL_HANDLE; // Defined later.
    writeDescriptorSets[colorIndex].dstBinding = VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE + colorIndex;
    writeDescriptorSets[colorIndex].dstArrayElement = 0;
    writeDescriptorSets[colorIndex].descriptorCount = 1;
    writeDescriptorSets[colorIndex].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[colorIndex].pImageInfo = &descriptorImageInfos[colorIndex];
    writeDescriptorSets[colorIndex].pBufferInfo = nullptr;
    writeDescriptorSets[colorIndex].pTexelBufferView = nullptr;
}

} /* namespace vkts */
