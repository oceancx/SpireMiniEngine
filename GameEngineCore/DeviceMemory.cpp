#include "DeviceMemory.h"
#include "HardwareRenderer.h"
#include "LibMath.h"

namespace GameEngine
{
	void DeviceMemory::Init(HardwareRenderer * hwRenderer, BufferUsage usage, int log2BufferSize, int alignment)
	{
		buffer = hwRenderer->CreateBuffer(usage, 1 << log2BufferSize);
		bufferPtr = new unsigned char[(int)(1 << log2BufferSize)];
		int logAlignment = CoreLib::Math::Log2Ceil(alignment);
		memory.Init((unsigned char*)bufferPtr, logAlignment, (1 << (log2BufferSize - logAlignment)));
	}

	DeviceMemory::~DeviceMemory()
	{
		delete[] bufferPtr;
	}

	void * DeviceMemory::Alloc(int size)
	{
		return memory.Alloc(size);
	}

	void DeviceMemory::Free(void * ptr, int size)
	{
		memory.Free((unsigned char*)ptr, size);
	}

	void DeviceMemory::Sync(void * ptr, int size)
	{
		buffer->SetData((int)((unsigned char*)ptr - bufferPtr), ptr, size);
	}

}

