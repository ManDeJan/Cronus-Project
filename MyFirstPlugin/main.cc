#include <DynRPG/DynRPG.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <memory>
#include <fmt/core.h>
#include <assert.h>

class RPG2K3Window : public sf::Sprite
{
public:
    static constexpr size_t screenWidth = 320;
    static constexpr size_t screenHeight = 240;
    std::vector<uint32_t> pixelArray;
    sf::Texture texture;

    RPG2K3Window() : pixelArray(screenWidth * screenHeight, 0xFF00FF00)
    {
        if (!texture.create(screenWidth, screenHeight))
        {
            throw std::runtime_error("Couldn't generate texture");
        }
        sf::Sprite::setTexture(texture);
    }

    void updateTexture()
    {
        const uint16_t *buffer = RPG::screen->canvas->getScanline(screenHeight - 1);
        // No one look at this jank plz
        for (int y = screenHeight - 1; y >= 0; y--)
        {
            for (int x = 0; x < screenWidth; x++)
            {
                pixelArray[x + (screenHeight - 1 - y) * screenWidth] = RPG::screen->canvas->convert16To32Bit(*(buffer + y * screenWidth + x));
            }
        }

        // std::transform(buffer,
        //                buffer + (screenHeight * screenWidth),
        //                pixelArray.begin(),
        //                [](uint16_t input) { return RPG::screen->canvas->convert16To32Bit(input); });

        texture.update((uint8_t *)pixelArray.data());
    }

    void setTexture(const sf::Texture &texture, bool resetRect = false) = delete;
    void setTextureRect(const sf::IntRect &rectangle) = delete;
};

std::unique_ptr<sf::RenderWindow> window;
std::unique_ptr<RPG2K3Window> rpg;

// Handler called on startup
bool onStartup(char *pluginName)
{

    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);
    sf::sleep(sf::milliseconds(250));

    // don't mind this thanks
    // Do not remove or game won't launch.
    volatile short *magic_address = reinterpret_cast<volatile short *>(0x48FA57);
    *magic_address = 0x9090;

    if (MessageBox(NULL,                                                 // We don't need a window handle
                   "This is a totally normal RPG Maker game, continue?", // Text
                   "He cometh. In anger.",                               // Title
                   MB_YESNO |
                       MB_ICONQUESTION // Flags (yes/no buttons and question icon)
                   ) == IDYES)
    {
        // The user clicked "Yes", we may continue
        return true;
    }
    else
    {
        // The user clicked "No", so we need to abort
        return false;
    }
}

void onDrawScreen()
{
    rpg->updateTexture();
    window->draw(*rpg);
    window->display();
}

void onInitFinished()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 480), "Cronus");
    rpg = std::make_unique<RPG2K3Window>();
}