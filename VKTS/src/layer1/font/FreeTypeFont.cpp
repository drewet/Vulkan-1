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

#include "FreeTypeFont.hpp"

#define VKTS_TO_CHAR_SIZE(size) ((FT_F26Dot6)((size) << 6))
#define VKTS_FROM_CHAR_SIZE(size) ((size) >> 6)

namespace vkts
{

FontCacheEntry* FreeTypeFont::getGlyph(const char32_t character, const size_t size)
{
    if (!face)
    {
        return nullptr;
    }

    //

    auto results = cache.find(character);

    while (results != cache.cend())
    {
        if (results->second.size == size)
        {
            return &results->second;
        }

        results++;
    }

    //

    if (FT_Set_Char_Size(face, VKTS_TO_CHAR_SIZE(size), VKTS_TO_CHAR_SIZE(size), 0, 0))
    {
        return nullptr;
    }

    auto glyphIndex = FT_Get_Char_Index(face, character);

    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
    {
        return nullptr;
    }

    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
    {
        return nullptr;
    }

    //

    IImageDataSP glyphImageData;

    if (face->glyph->bitmap.buffer)
    {
    	glyphImageData = imageDataCreate("FontCharacterImageData", face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, VK_IMAGE_TYPE_2D, VK_FORMAT_R8_UNORM);


        if (!glyphImageData.get())
        {
            return nullptr;
        }


        VkSubresourceLayout subresourceLayout;

        subresourceLayout.offset = 0;
        subresourceLayout.size = face->glyph->bitmap.width * face->glyph->bitmap.rows;
        subresourceLayout.rowPitch = face->glyph->bitmap.width;
        subresourceLayout.arrayPitch = 0;
        subresourceLayout.depthPitch = 0;

        if (!glyphImageData->upload(face->glyph->bitmap.buffer, 0, subresourceLayout))
        {
        	return nullptr;
        }
    }
    else
    {
        // For space, no data is created.

    	glyphImageData = imageDataCreate("FontCharacterImageData", 1, 1, 1, 0.0f, 0.0f, 0.0f, 0.0f, VK_IMAGE_TYPE_2D, VK_FORMAT_R8_UNORM);

        if (!glyphImageData.get())
        {
            return nullptr;
        }
    }

    //

    FontCacheEntry fontCacheEntry;

    fontCacheEntry.character = character;
    fontCacheEntry.size = size;

    fontCacheEntry.width = face->glyph->bitmap.width;
    fontCacheEntry.height = face->glyph->bitmap.rows;

    fontCacheEntry.advanceX = VKTS_FROM_CHAR_SIZE(face->glyph->advance.x);
    fontCacheEntry.advanceY = VKTS_FROM_CHAR_SIZE(face->glyph->advance.y);
    fontCacheEntry.offsetX = VKTS_FROM_CHAR_SIZE(face->glyph->metrics.vertBearingX - face->glyph->metrics.width);
    fontCacheEntry.offsetY = VKTS_FROM_CHAR_SIZE(face->glyph->metrics.horiBearingY - face->glyph->metrics.height);

    fontCacheEntry.glyphImageData = glyphImageData;

    fontCacheEntry.texture = ITextureSP();

    cache.insert(std::make_pair(character, fontCacheEntry));

    //

    results = cache.find(character);

    while (results != cache.cend())
    {
        if (results->second.size == size)
        {
            return &results->second;
        }

        results++;
    }

    return nullptr;
}

FontCacheEntry* FreeTypeFont::getGlyph(const ICommandBuffersSP& cmdBuffer, const char32_t character, const size_t size)
{
    if (!face || !cmdBuffer.get())
    {
        return nullptr;
    }

    //

    FontCacheEntry* fontCacheEntry = getGlyph(character, size);

    // Error case.
    if (!fontCacheEntry)
    {
        return nullptr;
    }

    // Character has no data e.g. space.
    if (!fontCacheEntry->glyphImageData.get())
    {
        return fontCacheEntry;
    }

    // Character already created.
    if (fontCacheEntry->texture.get())
    {
        return fontCacheEntry;
    }

    //

    VkImageTiling imageTiling;
    VkMemoryPropertyFlags memoryPropertyFlags;

    if (!initialResources->getPhysicalDevice()->getGetImageTilingAndMemoryProperty(imageTiling, memoryPropertyFlags, fontCacheEntry->glyphImageData->getFormat(), fontCacheEntry->glyphImageData->getImageType(), 0, fontCacheEntry->glyphImageData->getExtent3D(), fontCacheEntry->glyphImageData->getMipLevels(), 1, VK_SAMPLE_COUNT_1_BIT, fontCacheEntry->glyphImageData->getSize()))
    {
        return nullptr;
    }

    //

    VkImageCreateInfo imageCreateInfo;

    memset(&imageCreateInfo, 0, sizeof(VkImageCreateInfo));

    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = fontCacheEntry->glyphImageData->getImageType();
    imageCreateInfo.format = fontCacheEntry->glyphImageData->getFormat();
    imageCreateInfo.extent = fontCacheEntry->glyphImageData->getExtent3D();
    imageCreateInfo.mipLevels = fontCacheEntry->glyphImageData->getMipLevels();
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = imageTiling;
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    VkSamplerCreateInfo samplerCreateInfo;

    memset(&samplerCreateInfo, 0, sizeof(VkSamplerCreateInfo));

    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    samplerCreateInfo.flags = 0;
    samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
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

    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = VK_NULL_HANDLE;     // Defined later.
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = fontCacheEntry->glyphImageData->getFormat();
    imageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
    imageViewCreateInfo.subresourceRange = subresourceRange;

    IImageSP stageImage;
    IBufferSP stageBuffer;
    IDeviceMemorySP stageDeviceMemory;

    fontCacheEntry->texture = textureCreate(stageImage, stageBuffer, stageDeviceMemory, initialResources, cmdBuffer, "FontCharacterTexture", fontCacheEntry->glyphImageData, imageCreateInfo, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange, memoryPropertyFlags, samplerCreateInfo, imageViewCreateInfo);

    if (!fontCacheEntry->texture.get())
    {
        return nullptr;
    }

    if (stageImage.get())
    {
    	allStageImages.append(stageImage);
	}
    if (stageBuffer.get())
    {
    	allStageBuffers.append(stageBuffer);
    }
    if (stageDeviceMemory.get())
    {
        allStageDeviceMemories.append(stageDeviceMemory);
    }

    return fontCacheEntry;
}

FreeTypeFont::FreeTypeFont(const IInitialResourcesSP& initialResources, const FT_Library& library, const FT_Face& face) :
    IFont(), initialResources(initialResources), library(library), face(face), cache(), allStageImages(), allStageBuffers(), allStageDeviceMemories()
{
}

FreeTypeFont::~FreeTypeFont()
{
    destroy();
}

//
// IFont
//

VkBool32 FreeTypeFont::prepareText(const ICommandBuffersSP& cmdBuffer, const std::u32string& text, const size_t size)
{
    if (!face || !cmdBuffer.get())
    {
        return VK_FALSE;
    }

    size_t textIndex = 0;

    FontCacheEntry* fontCacheEntry;

    while (textIndex < text.length())
    {
        fontCacheEntry = getGlyph(cmdBuffer, text[textIndex], size);

        if (fontCacheEntry == nullptr)
        {
            return VK_FALSE;
        }

        textIndex++;
    }

    return VK_TRUE;
}

VkBool32 FreeTypeFont::renderText(const ICommandBuffersSP& cmdBuffer, const std::u32string& text, const size_t size, const glm::vec4& color)
{
    if (!face || !cmdBuffer.get())
    {
        return VK_FALSE;
    }

    //

    // TODO: Create once vertex buffer etc. like in example 3.

    // TODO: Create for each letter a descriptor set.

    //

    size_t textIndex = 0;

    FontCacheEntry* fontCacheEntry;

    while (textIndex < text.length())
    {
        // Do not use the render command buffer for creation-
        fontCacheEntry = getGlyph(text[textIndex], size);

        if (fontCacheEntry == nullptr)
        {
            return VK_FALSE;
        }

        // TODO: Bind descriptor set.

        // TODO: Draw triangle square.

        textIndex++;
    }

    return VK_TRUE;
}

int32_t FreeTypeFont::getTextLength(const std::u32string& text, const size_t size)
{
    if (!face)
    {
        return -1;
    }

    int32_t textLength = 0;

    size_t textIndex = 0;

    FontCacheEntry* fontCacheEntry;

    while (textIndex < text.length())
    {
        fontCacheEntry = getGlyph(text[textIndex], size);

        if (fontCacheEntry == nullptr)
        {
            return -1;
        }

        textLength += fontCacheEntry->advanceX;

        textIndex++;
    }

    return textLength;
}

void FreeTypeFont::destroyStageResources()
{
    for (uint32_t i = 0; i < allStageImages.size(); i++)
    {
        allStageImages[i]->destroy();
    }
    allStageImages.clear();

    for (uint32_t i = 0; i < allStageBuffers.size(); i++)
    {
        allStageBuffers[i]->destroy();
    }
    allStageBuffers.clear();

    for (uint32_t i = 0; i < allStageDeviceMemories.size(); i++)
    {
        allStageDeviceMemories[i]->destroy();
    }
    allStageDeviceMemories.clear();
}

//
// IDestroyable
//

void FreeTypeFont::destroy()
{
    destroyStageResources();

    //

    for (auto currentCachePair : cache)
    {
        if (currentCachePair.second.texture.get())
        {
            currentCachePair.second.texture->destroy();
        }
    }

    cache.clear();

    //

    if (face)
    {
        FT_Done_Face(face);

        face = nullptr;
    }
}

}

