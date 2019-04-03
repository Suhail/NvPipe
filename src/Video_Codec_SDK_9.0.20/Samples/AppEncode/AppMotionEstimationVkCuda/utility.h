/*
* Copyright 2018 NVIDIA Corporation.  All rights reserved.
*
* Please refer to the NVIDIA end user license agreement (EULA) associated
* with this source code for terms and conditions that govern your use of
* this software. Any use, reproduction, disclosure, or distribution of
* this software and related documentation outside the terms of the EULA
* is strictly prohibited.
*
*/

#include <vulkan/vulkan.h>
#include "cuda.h"
#include <vector>
#include <array>

class Vkinst
{
    VkInstance m_instance;
    VkDebugReportCallbackEXT m_callback;
    std::vector<VkPhysicalDevice> m_physicalDevices;

public:
    Vkinst(
        const std::vector<const char*>& layers = std::vector<const char*>(),
        const std::vector<const char*>& extensions = std::vector<const char*>()
    );
    ~Vkinst();

    const std::vector<VkPhysicalDevice>& getPhysicalDevices(void) const
    {
        return m_physicalDevices;
    }

    VkInstance get(void) const
    {
        return m_instance;
    }
};

class Vkcmdbuffer;
class Vksema;

class Vkque
{
    VkQueue m_queue;

    VkResult submit(
        const std::vector<VkSemaphore>& waitSemaphores = std::vector<VkSemaphore>(),
        const std::vector<VkCommandBuffer>& commandBuffers = std::vector<VkCommandBuffer>(),
        const std::vector<VkSemaphore>& signalSemaphores = std::vector<VkSemaphore>()
    );

public:
    Vkque(const VkQueue queue)
    {
        m_queue = queue;
    }

    ~Vkque(){}

    VkResult submit(const Vkcmdbuffer *commandBuffer);

    VkResult submit(
        const Vkcmdbuffer *commandBuffer,
        const Vksema *signalSemaphore
    );

    VkResult submit(
        const Vksema *waitSemaphore,
        const Vkcmdbuffer *commandBuffer,
        const Vksema *signalSemaphore
    );

    VkResult waitIdle(void);

    VkQueue get() const
    {
        return m_queue;
    }
};

class Vkdev
{
    VkDevice m_device;

    uint32_t m_transferQueueFamilyIndex;
    VkQueue m_transferQueue;

    VkPhysicalDeviceMemoryProperties m_deviceMemProps;

    std::array<uint8_t, VK_UUID_SIZE> m_deviceUUID;

public:
    Vkdev(
        const Vkinst *instance,
        const std::vector<const char*>& deviceExtensions = std::vector<const char*>()
    );
    ~Vkdev();

    uint32_t getTransferQueueFamilyIndex(void) const
    {
        return m_transferQueueFamilyIndex;
    }

    const VkPhysicalDeviceMemoryProperties& getMemoryProperties(void) const
    {
        return m_deviceMemProps;
    }

    const Vkque getTransferQueue(void) const
    {
        const static Vkque transferQueue(m_transferQueue);
        return transferQueue;
    }

    const std::array<uint8_t, VK_UUID_SIZE> getUUID(void) const
    {
        return m_deviceUUID;
    }

    VkDevice get() const
    {
        return m_device;
    }
};

class Vkcmdpool
{
    VkCommandPool m_commandPool;
    VkDevice m_device;

public:
    Vkcmdpool(const Vkdev *device);
    ~Vkcmdpool();

    VkCommandPool get() const
    {
        return m_commandPool;
    }
};

class Vkdevicemem;

class Vkbuf
{
    VkBuffer m_buffer;
    VkDevice m_device;

    VkDeviceSize m_size;
    VkDeviceSize m_alignment;
    uint32_t m_memoryTypeBits;

public:
    Vkbuf(
        const Vkdev *device, VkDeviceSize bufferSize,
        VkBufferUsageFlags bufferUsage, bool exportCapable = false
    );
    ~Vkbuf();

    void bind(const Vkdevicemem *deviceMem, VkDeviceSize offset = 0);

    VkDeviceSize getSize(void)
    {
        return m_size;
    }

    uint32_t getMemoryTypeBits(void)
    {
        return m_memoryTypeBits;
    }

    VkBuffer get() const
    {
        return m_buffer;
    }
};

class Vkimg2d
{
    VkImage m_image;
    VkDevice m_device;

    VkExtent2D m_extent;
    VkDeviceSize m_size;
    VkDeviceSize m_alignment;
    uint32_t m_memoryTypeBits;

public:
    Vkimg2d(
        const Vkdev *device, VkExtent2D extent, VkImageUsageFlags imageUsage,
        bool exportCapable = false
    );
    ~Vkimg2d();

    void bind(const Vkdevicemem *deviceMem, VkDeviceSize offset = 0);

    VkDeviceSize getSize(void) const
    {
        return m_size;
    }

    VkDeviceSize getAlignment(void) const
    {
        return m_alignment;
    }

    VkExtent2D getExtent(void) const
    {
        return m_extent;
    }

    uint32_t getMemoryTypeBits(void)
    {
        return m_memoryTypeBits;
    }

    VkImage get() const
    {
        return m_image;
    }
};

class Vkdevicemem
{
    VkDeviceMemory m_deviceMemory;
    VkDevice m_device;
    VkDeviceSize m_size;

public:
    Vkdevicemem(
        const Vkdev *device, VkDeviceSize size, uint32_t memoryTypeBits,
        VkMemoryPropertyFlags memoryProperties, bool exportCapable = false
    );
    ~Vkdevicemem();

    VkResult map(
        void **p, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0
    );

    void unmap(void);

    void *getExportHandle(void) const;

    const VkDeviceMemory getMemory(void) const
    {
        return m_deviceMemory;
    }

    VkDeviceSize getSize(void) const
    {
        return m_size;
    }

    VkDeviceMemory get() const
    {
        return m_deviceMemory;
    }
};

class Vkimgmembarrier;

class Vkcmdbuffer
{
    VkCommandBuffer m_commandBuffer;
    VkDevice m_device;
    VkCommandPool m_commandPool;

public:
    Vkcmdbuffer(const Vkdev *device, const Vkcmdpool *commandPool);
    ~Vkcmdbuffer();

    VkResult begin(void);
    VkResult end(void);

    void fillBuffer(const Vkbuf *buffer, uint32_t data,
        VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    void copyBuffer(const Vkbuf *dstBuffer, const Vkbuf *srcBuffer,
        VkDeviceSize size = VK_WHOLE_SIZE);

    void pipelineBarrier(
        const Vkimgmembarrier *imageBarrier,
        VkImageLayout oldLayout,
        VkImageLayout newLayout,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask
    );

    void clearImage(const Vkimg2d *image, VkClearColorValue value);

    void copyImageToBuffer(const Vkbuf *buffer, const Vkimg2d *image);

    void copyBufferToImage(const Vkimg2d *image, const Vkbuf *buffer);

    VkCommandBuffer get() const
    {
        return m_commandBuffer;
    }
};

class Vksema
{
    VkSemaphore m_semaphore;
    VkDevice m_device;

public:
    Vksema(const Vkdev *device, bool exportCapable = false);
    ~Vksema();

    void *getExportHandle(void) const;

    VkSemaphore get() const
    {
        return m_semaphore;
    }
};

class Vkimgmembarrier
{
    VkImageMemoryBarrier m_barrier;

public:
    Vkimgmembarrier(const Vkimg2d *image);
    ~Vkimgmembarrier(){};

    VkImageMemoryBarrier get() const
    {
        return m_barrier;
    }
};

class Cudactx
{
    CUcontext m_context;

public:
    Cudactx(const Vkdev *device);
    ~Cudactx(){};

    CUresult memcpyDtoH(void *p, CUdeviceptr dptr, uint64_t size);

    CUresult memcpy2D(void *p, CUarray array, uint32_t width, uint32_t height);

    CUcontext get() const
    {
        return m_context;
    }
};

class Cudabuffer
{
    CUdeviceptr m_deviceptr;
    CUexternalMemory m_extMem;

public:
    Cudabuffer(const Vkdevicemem *deviceMem);
    ~Cudabuffer();

    CUdeviceptr get() const
    {
        return m_deviceptr;
    }
};

class Cudaimage
{
    CUarray m_array;
    CUmipmappedArray m_mipmapArray;
    CUexternalMemory m_extMem;

public:
    Cudaimage(const Vkimg2d *image, const Vkdevicemem *deviceMem);
    ~Cudaimage();

    CUarray get() const
    {
        return m_array;
    }
};

class Cudasema
{
    CUexternalSemaphore m_extSema;

public:
    Cudasema(const Vksema *semaphore);
    ~Cudasema();

    CUresult wait(void);
    CUresult signal(void);
};
