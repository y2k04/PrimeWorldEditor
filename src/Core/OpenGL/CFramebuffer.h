#ifndef CFRAMEBUFFER_H
#define CFRAMEBUFFER_H

#include <GL/glew.h>
#include <cstdint>

class CRenderbuffer;
class CTexture;

class CFramebuffer
{
    GLuint mFramebuffer = 0;
    CRenderbuffer *mpRenderbuffer = nullptr;
    CTexture *mpTexture = nullptr;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    bool mEnableMultisampling = false;
    bool mInitialized = false;
    GLenum mStatus{};

    static GLint smDefaultFramebuffer;
    static bool smStaticsInitialized;

public:
    CFramebuffer();
    CFramebuffer(uint32_t Width, uint32_t Height);
    ~CFramebuffer();

    void Init();
    void Bind(GLenum Target = GL_FRAMEBUFFER);
    void Resize(uint32_t Width, uint32_t Height);
    void SetMultisamplingEnabled(bool Enable);

    // Accessors
    CTexture* Texture() const { return mpTexture; }

    // Static
    static void BindDefaultFramebuffer(GLenum Target = GL_FRAMEBUFFER);

protected:
    void InitBuffers();
};

#endif // CFRAMEBUFFER_H
