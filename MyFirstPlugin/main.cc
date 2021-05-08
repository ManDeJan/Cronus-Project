#include <DynRPG/DynRPG.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <fmt/core.h>
#include <random>
#include <assert.h>

std::random_device rd;
std::minstd_rand0 rg(rd());
std::uniform_int_distribution uint_dist(0, 255);
class RPG2K3Window
{
public:
    sf::RenderWindow window;

    const size_t screenWidth;
    const size_t screenHeight;
    std::vector<uint32_t> pixelArray;
    sf::Texture texture;
    sf::Sprite sprite;

    RPG2K3Window() : window(sf::VideoMode(640, 480), "Cronus"),
                     screenWidth(RPG::screen->canvas->width()),
                     screenHeight(RPG::screen->canvas->height()),
                     pixelArray(screenWidth * screenHeight, 0xFF00FF00),
                     sprite()
    {
        if (!texture.create(RPG::screen->canvas->width(), RPG::screen->canvas->height()))
        {
            throw std::runtime_error("Couldn't generate texture");
        }
        sprite.setTexture(texture);
        sprite.setOrigin(screenWidth / 2, screenHeight / 2);
        sprite.setPosition(640 / 2, 480 / 2);
        sprite.setScale(640.0 / screenWidth, -1 * (480.0 / screenHeight));
        fmt::print("Sprite scale: {}x{}", sprite.getScale().x, sprite.getScale().y);
    }

    void onDraw()
    {
        assert(RPG::screen->canvas->getScanline(0) > RPG::screen->canvas->getScanline(screenHeight - 1));
        const uint16_t *buffer = RPG::screen->canvas->getScanline(screenHeight - 1);
        std::transform(buffer,
                       buffer + (screenHeight * screenWidth),
                       pixelArray.begin(),
                       [](uint16_t input) { return RPG::screen->canvas->convert16To32Bit(input); });

        texture.update((uint8_t *)pixelArray.data());
        window.draw(sprite);
        window.display();
    };
};

std::unique_ptr<RPG2K3Window> window;

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
    window->onDraw();
}

void onInitFinished()
{
    window = std::make_unique<RPG2K3Window>();
}