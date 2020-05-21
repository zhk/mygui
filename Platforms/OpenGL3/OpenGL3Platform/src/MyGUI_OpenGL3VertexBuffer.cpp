/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include "MyGUI_OpenGL3VertexBuffer.h"
#include "MyGUI_OpenGL3Diagnostic.h"

#include <GL/glew.h>

namespace MyGUI
{

	const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;

	OpenGL3VertexBuffer::OpenGL3VertexBuffer() :
		mVAOID(0),
		mBufferID(0),
		mVertexCount(0),
		mNeedVertexCount(0),
		mSizeInBytes(0)
	{
	}

	OpenGL3VertexBuffer::~OpenGL3VertexBuffer()
	{
		destroy();
	}

	void OpenGL3VertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t OpenGL3VertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* OpenGL3VertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount || mVertexCount == 0)
			resize();

		MYGUI_PLATFORM_ASSERT(mBufferID, "Vertex buffer in not created");

		// Use glMapBuffer
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);

		// Discard the buffer
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GL_STREAM_DRAW);

		Vertex* pBuffer = reinterpret_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		MYGUI_PLATFORM_ASSERT(pBuffer, "Error lock vertex buffer");

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return pBuffer;
	}

	void OpenGL3VertexBuffer::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mBufferID, "Vertex buffer in not created");

		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		GLboolean result = glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		MYGUI_PLATFORM_ASSERT(result, "Error unlock vertex buffer");
	}

	void OpenGL3VertexBuffer::create()
	{
		MYGUI_PLATFORM_ASSERT(!mBufferID, "Vertex buffer already exist");

		mSizeInBytes = mVertexCount * sizeof(Vertex);
		void* data = nullptr;

		glGenBuffers(1, &mBufferID);
		glGenVertexArrays(1, &mVAOID);

		glBindVertexArray(mVAOID);
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, data, GL_STREAM_DRAW);

		// check data size in VBO is same as input array, if not return 0 and delete VBO
		int bufferSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
		if (mSizeInBytes != (size_t)bufferSize)
		{
			destroy();
			MYGUI_PLATFORM_EXCEPT("Data size is mismatch with input array");
		}

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)nullptr);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLubyte*)offsetof(struct Vertex, colour));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)offsetof(struct Vertex, u));

		//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offset);
		//offset += (sizeof(float) * 3);
		//glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offset);
		//offset += (4);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offset);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGL3VertexBuffer::destroy()
	{
		if (mBufferID != 0)
		{
			glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
		if (mVAOID != 0)
		{
			glDeleteVertexArrays(1, &mVAOID);
			mVAOID = 0;
		}
	}

	void OpenGL3VertexBuffer::resize()
	{
		mVertexCount = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		destroy();
		create();
	}

} // namespace MyGUI
