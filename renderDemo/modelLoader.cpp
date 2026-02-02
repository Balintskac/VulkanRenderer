#include "modelLoader.h"
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

namespace fs = std::filesystem;

void ModelLoader::loadModel(VulkanTextureManager& vkTextureManager)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materialsObj;
    std::string err;

    const std::string MODEL_PATH = "models/room/breakfast_room.obj";
    const std::string BASE_PATH = "models/room/";
    if (!tinyobj::LoadObj(&attrib, &shapes, &materialsObj, nullptr, &err, MODEL_PATH.c_str(), BASE_PATH.c_str()))
    {
        throw std::runtime_error(err);
    }


    std::vector<int> materialToTexIndex(materialsObj.size(), 0);

    std::vector<std::string> textureFiles;

    // összes fájl összegyűjtése
    for (const auto& entry : fs::directory_iterator("models/room/textures/")) {
        if (entry.is_regular_file()) {
            textureFiles.push_back(entry.path().string());
        }
    }


    size_t globalIndex = 0;
    size_t tex_index = 0;
    for (const auto& mat : textureFiles) {
        Material m{};
      //  m.diffuseColor = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);

      //  int texIdx = -1;
      
        auto it = textureLookup.find(mat.c_str());
        if (it != textureLookup.end()) {
            //   texIdx = it->second; // már betöltött textúra
            tex_index = it->second;
        }
        else {
            // Új textúra betöltése
        //    Texture tex = loadTextureFromFile(mat.c_str(), vkTextureManager);
          //  textures.push_back(tex);
            //   texIdx = static_cast<int>(textures.size() - 1);
            textureLookup[mat.c_str()] = globalIndex;
            globalIndex++;
        }
        

        m.diffuseTextureIndex = (tex_index > 0) ? tex_index : globalIndex; // ha nincs, fallback 0
        materials.push_back(m);
    }
    /*
    for (size_t i = 0; i < materialsObj.size(); i++)
    {
        const auto& mat = materialsObj[i];
        if (!mat.diffuse_texname.empty()) {
            materialToTexIndex[i] = textureLookup[mat.diffuse_texname];
        }
        else {
            materialToTexIndex[i] = 0; // default texture index
        }
    }

    */
   // std::unordered_map<VertexModel, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        size_t index_offset = 0;
        // Végigmegyünk minden polygonon (face)
        for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); ++i) 
        {
           // meshes[i].indexCount = aiScene->mNumFaces * 3;
           // const auto& index = mesh.indices[i];

             const auto& mesh = shape.mesh;
            int fv = mesh.num_face_vertices[i];
            int materialId = mesh.material_ids[i];
          //  if (materialId < 0) materialId = 0;
            int texIndex = materials[materialId].diffuseTextureIndex;

            /*
            int texIndex = 0;
            if (materialId >= 0 && materialId < (int)materialToTexIndex.size()) {
                texIndex = materialToTexIndex[materialId];
            }
            */

            for (size_t v = 0; v < fv; v++)
            {
                //::index_t idx = mesh.indices[index_offset + v];

                VertexModel vertex{};

                tinyobj::index_t index = shape.mesh.indices[index_offset + v];            vertex.pos = {
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
             //   size_t faceIndex = i / 3;

                //   if (i / 3 < mesh.material_ids.size())
                        //  vertex.texIndex = mesh.material_ids[faceIndex] >= 0? mesh.material_ids[faceIndex] : 0; // vagy textúra ID táblából név/útvonal alapján

         //       vertex.texIndex = texIndex;

             //   if (uniqueVertices.count(vertex) == 0) {
                 //   uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
             //       vertices.push_back(vertex);
            //    }

          //      indices.push_back(uniqueVertices[vertex]);
                
            }
            index_offset += fv;
        }
    }

    for (size_t m = 0; m < materialsObj.size(); m++) {
        std::cout << "Material " << m
            << " uses " << materialsObj[m].diffuse_texname << std::endl;
    }

            
    std::cout << "size of vertices of the model: " << vertices.size() << std::endl;
    std::cout << "size of indices of the model: " << indices.size() << std::endl;
   // std::cout << "size of unique indices of the model: " << uniqueVertices.size() << std::endl;

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(pyhsicalDevice, &props);
    std::cout << "Max samplers per stage: " << props.limits.maxPerStageDescriptorSamplers << std::endl;
}

void ModelLoader::loadModelWithAssimp(VulkanTextureManager& vkTextureManager)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("models/sponza/sponza.obj",
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error("Failed to load model with Assimp!");
    }

    objectMaterials.resize(scene->mNumMaterials);

    for (unsigned int i = 0; i < objectMaterials.size(); i++) {
        aiMaterial* material = scene->mMaterials[i];

        aiString name;
        scene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
        aiColor3D ambient;
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        //materials[i].name = name.C_Str();
        std::cout << "Material \"" << name.C_Str() << "\"" << std::endl;
        //  std::cout << "ambient \"" << ambient. << "\"" << std::endl;
        objectMaterials[i].name = name.C_Str();

        aiString path;
        //  material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
     //     if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
            std::string filename = path.C_Str();
            std::cout << "  Diffuse: \"" << path.C_Str() << "\"" << std::endl;
            // ha relatív, prependeld a model könyvtárat
            filename = "models/sponza/" + filename;


            auto tex = loadTextureFromFile(filename.c_str(), vkTextureManager);
            Texture* ptr = tex.get();
            objectMaterials[i].diffuse.descriptor = ptr->descriptor;
            objectMaterials[i].diffuse.sampler = ptr->sampler;
            objectMaterials[i].diffuse.imageView = ptr->imageView;
            //     }  

        }

        else if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS) {
            std::string filename = path.C_Str();
            std::cout << "  ambient: \"" << path.C_Str() << "\"" << std::endl;
            // ha relatív, prependeld a model könyvtárat
            filename = "models/room/textures/" + filename;


            auto tex = loadTextureFromFile(filename.c_str(), vkTextureManager);
            Texture* ptr = tex.get();
            objectMaterials[i].Ambient.descriptor = ptr->descriptor;
            objectMaterials[i].Ambient.sampler = ptr->sampler;
            objectMaterials[i].Ambient.imageView = ptr->imageView;
            //     }  

        }
        else if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
            std::string filename = path.C_Str();
            std::cout << "  ambient: \"" << path.C_Str() << "\"" << std::endl;
            // ha relatív, prependeld a model könyvtárat
            filename = "models/room/textures/" + filename;


            auto tex = loadTextureFromFile(filename.c_str(), vkTextureManager);
            Texture* ptr = tex.get();
            objectMaterials[i].specular.descriptor = ptr->descriptor;
            objectMaterials[i].specular.sampler = ptr->sampler;
            objectMaterials[i].specular.imageView = ptr->imageView;
            //     }  

        }
        else
        {
            std::string filename = "models/room/textures/bodas.jpg";
            auto tex = loadTextureFromFile(filename.c_str(), vkTextureManager);
            Texture* ptr = tex.get();
            objectMaterials[i].diffuse.descriptor = ptr->descriptor;
            objectMaterials[i].diffuse.sampler = ptr->sampler;
            objectMaterials[i].diffuse.imageView = ptr->imageView;
        }
    }

    uint32_t gIndexBase = 0;
    meshes.resize(scene->mNumMeshes);
    for (uint32_t i = 0; i < meshes.size(); i++)
    {
        aiMesh* mesh = scene->mMeshes[i];

        //   std::cout << "Mesh \"" << mesh->mName.C_Str() << "\"" << std::endl;
         //  std::cout << "	Material: \"" << objectMaterials[mesh->mMaterialIndex].name << "\"" << std::endl;
        //   std::cout << "	Faces: " << mesh->mNumFaces << std::endl;

        meshes[i].material = &objectMaterials[mesh->mMaterialIndex];
        meshes[i].indexBase = gIndexBase;

        std::vector<VertexModel> vertices;
        vertices.resize(mesh->mNumVertices);

        uint32_t vertexBase = gVertices.size();

        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D aiPos = mesh->mVertices[i];
            vertices[i].pos = glm::vec3(aiPos.x, aiPos.y, aiPos.z);
            vertices[i].color = glm::vec3(1.0f); // todo : take from material 
            vertices[i].tex = glm::vec2(0.0);
            if (mesh->HasTextureCoords(0)) {
                aiVector3D aiTexCoord = mesh->mTextureCoords[0][i];
                vertices[i].tex = glm::vec2(aiTexCoord.x, 1.0f - aiTexCoord.y); // flip Y 
            }
            gVertices.push_back(vertices[i]);
        }

        std::vector<uint32_t> indices;
        indices.resize(mesh->mNumFaces * 3);
        meshes[i].indexCount = indices.size();

        for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
            // Assume mesh is triangulated 
            indices[j * 3] = mesh->mFaces[j].mIndices[0];
            indices[j * 3 + 1] = mesh->mFaces[j].mIndices[1];
            indices[j * 3 + 2] = mesh->mFaces[j].mIndices[2];
            gIndices.push_back(indices[j * 3] + vertexBase);
            gIndices.push_back(indices[j * 3 + 1] + vertexBase);
            gIndices.push_back(indices[j * 3 + 2] + vertexBase);
            gIndexBase += 3;
        }

        createsVertexBuffer(meshes[i], vertices);
        createsIndexBuffer(meshes[i], indices);
    }


    std::cout << "size of vertices of the model: " << gVertices.size() << std::endl;
    std::cout << "size of indices of the model: " << gIndices.size() << std::endl;

}

void ModelLoader::createsVertexBuffer(MeshObject& mesh, std::vector<VertexModel>& vertices)
{
    uint32_t vertexDataSize = vertices.size() * sizeof(vertices[0]);
   // for (uint32_t i = 0; i < meshes.size(); i++) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = vertexDataSize;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &mesh.vertexBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer!");
        }
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, mesh.vertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = BufferManager::findingMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pyhsicalDevice);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &mesh.vertexMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        void* data;
        vkMapMemory(device, mesh.vertexMemory, 0, VK_WHOLE_SIZE, 0, &data);
        memcpy(data, vertices.data(), vertexDataSize);

        vkUnmapMemory(device, mesh.vertexMemory);
        
        vkBindBufferMemory(device, mesh.vertexBuffer, mesh.vertexMemory, 0);
  //  }

}

void ModelLoader::createsIndexBuffer(MeshObject& mesh, std::vector<uint32_t>& indices)
{
    uint32_t indexDataSize = indices.size() * sizeof(indices[0]);
   // for (uint32_t i = 0; i < meshes.size(); i++) {
        VkBufferCreateInfo bufferInfo = {
             .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
             .size = indexDataSize,
             .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
             .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &mesh.indexBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, mesh.indexBuffer, &memRequirements);

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

        if (vkAllocateMemory(device, &allocInfo, nullptr, &mesh.indexMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate index buffer memory!");
        }

        void* data;
        vkMapMemory(device, mesh.indexMemory, 0, VK_WHOLE_SIZE, 0, &data);
        memcpy(data, indices.data(), indexDataSize);
        vkUnmapMemory(device, mesh.indexMemory);
      
        vkBindBufferMemory(device, mesh.indexBuffer, mesh.indexMemory, 0);
        
  //  }
}

std::unique_ptr<Texture> ModelLoader::loadTextureFromFile(const std::string& filename, VulkanTextureManager& vkTextureManager)
{
    auto texture = std::make_unique<Texture>();;

    int texWidth, texHeight, texChannels;

   // std::string fullPath = std::string("models/sponza/") + filename.c_str();

    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Failed to load texture image: " + filename);
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    // staging buffer létrehozás + feltöltés
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    BufferManager::creatingBuffer(device, pyhsicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    stbi_image_free(pixels);

    // image + memory
    vkTextureManager.createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
         texture->image, texture->memory);

    // layout váltás → buffer copy → layout váltás
    VkCommandBuffer commandBuffer = vkTextureManager.beginSingleTimeCommands();
    vkTextureManager.transitionImageLayout(commandBuffer, texture->image, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vkTextureManager.copyBufferToImage(commandBuffer, stagingBuffer, texture->image, texWidth, texHeight);

    vkTextureManager.transitionImageLayout(commandBuffer, texture->image, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkTextureManager.endSingleTimeCommands(commandBuffer);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
    
    // image view
    texture->descriptor.imageView = vkTextureManager.createImageView(texture->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

    // sampler
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    //  samplerInfo.maxAnisotropy = 16.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    if (vkCreateSampler(device, &samplerInfo, nullptr, &texture->descriptor.sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
    texture->descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    return texture;
}
