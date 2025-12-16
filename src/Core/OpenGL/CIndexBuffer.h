#ifndef CINDEXBUFFER_H
#define CINDEXBUFFER_H

#include <GL/glew.h>
#include <cstdint>
#include <vector>

class CIndexBuffer
{
    GLuint mIndexBuffer = 0;
    std::vector<uint16_t> mIndices;
    GLenum mPrimitiveType{};
    bool mBuffered = false;

public:
    CIndexBuffer();
    explicit CIndexBuffer(GLenum type);
    ~CIndexBuffer();
    void AddIndex(uint16_t index);
    void AddIndices(const uint16_t *indices, size_t count);
    void Reserve(size_t size);
    void Clear();
    void Buffer();
    void Bind();
    void Unbind();
    void DrawElements();
    void DrawElements(uint32_t offset, uint32_t size);
    bool IsBuffered() const;

    uint32_t GetSize() const;
    GLenum GetPrimitiveType() const;
    void SetPrimitiveType(GLenum type);

    void TrianglesToStrips(uint16_t *indices, size_t count);
    void FansToStrips(uint16_t *indices, size_t count);
    void QuadsToStrips(uint16_t *indices, size_t count);
};

#endif // CINDEXBUFFER_H
