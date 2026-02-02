#include "VertexBuffer.h"

void VertexBuffer::createVertexBuffer(const VkDevice& device)
{

    instances.resize(100);

    for (size_t i = 0; i < instances.size(); i++) {
        float x = (i % 10) * 2.0f;    
        float y = (i / 10) * 2.0f;    
        instances[i].offset = glm::vec3(x, y, 0.0f);
    }

    VkDeviceSize vertexBufferSize = sizeof(Vertex) * vertices.size();
    VkDeviceSize instanceBufferSize = sizeof(InstanceData) * instances.size();
    VkDeviceSize totalSize = vertexBufferSize + instanceBufferSize;

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = totalSize;// sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


    if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }
}

void VertexBuffer::memoryAllocation(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, physicalDevice);
  
    if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice)
{
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VertexBuffer::fillVertexBuffer(const VkDevice& device)
{
    VkDeviceSize vertexBufferSize = sizeof(Vertex) * vertices.size();
    VkDeviceSize instanceBufferSize = sizeof(InstanceData) * instances.size();
    VkDeviceSize totalSize = vertexBufferSize + instanceBufferSize;
    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, totalSize, 0, &data);

    memcpy(data, vertices.data(), (size_t)vertexBufferSize);

    memcpy((char*)data + vertexBufferSize, instances.data(), (size_t)instanceBufferSize);

    vkUnmapMemory(device, vertexBufferMemory);
}

void VertexBuffer::createIndexBuffer(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBufferCreateInfo bufferInfo = {
         .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
         .size = bufferSize,
         .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
         .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &indexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    auto memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        physicalDevice
    );

    VkMemoryAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = memoryTypeIndex,
    };

    vkAllocateMemory(device, &allocInfo, NULL, &indexBufferMemory);
    vkBindBufferMemory(device, indexBuffer, indexBufferMemory, 0);

    void* data;
    vkMapMemory(device, indexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);

    /*
    VkMappedMemoryRange range = {
    .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
    .memory = indexBufferMemory,
    .offset = 0,
    .size = VK_WHOLE_SIZE,
    };

    vkFlushMappedMemoryRanges(device, 1, &range);
    */

    vkUnmapMemory(device, indexBufferMemory);


}

void VertexBuffer::createDescriptorSetLayout(const VkDevice& device)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
   
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    /*
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(int);   // itt most csak egy int
    */
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
  //pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
         throw std::runtime_error("failed to create pipeline layout!");
    }
}

void VertexBuffer::createUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    createBuffer(device, physicalDevice, bufferSize,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        uniformBuffers, uniformBuffersMemory);
  
    vkMapMemory(device, uniformBuffersMemory, 0, bufferSize, 0, &uniformBuffersMapped);
    vkUnmapMemory(device, uniformBuffersMemory);
}

void VertexBuffer::createBuffer(const VkDevice& device, 
    const VkPhysicalDevice& physicalDevice,
    VkDeviceSize size, 
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &uniformBuffers) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, uniformBuffers, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &uniformBuffersMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, uniformBuffers, uniformBuffersMemory, 0);
}

void VertexBuffer::createDescriptorPool(const VkDevice& device)
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = meshes.size(); 

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = meshes.size() * 3; 

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = meshes.size();

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void VertexBuffer::createDescriptorSets(const VkDevice& device, const std::vector<Texture>& textures, std::vector<MeshObject>& meshescske)
{

    // Descriptor sets
    for (uint32_t i = 0; i < meshescske.size(); i++)
    {
        VkDescriptorSetAllocateInfo allocInfo{};

        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        allocInfo.pNext = NULL;

        if (vkAllocateDescriptorSets(device, &allocInfo, &meshescske[i].descriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        std::vector<VkWriteDescriptorSet> writeDescriptorSets;

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        // First write: Uniform buffer
        VkWriteDescriptorSet bufferWrite{};
        bufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        bufferWrite.dstSet = meshescske[i].descriptorSet;
        bufferWrite.dstBinding = 0;  // Binding point for the uniform buffer
     //   bufferWrite.dstArrayElement = 0;
        bufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        bufferWrite.descriptorCount = 1;
        bufferWrite.pBufferInfo = &bufferInfo;

        writeDescriptorSets.push_back(bufferWrite);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = meshescske[i].material->diffuse.descriptor.imageView;
        imageInfo.sampler = meshescske[i].material->diffuse.descriptor.sampler;

        VkWriteDescriptorSet imageWrite{};
        imageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        imageWrite.dstSet = meshescske[i].descriptorSet;
        imageWrite.dstBinding = 1;  // Binding point for textures
      //  imageWrite.dstArrayElement = 0;  // Array element index for the texture
        imageWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imageWrite.descriptorCount = 1;
        imageWrite.pImageInfo = &imageInfo;

        writeDescriptorSets.push_back(imageWrite);

        std::cout
            << "mesh " << i
            << " imageView " << meshescske[i].material->diffuse.descriptor.imageView
            << " sampler " << meshescske[i].material->diffuse.descriptor.sampler
            << std::endl;

        vkUpdateDescriptorSets(device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, NULL);
    }
}

