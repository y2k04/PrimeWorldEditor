#ifndef CUNIFORMBUFFER_H
#define CUNIFORMBUFFER_H

#include <cstdint>
#include <GL/glew.h>

class CUniformBuffer
{
    GLuint mUniformBuffer;
    uint32_t mBufferSize;

public:

    CUniformBuffer()
    {
        glGenBuffers(1, &mUniformBuffer);
        SetBufferSize(0);
    }

    explicit CUniformBuffer(uint32_t Size)
    {
        glGenBuffers(1, &mUniformBuffer);
        SetBufferSize(Size);
    }

    ~CUniformBuffer()
    {
        glDeleteBuffers(1, &mUniformBuffer);
    }

    void Bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
    }

    void Unbind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void BindBase(GLuint Index)
    {
        Bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, Index, mUniformBuffer);
        Unbind();
    }

    void Buffer(const void *pkData)
    {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, mBufferSize, pkData);
        Unbind();
    }

    void BufferRange(const void *pkData, uint32_t Offset, uint32_t Size)
    {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, Offset, Size, pkData);
        Unbind();
    }

    void SetBufferSize(uint32_t Size)
    {
        mBufferSize = Size;
        InitializeBuffer();
    }

    uint32_t GetBufferSize() const
    {
        return mBufferSize;
    }

private:
    void InitializeBuffer()
    {
        Bind();
        glBufferData(GL_UNIFORM_BUFFER, mBufferSize, nullptr, GL_DYNAMIC_DRAW);
        Unbind();
    }
};

#endif // CUNIFORMBUFFER_H
