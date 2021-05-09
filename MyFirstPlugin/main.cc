#include "DynRPG/DynCore/PluginCallbacks.hpp"
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

// Handler called on startup
bool onStartup(char *pluginName)
{
    // don't mind this thanks
    // Do not remove or game won't launch.
    volatile short *magic_address = reinterpret_cast<volatile short *>(0x48FA57);
    *magic_address = 0x9090;

    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);
    sf::sleep(sf::milliseconds(250));

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

class DvdBouncer
{
    sf::Transformable &obj;
    sf::Vector2f direction = {1, 1};
    sf::Vector2i objSize;
    sf::Vector2i windowSize;

public:
    DvdBouncer(sf::Transformable &obj, sf::Vector2i objSize, sf::Vector2i windowSize) : obj(obj), objSize(objSize), windowSize(windowSize)
    {
    }

    void update()
    {
        if (obj.getPosition().x + objSize.x * obj.getScale().x > windowSize.x || obj.getPosition().x < 0)
        {
            direction.x *= -1;
            fmt::print("Flip\n");
        }
        if (obj.getPosition().y + objSize.y * obj.getScale().y > windowSize.y || obj.getPosition().y < 0)
        {
            direction.y *= -1;
            fmt::print("Flip\n");
        }
        obj.move(direction);
    }
};

std::unique_ptr<DvdBouncer> bouncer;
std::unique_ptr<sf::RenderWindow> window;
std::unique_ptr<RPG2K3Window> rpg;
std::unique_ptr<sf::Texture> backgroundTexture;
std::unique_ptr<sf::Sprite> background;

void onFrame(RPG::Scene scene) {
    bouncer->update();
}

void onDrawScreen()
{
    window->clear();

    rpg->updateTexture();
    // bouncer->update();

    // window->draw(*background);
    window->draw(*rpg);
    window->display();
}

void onInitTitleScreen() {
    HWND winContext = GetParent(GetParent(RPG::screen->getCanvasHWND()));
    ShowWindow(winContext, SW_HIDE);
}

void onInitFinished()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(620 * 2, 480 * 2), "Cronus", sf::Style::Fullscreen);
    rpg = std::make_unique<RPG2K3Window>();
    const auto window_size = window->getSize();
    const auto scaling_factor = std::min(
        (float)window_size.x / rpg->screenWidth,
        (float)window_size.y / rpg->screenHeight);
    rpg->setScale(scaling_factor, scaling_factor);
    rpg->setPosition({(window_size.x - rpg->screenWidth * scaling_factor) / 2,
                      (window_size.y - rpg->screenHeight * scaling_factor) / 2});
    // bouncer = std::make_unique<DvdBouncer>(*rpg, sf::Vector2i{320, 240}, sf::Vector2i{640, 480});
    // backgroundTexture = std::make_unique<sf::Texture>();
    // backgroundTexture->loadFromFile("background.png");
    // background = std::make_unique<sf::Sprite>(*backgroundTexture);
}