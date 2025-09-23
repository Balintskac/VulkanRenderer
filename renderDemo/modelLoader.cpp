#include "modelLoader.h"

void ModelLoader::loadModel()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    const std::string MODEL_PATH = "models/sponza/sponza.obj";
    const std::string BASE_PATH = "models/sponza/";
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, &err, MODEL_PATH.c_str(), BASE_PATH.c_str()))
    {
        throw std::runtime_error(err);
    }

    size_t tex_index = 0;

    size_t globalIndex = 0;

    std::unordered_map<VertexModel, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {

        const auto& mesh = shape.mesh;
        // Végigmegyünk minden polygonon (face)
        for (size_t i = 0; i < mesh.indices.size(); ++i) {
            const auto& index = mesh.indices[i];


            //if (material_id < 0) material_id = 0;        // ha nincs material

            VertexModel vertex{};
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.texcoord_index >= 0) {
                vertex.tex = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            vertex.color = { 1.0f, 1.0f, 1.0f };

            // Face index meghatározása
            size_t faceIndex = i / 3;

            if (i / 3 < mesh.material_ids.size())
                vertex.texIndex = mesh.material_ids[faceIndex] >= 0? mesh.material_ids[faceIndex] : 0; // vagy textúra ID táblából név/útvonal alapján

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
            
    std::cout << "size of index of the model: " << vertices.size() << std::endl;
    std::cout << "size of indices of the model: " << indices.size() << std::endl;
    std::cout << "size of unique indices of the model: " << uniqueVertices.size() << std::endl;

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(pyhsicalDevice, &props);
    std::cout << "Max samplers per stage: " << props.limits.maxPerStageDescriptorSamplers << std::endl;
}

void ModelLoader::createsVertexBuffer()
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = BufferManager::findingMemoryType(memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pyhsicalDevice);
   
    if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
    
    


    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);

    memcpy(data, vertices.data(), (size_t)bufferInfo.size);

    vkUnmapMemory(device, vertexBufferMemory);

}

void ModelLoader::createsIndexBuffer()
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

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    auto memoryTypeIndex = BufferManager::findingMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        pyhsicalDevice
    );

    VkMemoryAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = memoryTypeIndex,
    };

    vkAllocateMemory(device, &allocInfo, nullptr, &indexBufferMemory);
    vkBindBufferMemory(device, indexBuffer, indexBufferMemory, 0);

    void* data;
    vkMapMemory(device, indexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);

    vkUnmapMemory(device, indexBufferMemory);
}
