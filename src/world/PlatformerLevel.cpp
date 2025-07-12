#include "world/PlatformerLevel.hpp"

PlatformerLevel::PlatformerLevel(){
    // Ground platform
    platforms.push_back(sf::RectangleShape({1200.f, 50.f}));
    platforms.back().setPosition({0.f, 700.f});

    // Floating platforms
    platforms.push_back(sf::RectangleShape({200.f, 30.f}));
    platforms.back().setPosition({300.f, 550.f});

    platforms.push_back(sf::RectangleShape({200.f, 30.f}));
    platforms.back().setPosition({750.f, 400.f});
    
    platforms.push_back(sf::RectangleShape({150.f, 30.f}));
    platforms.back().setPosition({50.f, 250.f});

    platforms.push_back(sf::RectangleShape({200.f, 30.f}));
    platforms.back().setPosition({320.f, 333.f});

    // color for all platforms
    for (auto& p : platforms) {
        p.setFillColor(sf::Color(139, 69, 19)); // Brown
    }

    // Spikes
    spikes.push_back(sf::RectangleShape({100.f, 20.f}));
    spikes.back().setPosition({450.f, 680.f});
    spikes.back().setFillColor(sf::Color::Red);

    // End door
    endDoor.setSize({50.f, 80.f});
    endDoor.setPosition({80.f, 170.f});
    endDoor.setFillColor(sf::Color::Cyan);
}

void PlatformerLevel::render(sf::RenderWindow& window){
    for(const auto& p : platforms){
        window.draw(p);
    }
    for(const auto& s :spikes){
        window.draw(s);
    }
    window.draw(endDoor);
}
const std::vector<sf::RectangleShape>& PlatformerLevel::getPlatforms() const {
    return platforms;
}
const std::vector<sf::RectangleShape>& PlatformerLevel::getSpikes() const {
    return spikes;
}
const sf::RectangleShape& PlatformerLevel::getEndDoor() const {
    return endDoor;
}