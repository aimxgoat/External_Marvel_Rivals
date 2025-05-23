
#include <cstdint>
#include <d3d9.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
public:
    Texture(IDirect3DDevice9* pDevice) : pDevice(pDevice), texture(nullptr), image_width(0), image_height(0) {}

    bool LoadTextureFromFile(const char* filename)
    {
        stbi_set_flip_vertically_on_load_thread(false);
        std::uint8_t* image_data = stbi_load(filename, &image_width, &image_height, nullptr, STBI_rgb_alpha);
        if (!image_data)
            return false;

        IDirect3DTexture9* temp_texture = nullptr;
        if (pDevice->CreateTexture(image_width, image_height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &temp_texture, nullptr) != D3D_OK)
        {
            stbi_image_free(image_data);
            return false;
        }

        D3DLOCKED_RECT locked_rect;
        if (temp_texture->LockRect(0, &locked_rect, nullptr, D3DLOCK_DISCARD) != D3D_OK)
        {
            temp_texture->Release();
            stbi_image_free(image_data);
            return false;
        }

        for (int y = 0; y < image_height; ++y)
        {
            const auto src = reinterpret_cast<const std::uint32_t*>(image_data + image_width * 4 * y);
            const auto dest = reinterpret_cast<std::uint32_t*>(static_cast<unsigned char*>(locked_rect.pBits) + locked_rect.Pitch * y);

            for (int i = 0; i < image_width; ++i)
            {
                auto color = src[i];
                color = (color & 0xFF00FF00) | ((color & 0xFF0000) >> 16) | ((color & 0xFF) << 16);
                dest[i] = color;
            }
        }

        temp_texture->UnlockRect(0);

        if (pDevice->CreateTexture(image_width, image_height, 1, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr) != D3D_OK)
        {
            temp_texture->Release();
            stbi_image_free(image_data);
            return false;
        }

        pDevice->UpdateTexture(temp_texture, texture);
        temp_texture->Release();
        stbi_image_free(image_data);

        return true;
    }

    std::int32_t GetWidth() const { return image_width; }
    std::int32_t GetHeight() const { return image_height; }

    void Destroy()
    {
        if (texture)
        {
            reinterpret_cast<IDirect3DTexture9*>(texture)->Release();
            texture = nullptr;
        }
    }

    ImTextureID GetTexture() const { return texture; }

private:
    IDirect3DDevice9* pDevice;
    IDirect3DTexture9* texture;
    std::int32_t image_width;
    std::int32_t image_height;
};