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

#include "BRDFMaterial.hpp"

namespace vkts
{

BRDFMaterial::BRDFMaterial() :
    Material(), IBRDFMaterial(), baseColor(), metallic(), specular(), roughness()
{
}

BRDFMaterial::BRDFMaterial(const BRDFMaterial& other) :
    Material(other), IBRDFMaterial(), baseColor(other.baseColor), metallic(other.metallic), specular(other.specular), roughness(other.roughness)
{
    // Textures cannot be cloned, just replaced.

    //

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_BASE_COLOR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->baseColor->getSampler()->getSampler(), this->baseColor->getImageView()->getImageView(), this->baseColor->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_METALLIC - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->metallic->getSampler()->getSampler(), this->metallic->getImageView()->getImageView(), this->metallic->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_SPECULAR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specular->getSampler()->getSampler(), this->specular->getImageView()->getImageView(), this->specular->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_ROUGHNESS - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->roughness->getSampler()->getSampler(), this->roughness->getImageView()->getImageView(), this->roughness->getImage()->getImageLayout());
}

BRDFMaterial::~BRDFMaterial()
{
    destroy();
}

//
// IMaterial
//

const std::string& BRDFMaterial::getName() const
{
    return name;
}

void BRDFMaterial::setName(const std::string& name)
{
    this->name = name;
}

const ITextureSP& BRDFMaterial::getEmissive() const
{
    return emissive;
}

void BRDFMaterial::setEmissive(const ITextureSP& emissive)
{
    this->emissive = emissive;

    if (this->emissive.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->emissive->getSampler()->getSampler(), this->emissive->getImageView()->getImageView(), this->emissive->getImage()->getImageLayout());
    }
}

const ITextureSP& BRDFMaterial::getAlpha() const
{
    return alpha;
}

void BRDFMaterial::setAlpha(const ITextureSP& alpha)
{
    this->alpha = alpha;

    if (this->alpha.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_ALPHA - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->alpha->getSampler()->getSampler(), this->alpha->getImageView()->getImageView(), this->alpha->getImage()->getImageLayout());
    }
}

const ITextureSP& BRDFMaterial::getDisplacement() const
{
    return displacement;
}

void BRDFMaterial::setDisplacement(const ITextureSP& displacement)
{
    this->displacement = displacement;

    if (this->displacement.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_DISPLACEMENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->displacement->getSampler()->getSampler(), this->displacement->getImageView()->getImageView(), this->displacement->getImage()->getImageLayout());
    }
}

const ITextureSP& BRDFMaterial::getNormal() const
{
    return normal;
}

void BRDFMaterial::setNormal(const ITextureSP& normal)
{
    this->normal = normal;

    if (this->normal.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_NORMAL - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->normal->getSampler()->getSampler(), this->normal->getImageView()->getImageView(), this->normal->getImage()->getImageLayout());
    }
}

const IDescriptorPoolSP& BRDFMaterial::getDescriptorPool() const
{
    return descriptorPool;
}

void BRDFMaterial::setDescriptorPool(const IDescriptorPoolSP& descriptorPool)
{
    this->descriptorPool = descriptorPool;
}

IDescriptorSetsSP BRDFMaterial::getDescriptorSets() const
{
    return descriptorSets;
}

void BRDFMaterial::setDescriptorSets(const IDescriptorSetsSP& descriptorSets)
{
    this->descriptorSets = descriptorSets;
}

void BRDFMaterial::updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    for (uint32_t i = 0; i < VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT; i++)
    {
    	allWriteDescriptorSets[writeDescriptorSets[i].dstBinding] = writeDescriptorSets[i];
    }

    for (uint32_t i = 0; i < VKTS_BINDING_UNIFORM_BINDING_COUNT; i++)
    {
    	if (i >= allWriteDescriptorSetsCount)
    	{
    		break;
    	}

    	allWriteDescriptorSets[i].dstSet = descriptorSets->getDescriptorSets()[0];
    }

    descriptorSets->updateDescriptorSets(allWriteDescriptorSetsCount, allWriteDescriptorSets, 0, nullptr);
}

void BRDFMaterial::bindDescriptorSets(const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout) const
{
    if (!cmdBuffer.get())
    {
        return;
    }

    if (!descriptorSets.get())
    {
        return;
    }

    vkCmdBindDescriptorSets(cmdBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSets->getDescriptorSets()[0], 0, nullptr);
}

void BRDFMaterial::bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline) const
{
    bindDescriptorSets(cmdBuffer, graphicsPipeline->getLayout());
}

//
// IBRDFMaterial
//

const ITextureSP& BRDFMaterial::getBaseColor() const
{
    return baseColor;
}

void BRDFMaterial::setBaseColor(const ITextureSP& baseColor)
{
    this->baseColor = baseColor;

    if (this->baseColor.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_BASE_COLOR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->baseColor->getSampler()->getSampler(), this->baseColor->getImageView()->getImageView(), this->baseColor->getImage()->getImageLayout());
    }
}

const ITextureSP& BRDFMaterial::getMetallic() const
{
    return metallic;
}

void BRDFMaterial::setMetallic(const ITextureSP& metallic)
{
    this->metallic = metallic;

    if (this->metallic.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_METALLIC - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->metallic->getSampler()->getSampler(), this->metallic->getImageView()->getImageView(), this->metallic->getImage()->getImageLayout());
    }
}

const ITextureSP& BRDFMaterial::getSpecular() const
{
    return specular;
}

void BRDFMaterial::setSpecular(const ITextureSP& specular)
{
    this->specular = specular;

    if (this->specular.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_SPECULAR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specular->getSampler()->getSampler(), this->specular->getImageView()->getImageView(), this->specular->getImage()->getImageLayout());
    }
}

const ITextureSP& BRDFMaterial::getRoughness() const
{
    return roughness;
}

void BRDFMaterial::setRoughness(const ITextureSP& roughness)
{
    this->roughness = roughness;

    if (this->roughness.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_BRDF_ROUGHNESS - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->roughness->getSampler()->getSampler(), this->roughness->getImageView()->getImageView(), this->roughness->getImage()->getImageLayout());
    }
}

//
// ICloneable
//

IBRDFMaterialSP BRDFMaterial::clone() const
{
    return IBRDFMaterialSP(new BRDFMaterial(*this));
}

//
// IDestroyable
//

void BRDFMaterial::destroy()
{
    if (baseColor.get())
    {
        baseColor->destroy();
    }

    if (metallic.get())
    {
        metallic->destroy();
    }

    if (specular.get())
    {
        specular->destroy();
    }

    if (roughness.get())
    {
        roughness->destroy();
    }

    destroyMaterial();
}

} /* namespace vkts */
