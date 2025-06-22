#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "../src/avl_tree.h"
#include "../src/splay_tree.h"
#include "../src/dancing_tree.h"
#include "../src/tree_structure.h"

enum class Screen {
    MainMenu,
    TreeScreen,
    InputKey
};

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;
    bool hovered = false;
    Button(const std::string& str, const sf::Font& font, unsigned int size)
        : text(font, str, size) {}
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1280u, 800u}), "Tree Visualizer");
    sf::Font font;
    if (!font.openFromFile("GOTHICB.ttf")) return 1;

    Screen currentScreen = Screen::MainMenu;
    std::unique_ptr<TreeStructure> currentTree;
    std::string selectedTreeType;
    std::string inputKey, inputValue, message;
    float messageTimer = 0.f;

    std::vector<std::string> treeTypes = {"AVL Tree", "Splay Tree", "Dancing Tree"};
    int mainMenuSelected = 0;

    std::vector<Button> opButtons;

    auto setupTree = [&](const std::string& type) {
        if (type == "AVL Tree") currentTree = std::make_unique<AVLTree>();
        else if (type == "Splay Tree") currentTree = std::make_unique<SplayTree>();
        else if (type == "Dancing Tree") currentTree = std::make_unique<DancingTree>();
        selectedTreeType = type;
        currentScreen = Screen::TreeScreen;
        message.clear();
    };

    auto createOpButtons = [&]() {
        opButtons.clear();
        std::vector<std::string> ops = {"Insert", "Remove", "Search", "Fill Random", "Clear", "Back"};
        for (size_t i = 0; i < ops.size(); ++i) {
            Button btn(ops[i], font, 24);
            btn.shape = sf::RectangleShape(sf::Vector2f(180, 50));
            btn.shape.setPosition(sf::Vector2f(1040.f, 80.f + i * 70.f));
            btn.shape.setFillColor(sf::Color(200, 220, 255));
            btn.shape.setOutlineColor(sf::Color::Black);
            btn.shape.setOutlineThickness(2.f);
            btn.text.setFillColor(sf::Color::Black);
            btn.text.setPosition(sf::Vector2f(1055.f, 90.f + i * 70.f));
            // обработка кнопок
            if (ops[i] == "Insert") {
                btn.onClick = [&]() { currentScreen = Screen::InputKey; inputKey.clear(); inputValue.clear(); message = "Enter key and value"; };
            } else if (ops[i] == "Remove") {
                btn.onClick = [&]() { currentScreen = Screen::InputKey; inputKey.clear(); inputValue.clear(); message = "Enter key to remove"; };
            } else if (ops[i] == "Search") {
                btn.onClick = [&]() { currentScreen = Screen::InputKey; inputKey.clear(); inputValue.clear(); message = "Enter key to search"; };
            } else if (ops[i] == "Fill Random") {
                btn.onClick = [&]() { for (int i = 0; i < 20; ++i) currentTree->insert(rand()%1000, rand()%1000); message = "Filled with random values"; messageTimer = 2.f; };
            } else if (ops[i] == "Clear") {
                btn.onClick = [&]() { currentTree->display(); currentTree.reset(); setupTree(selectedTreeType); message = "Tree cleared"; messageTimer = 2.f; };
            } else if (ops[i] == "Back") {
                btn.onClick = [&]() { currentScreen = Screen::MainMenu; };
            }
            opButtons.push_back(btn);
        }
    };

    createOpButtons();

    std::function<void(const AVLNode*, float, float, float)> drawAVL;

    drawAVL = [&](const AVLNode* node, float x, float y, float xOffset) {
        if (!node) return;
        if (node->left) {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = sf::Vector2f(x, y);
            line[1].position = sf::Vector2f(x - xOffset, y + 80);
            window.draw(line);
            drawAVL(node->left, x - xOffset, y + 80, xOffset / 2.f);
        }
        if (node->right) {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            line[0].position = sf::Vector2f(x, y);
            line[1].position = sf::Vector2f(x + xOffset, y + 80);
            window.draw(line);
            drawAVL(node->right, x + xOffset, y + 80, xOffset / 2.f);
        }
        sf::CircleShape circle(24);
        circle.setOrigin(sf::Vector2f(24.f, 24.f));
        circle.setPosition(sf::Vector2f(x, y));
        circle.setFillColor(sf::Color(200, 220, 255));
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(2.f);
        window.draw(circle);
        sf::Text keyText(font, std::to_string(node->key), 20);
        keyText.setFillColor(sf::Color::Black);
        keyText.setPosition(sf::Vector2f(x - 12.f, y - 16.f));
        window.draw(keyText);
    };

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        std::optional<sf::Event> event;
        while (event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (currentScreen == Screen::MainMenu) {
                if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Up)
                        mainMenuSelected = (mainMenuSelected + treeTypes.size() - 1) % treeTypes.size();
                    if (key->code == sf::Keyboard::Key::Down)
                        mainMenuSelected = (mainMenuSelected + 1) % treeTypes.size();
                    if (key->code == sf::Keyboard::Key::Enter)
                        setupTree(treeTypes[mainMenuSelected]);
                }
            } else if (currentScreen == Screen::TreeScreen) {
                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        auto mousePos = sf::Vector2f(mouse->position.x, mouse->position.y);
                        for (auto& btn : opButtons) {
                            if (btn.shape.getGlobalBounds().contains(mousePos)) btn.onClick();
                        }
                    }
                }
            } else if (currentScreen == Screen::InputKey) {
                if (auto* text = event->getIf<sf::Event::TextEntered>()) {
                    if (std::isdigit(text->unicode)) inputKey += static_cast<char>(text->unicode);
                    if (text->unicode == '\b' && !inputKey.empty()) inputKey.pop_back();
                    if (text->unicode == '\r') {
                        // обработка вставки/удаления/поиска
                        // ...
                        currentScreen = Screen::TreeScreen;
                    }
                }
            }
        }

        window.clear(sf::Color(7, 107, 148));

        if (currentScreen == Screen::MainMenu) {
            for (size_t i = 0; i < treeTypes.size(); ++i) {
                sf::Text text(font, treeTypes[i], 40);
                text.setPosition(sf::Vector2f(100.f, 100.f + i * 60.f));
                text.setFillColor(i == mainMenuSelected ? sf::Color::Yellow : sf::Color(224, 247, 198));
                window.draw(text);
            }
        } else if (currentScreen == Screen::TreeScreen) {
            // Левая часть — дерево
            sf::RectangleShape leftBg(sf::Vector2f(900, 800));
            leftBg.setPosition(sf::Vector2f(0.f, 0.f));
            leftBg.setFillColor(sf::Color(220, 240, 255));
            window.draw(leftBg);

            // Правая часть — панель
            sf::RectangleShape rightBg(sf::Vector2f(380, 800));
            rightBg.setPosition(sf::Vector2f(900.f, 0.f));
            rightBg.setFillColor(sf::Color(180, 210, 255));
            window.draw(rightBg);

            // Кнопки
            for (auto& btn : opButtons) {
                window.draw(btn.shape);
                window.draw(btn.text);
            }

            // Визуализация дерева (пример для AVL)
            if (selectedTreeType == "AVL Tree") {
                auto* avl = dynamic_cast<AVLTree*>(currentTree.get());
                if (avl && avl->getRoot()) drawAVL(avl->getRoot(), 450, 100, 200);
            }
            // Аналогично для других деревьев
        }

        // Сообщения
        if (messageTimer > 0.f && !message.empty()) {
            sf::RectangleShape notifBg(sf::Vector2f(400, 50));
            notifBg.setPosition(sf::Vector2f(440.f, 720.f));
            notifBg.setFillColor(sf::Color(30, 30, 30, 200));
            notifBg.setOutlineColor(sf::Color(200, 200, 200));
            notifBg.setOutlineThickness(2.f);
            window.draw(notifBg);
            sf::Text notifText(font, message, 24);
            notifText.setFillColor(sf::Color::White);
            notifText.setPosition(sf::Vector2f(460.f, 730.f));
            window.draw(notifText);
            messageTimer -= dt;
            if (messageTimer < 0.f) messageTimer = 0.f;
        }

        window.display();
    }
    return 0;
}