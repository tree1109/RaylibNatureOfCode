#include "SafeImage.h"

#include <stdexcept>

CSafeImage::CSafeImage(Image& image)
{
    if (!image.data) {
        throw std::runtime_error("Invalid image data provided.");
    }

    // Move the image into the safe image object.
    m_image = image;
    image = Image();
}

CSafeImage::~CSafeImage()
{
    UnloadImage(m_image);
    m_image = Image(); // Reset the image to ensure no dangling references.
}

CSafeImage::CSafeImage(const CSafeImage& other)
{
    m_image = ImageCopy(other.m_image);
}

CSafeImage& CSafeImage::operator=(const CSafeImage& other)
{
    UnloadImage(m_image);
    m_image = ImageCopy(other.m_image);
    return *this;
}

CSafeImage::CSafeImage(CSafeImage&& other) noexcept
{
    m_image = other.m_image;
    other.m_image = Image();
}

CSafeImage& CSafeImage::operator=(CSafeImage&& other) noexcept
{
    UnloadImage(m_image);
    m_image = other.m_image;
    other.m_image = Image();
    return *this;
}

const Image& CSafeImage::GetImage() const
{
    return m_image;
}
