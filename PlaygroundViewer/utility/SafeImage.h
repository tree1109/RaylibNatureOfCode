#pragma once

#ifndef INCLUDE_SAFE_IMAGE_H
#define INCLUDE_SAFE_IMAGE_H

#include <raylib.h>

class CSafeImage {
public:
    CSafeImage() = default;
    explicit CSafeImage(Image& image);
    ~CSafeImage();

    CSafeImage(const CSafeImage& other);
    CSafeImage& operator=(const CSafeImage& other);
    CSafeImage(CSafeImage&& other) noexcept;
    CSafeImage& operator=(CSafeImage&& other) noexcept;

    const Image& GetImage() const;

private:
    Image m_image;
};

#endif //INCLUDE_SAFE_IMAGE_H
