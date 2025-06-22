#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <optional>
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

// --- Инфиксное позиционирование для компактной отрисовки дерева ---
struct DrawNode {
    float x, y;
    int key, value;
    DrawNode* left = nullptr;
    DrawNode* right = nullptr;
};

template<typename NodeT>
void assignPositions(const NodeT* node, float depth, float& x, std::vector<DrawNode>& nodes) {
    if (!node) return;
    assignPositions(node->left, depth + 1, x, nodes);
    nodes.push_back({x * 50 + 100, depth * 80 + 100, node->key, node->value});
    x += 1;
    assignPositions(node->right, depth + 1, x, nodes);
}

template<typename NodeT>
void drawTreeCompact(const NodeT* node, sf::RenderWindow& window, const sf::Font& font) {
    std::vector<DrawNode> nodes;
    float x = 0;
    assignPositions(node, 0, x, nodes);

    // Нарисовать линии между родителем и детьми
    for (size_t i = 0; i < nodes.size(); ++i) {
        const DrawNode& n = nodes[i];
        // Найти детей (по координатам)
        for (size_t j = 0; j < nodes.size(); ++j) {
            if (nodes[j].y == n.y + 80) {
                if (nodes[j].x < n.x) {
                    // левый потомок
                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    line[0].position = sf::Vector2f(n.x, n.y);
                    line[1].position = sf::Vector2f(nodes[j].x, nodes[j].y);
                    window.draw(line);
                } else if (nodes[j].x > n.x) {
                    // правый потомок
                    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                    line[0].position = sf::Vector2f(n.x, n.y);
                    line[1].position = sf::Vector2f(nodes[j].x, nodes[j].y);
                    window.draw(line);
                }
            }
        }
    }

    // Нарисовать узлы
    for (const auto& n : nodes) {
        sf::CircleShape circle(24);
        circle.setOrigin(sf::Vector2f(24.f, 24.f));
        circle.setPosition(sf::Vector2f(n.x, n.y));
        circle.setFillColor(sf::Color(200, 220, 255));
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(2.f);
        window.draw(circle);
        sf::Text keyText(font, std::to_string(n.key) + ":" + std::to_string(n.value), 20);
        keyText.setFillColor(sf::Color::Black);
        keyText.setPosition(sf::Vector2f(n.x - 18.f, n.y - 16.f));
        window.draw(keyText);
    }
}

int run() {
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

    sf::Clock clock;
    bool inputForInsert = false;

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
                    if (message.find("value") != std::string::npos) {
                        // Insert: сначала вводим ключ, потом значение через пробел
                        if (std::isdigit(text->unicode)) {
                            if (inputValue.empty())
                                inputKey += static_cast<char>(text->unicode);
                            else
                                inputValue += static_cast<char>(text->unicode);
                        }
                        if (text->unicode == ' ' && !inputKey.empty() && inputValue.empty()) {
                            inputValue = ""; // начать ввод значения
                        }
                        if (text->unicode == '\b') {
                            if (!inputValue.empty()) inputValue.pop_back();
                            else if (!inputKey.empty()) inputKey.pop_back();
                        }
                        if (text->unicode == '\r') {
                            if (!inputKey.empty() && !inputValue.empty()) {
                                currentTree->insert(std::stoi(inputKey), std::stoi(inputValue));
                                message = "Inserted: " + inputKey + ":" + inputValue;
                                messageTimer = 2.f;
                            } else {
                                message = "Enter key and value";
                            }
                            currentScreen = Screen::TreeScreen;
                        }
                    } else {
                        // Remove/Search: только ключ
                        if (std::isdigit(text->unicode)) inputKey += static_cast<char>(text->unicode);
                        if (text->unicode == '\b' && !inputKey.empty()) inputKey.pop_back();
                        if (text->unicode == '\r') {
                            if (!inputKey.empty()) {
                                if (message.find("remove") != std::string::npos) {
                                    currentTree->remove(std::stoi(inputKey));
                                    message = "Removed: " + inputKey;
                                } else if (message.find("search") != std::string::npos) {
                                    int val = 0;
                                    if (currentTree->search(std::stoi(inputKey), val))
                                        message = "Found: " + inputKey + ":" + std::to_string(val);
                                    else
                                        message = "Not found: " + inputKey;
                                }
                                messageTimer = 2.f;
                            }
                            currentScreen = Screen::TreeScreen;
                        }
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
            sf::RectangleShape leftBg(sf::Vector2f(900, 800));
            leftBg.setPosition(sf::Vector2f(0.f, 0.f));
            leftBg.setFillColor(sf::Color(220, 240, 255));
            window.draw(leftBg);

            sf::RectangleShape rightBg(sf::Vector2f(380, 800));
            rightBg.setPosition(sf::Vector2f(900.f, 0.f));
            rightBg.setFillColor(sf::Color(180, 210, 255));
            window.draw(rightBg);

            for (auto& btn : opButtons) {
                window.draw(btn.shape);
                window.draw(btn.text);
            }

            // Визуализация дерева (компактная для всех типов)
            if (selectedTreeType == "AVL Tree") {
                auto* avl = dynamic_cast<AVLTree*>(currentTree.get());
                if (avl && avl->getRoot()) drawTreeCompact(avl->getRoot(), window, font);
            } else if (selectedTreeType == "Splay Tree") {
                auto* splay = dynamic_cast<SplayTree*>(currentTree.get());
                if (splay && splay->getRoot()) drawTreeCompact(splay->getRoot(), window, font);
            } else if (selectedTreeType == "Dancing Tree") {
                auto* dancing = dynamic_cast<DancingTree*>(currentTree.get());
                if (dancing && dancing->getRoot()) drawTreeCompact(dancing->getRoot(), window, font);
            }
        } else if (currentScreen == Screen::InputKey) {
            sf::RectangleShape inputBg(sf::Vector2f(600, 200));
            inputBg.setPosition(sf::Vector2f(340.f, 300.f));
            inputBg.setFillColor(sf::Color(240, 250, 255));
            inputBg.setOutlineColor(sf::Color(100, 100, 100));
            inputBg.setOutlineThickness(2.f);
            window.draw(inputBg);

            sf::Text prompt(font, message, 28);
            prompt.setFillColor(sf::Color::Black);
            prompt.setPosition(sf::Vector2f(360.f, 320.f));
            window.draw(prompt);

            sf::Text inputText(font, "", 28);
            if (message.find("value") != std::string::npos)
                inputText.setString(inputKey + (inputValue.empty() ? "" : " " + inputValue));
            else
                inputText.setString(inputKey);
            inputText.setFillColor(sf::Color::Blue);
            inputText.setPosition(sf::Vector2f(360.f, 370.f));
            window.draw(inputText);
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

int main() {
    run();
    return 0;
}