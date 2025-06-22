#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <optional>
#include <limits>
#include "../src/avl_tree.h"
#include "../src/splay_tree.h"
#include "../src/dancing_tree.h"
#include "../src/t_tree.h"
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

// Структура для узла, который будет нарисован
struct DrawNode {
    float x, y;
    int key, value;
    int leftIdx = -1, rightIdx = -1;
    std::string label; // Используется для T-Tree
};

// --- ПОЛНОСТЬЮ ИСПРАВЛЕННЫЕ ФУНКЦИИ assignPositions ---

// Правильная версия для стандартных деревьев
template<typename NodeT>
int assignPositions(const NodeT* node, float depth, float& x, std::vector<DrawNode>& nodes) {
    if (!node) return -1;

    // 1. Рекурсивно обрабатываем левое поддерево
    int leftIdx = assignPositions(node->left, depth + 1, x, nodes);

    // 2. Добавляем ТЕКУЩИЙ узел и получаем его индекс
    int myIdx = nodes.size();
    nodes.push_back({0, 0, node->key, node->value, -1, -1});
    
    // 3. Вычисляем его позицию по X после всех левых узлов
    nodes[myIdx].x = x * 70 + 100;
    nodes[myIdx].y = depth * 80 + 100;
    x += 1; // Сдвигаем X для следующих (правых) узлов

    // 4. Рекурсивно обрабатываем правое поддерево
    int rightIdx = assignPositions(node->right, depth + 1, x, nodes);

    // 5. Записываем правильные индексы детей
    nodes[myIdx].leftIdx = leftIdx;
    nodes[myIdx].rightIdx = rightIdx;

    // 6. Возвращаем свой индекс родителю
    return myIdx;
}

// Правильная перегрузка для T-Tree
int assignPositions(const TTreeNode* node, float depth, float& x, std::vector<DrawNode>& nodes) {
    if (!node) return -1;

    // Собираем метку для узла
    std::string label;
    for (size_t i = 0; i < node->keys.size(); ++i) {
        if (i > 0) label += " ";
        label += std::to_string(node->keys[i]) + ":" + std::to_string(node->values[i]);
    }

    // Та же самая правильная логика обхода
    int leftIdx = assignPositions(node->left, depth + 1, x, nodes);

    int myIdx = nodes.size();
    nodes.push_back({0, 0, 0, 0, -1, -1, label}); // Добавляем узел с готовой меткой
    
    nodes[myIdx].x = x * 70 + 100;
    nodes[myIdx].y = depth * 80 + 100;
    x += 1;

    int rightIdx = assignPositions(node->right, depth + 1, x, nodes);

    nodes[myIdx].leftIdx = leftIdx;
    nodes[myIdx].rightIdx = rightIdx;

    return myIdx;
}


// --- УНИФИЦИРОВАННЫЕ ФУНКЦИИ ОТРИСОВКИ С ИСПРАВЛЕНИЯМИ ДЛЯ SFML 3.0 ---
void drawTree(sf::RenderWindow& window, const std::vector<DrawNode>& nodes, const sf::Font& font,
              int highlightKey, float highlightTimer,
              float areaX, float areaY, float areaWidth, float areaHeight, float padding) {
    if (nodes.empty()) return;

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    for (const auto& n : nodes) {
        minX = std::min(minX, n.x);
        maxX = std::max(maxX, n.x);
        minY = std::min(minY, n.y);
        maxY = std::max(maxY, n.y);
    }

    float treeWidth = maxX - minX;
    float treeHeight = maxY - minY;
    float scaleX = (areaWidth - 2 * padding) / (treeWidth > 0 ? treeWidth : 1);
    float scaleY = (areaHeight - 2 * padding) / (treeHeight > 0 ? treeHeight : 1);
    float scale = std::min(scaleX, scaleY);
    float offsetX = areaX + padding - minX * scale + (areaWidth - 2 * padding - (treeWidth * scale)) / 2;
    float offsetY = areaY + padding - minY * scale + (areaHeight - 2 * padding - (treeHeight * scale)) / 2;

    for (const auto& n : nodes) {
        float drawX1 = n.x * scale + offsetX;
        float drawY1 = n.y * scale + offsetY;
        if (n.leftIdx != -1) {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            float drawX2 = nodes[n.leftIdx].x * scale + offsetX;
            float drawY2 = nodes[n.leftIdx].y * scale + offsetY;
            line[0].position = {drawX1, drawY1};
            line[1].position = {drawX2, drawY2};
            line[0].color = line[1].color = sf::Color::White;
            window.draw(line);
        }
        if (n.rightIdx != -1) {
            sf::VertexArray line(sf::PrimitiveType::Lines, 2);
            float drawX2 = nodes[n.rightIdx].x * scale + offsetX;
            float drawY2 = nodes[n.rightIdx].y * scale + offsetY;
            line[0].position = {drawX1, drawY1};
            line[1].position = {drawX2, drawY2};
            line[0].color = line[1].color = sf::Color::White;
            window.draw(line);
        }
    }

    for (const auto& n : nodes) {
        std::string label = n.label.empty() ? (std::to_string(n.key) + ":" + std::to_string(n.value)) : n.label;
        
        // ИСПРАВЛЕНИЕ: Конструктор sf::Text
        sf::Text keyText(font, label, 20);
        keyText.setFillColor(sf::Color::Black);

        // ИСПРАВЛЕНИЕ: getLocalBounds().size.x/y вместо width/height
        sf::FloatRect textRect = keyText.getLocalBounds();
        float nodePadding = 12.f;
        sf::Vector2f rectSize(textRect.size.x + nodePadding * 2, textRect.size.y + nodePadding * 2);

        sf::RectangleShape rect(rectSize);
        // ИСПРАВЛЕНИЕ: setOrigin принимает sf::Vector2f
        rect.setOrigin({rectSize.x / 2, rectSize.y / 2});
        float drawX = n.x * scale + offsetX;
        float drawY = n.y * scale + offsetY;
        // ИСПРАВЛЕНИЕ: setPosition принимает sf::Vector2f
        rect.setPosition({drawX, drawY});

        bool isHighlighted = highlightTimer > 0.f && highlightKey != -1 && label.find(std::to_string(highlightKey)) != std::string::npos;
        rect.setFillColor(isHighlighted ? sf::Color(255, 255, 100) : sf::Color(200, 220, 255));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2.f);
        window.draw(rect);

        // ИСПРАВЛЕНИЕ: setOrigin принимает sf::Vector2f, getLocalBounds().position.x/y вместо left/top
        keyText.setOrigin({textRect.position.x + textRect.size.x / 2.f, textRect.position.y + textRect.size.y / 2.f});
        // ИСПРАВЛЕНИЕ: setPosition принимает sf::Vector2f
        keyText.setPosition({drawX, drawY});
        window.draw(keyText);
    }
}

template<typename NodeT>
void drawTreeCompact(const NodeT* node, sf::RenderWindow& window, const sf::Font& font,
                     int highlightKey, float highlightTimer,
                     float areaX, float areaY, float areaWidth, float areaHeight, float padding) {
    if (!node) return;
    std::vector<DrawNode> nodes;
    float x = 0;
    assignPositions(node, 0, x, nodes);
    drawTree(window, nodes, font, highlightKey, highlightTimer, areaX, areaY, areaWidth, areaHeight, padding);
}

void drawTreeCompact(const TTreeNode* node, sf::RenderWindow& window, const sf::Font& font,
                     int highlightKey, float highlightTimer,
                     float areaX, float areaY, float areaWidth, float areaHeight, float padding) {
    if (!node) return;
    std::vector<DrawNode> nodes;
    float x = 0;
    assignPositions(node, 0, x, nodes);
    drawTree(window, nodes, font, highlightKey, highlightTimer, areaX, areaY, areaWidth, areaHeight, padding);
}


int run() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // ИСПРАВЛЕНИЕ: sf::State::Fullscreen
    sf::RenderWindow window(desktop, "Tree Visualizer", sf::State::Fullscreen);
    window.setFramerateLimit(60);
    sf::Font font;
    // ИСПРАВЛЕНИЕ: openFromFile
    if (!font.openFromFile("GOTHICB.ttf")) return 1;

    Screen currentScreen = Screen::MainMenu;
    std::unique_ptr<TreeStructure> currentTree;
    std::string selectedTreeType;
    std::string inputKey, inputValue, message;
    float messageTimer = 0.f;

    int highlightKey = -1;
    float highlightTimer = 0.f;

    std::vector<std::string> treeTypes = {"AVL Tree", "Splay Tree", "Dancing Tree", "T-Tree"};
    int mainMenuSelected = 0;

    std::vector<Button> opButtons;

    auto setupTree = [&](const std::string& type) {
        if (type == "AVL Tree") currentTree = std::make_unique<AVLTree>();
        else if (type == "Splay Tree") currentTree = std::make_unique<SplayTree>();
        else if (type == "Dancing Tree") currentTree = std::make_unique<DancingTree>();
        else if (type == "T-Tree") currentTree = std::make_unique<TTree>();
        selectedTreeType = type;
        currentScreen = Screen::TreeScreen;
        message.clear();
        highlightKey = -1;
        highlightTimer = 0.f;
    };

    auto createOpButtons = [&]() {
        opButtons.clear();
        float winWidth = window.getSize().x;
        float rightPanelX = winWidth * 0.7f;
        float rightPanelWidth = winWidth - rightPanelX;
        std::vector<std::string> ops = {"Insert", "Remove", "Search", "Fill Random", "Clear", "Back"};
        for (size_t i = 0; i < ops.size(); ++i) {
            Button btn(ops[i], font, 24);
            btn.shape.setSize({200, 50});
            float btnX = rightPanelX + (rightPanelWidth - btn.shape.getSize().x) / 2;
            float btnY = 80.f + i * 70.f;
            // ИСПРАВЛЕНИЕ: setPosition принимает sf::Vector2f
            btn.shape.setPosition({btnX, btnY});
            btn.shape.setFillColor(sf::Color(200, 220, 255));
            btn.shape.setOutlineColor(sf::Color::Black);
            btn.shape.setOutlineThickness(2.f);
            
            sf::FloatRect textBounds = btn.text.getLocalBounds();
            // ИСПРАВЛЕНИЕ: setOrigin и getLocalBounds
            btn.text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
            // ИСПРАВЛЕНИЕ: setPosition
            btn.text.setPosition({btnX + btn.shape.getSize().x / 2.f, btnY + btn.shape.getSize().y / 2.f});
            btn.text.setFillColor(sf::Color::Black);
            
            if (ops[i] == "Insert") {
                btn.onClick = [&]() { currentScreen = Screen::InputKey; inputKey.clear(); inputValue.clear(); message = "Enter key and value (e.g. 10 5)"; };
            } else if (ops[i] == "Remove") {
                btn.onClick = [&]() { currentScreen = Screen::InputKey; inputKey.clear(); inputValue.clear(); message = "Enter key to remove"; };
            } else if (ops[i] == "Search") {
                btn.onClick = [&]() { currentScreen = Screen::InputKey; inputKey.clear(); inputValue.clear(); message = "Enter key to search"; };
            } else if (ops[i] == "Fill Random") {
                btn.onClick = [&]() { currentTree->fillRandom(10); message = "Filled with 10 random values"; messageTimer = 2.f; };
            } else if (ops[i] == "Clear") {
                btn.onClick = [&]() { currentTree->clear(); message = "Tree cleared"; messageTimer = 2.f; };
            } else if (ops[i] == "Back") {
                btn.onClick = [&]() { currentScreen = Screen::MainMenu; };
            }
            opButtons.push_back(btn);
        }
    };

    createOpButtons();

    sf::Clock clock;
    bool enteringValue = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
            }
            else if (auto* resized = event->getIf<sf::Event::Resized>())
            {
                window.setView(sf::View(sf::FloatRect({0, 0}, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)})));
                createOpButtons();
            }

            if (currentScreen == Screen::MainMenu) {
                if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Up)
                        mainMenuSelected = (mainMenuSelected + treeTypes.size() - 1) % treeTypes.size();
                    if (keyPressed->code == sf::Keyboard::Key::Down)
                        mainMenuSelected = (mainMenuSelected + 1) % treeTypes.size();
                    if (keyPressed->code == sf::Keyboard::Key::Enter)
                        setupTree(treeTypes[mainMenuSelected]);
                }
            } else if (currentScreen == Screen::TreeScreen) {
                if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left) {
                        auto mousePos = sf::Vector2f(mousePressed->position);
                        for (auto& btn : opButtons) {
                            if (btn.shape.getGlobalBounds().contains(mousePos)) {
                                btn.onClick();
                                break; 
                            }
                        }
                    }
                }
            } else if (currentScreen == Screen::InputKey) {
                bool isInsert = message.find("value") != std::string::npos;
                if (auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                    char ch = static_cast<char>(textEntered->unicode);

                    if (ch == '\b') {
                        if (enteringValue && !inputValue.empty()) inputValue.pop_back();
                        else if (!enteringValue && !inputKey.empty()) inputKey.pop_back();
                        else if (enteringValue && inputValue.empty()) enteringValue = false;
                    } else if (ch == ' ' && isInsert && !inputKey.empty() && !enteringValue) {
                        enteringValue = true;
                    } else if (std::isdigit(ch) || (ch == '-' && (enteringValue ? inputValue.empty() : inputKey.empty()))) {
                        if (enteringValue) inputValue += ch;
                        else inputKey += ch;
                    } else if (ch == '\r' || ch == '\n') {
                        if (!inputKey.empty()) {
                            int key = std::stoi(inputKey);
                            if (isInsert) {
                                if (!inputValue.empty()) {
                                    currentTree->insert(key, std::stoi(inputValue));
                                    message = "Inserted: " + inputKey + ":" + inputValue;
                                    highlightKey = key;
                                    highlightTimer = 2.f;
                                }
                            } else if (message.find("remove") != std::string::npos) {
                                currentTree->remove(key);
                                message = "Removed: " + inputKey;
                                highlightKey = -1;
                            } else if (message.find("search") != std::string::npos) {
                                int val = 0;
                                if (currentTree->search(key, val)) {
                                    message = "Found: " + inputKey + ":" + std::to_string(val);
                                    highlightKey = key;
                                    highlightTimer = 2.f;
                                } else {
                                    message = "Not found: " + inputKey;
                                    highlightKey = -1;
                                }
                            }
                            messageTimer = 2.f;
                        }
                        currentScreen = Screen::TreeScreen;
                        inputKey.clear();
                        inputValue.clear();
                        enteringValue = false;
                    }
                }
            }
        } // Конец цикла событий

        float winWidth = window.getSize().x;
        float winHeight = window.getSize().y;
        float leftWidth = winWidth * 0.7f;
        
        window.clear(sf::Color(7, 107, 148));

        if (currentScreen == Screen::MainMenu) {
            sf::Text title(font, "Tree Visualizer", 60);
            sf::FloatRect titleBounds = title.getLocalBounds();
            title.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y});
            title.setPosition({winWidth / 2.f, 100.f});
            title.setFillColor(sf::Color::White);
            window.draw(title);
            
            for (size_t i = 0; i < treeTypes.size(); ++i) {
                sf::Text text(font, treeTypes[i], 40);
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y});
                text.setPosition({winWidth / 2.f, 250.f + i * 70.f});
                text.setFillColor(i == mainMenuSelected ? sf::Color::Yellow : sf::Color(224, 247, 198));
                window.draw(text);
            }
        } else if (currentScreen == Screen::TreeScreen || currentScreen == Screen::InputKey) {
            sf::RectangleShape leftBg({leftWidth, winHeight});
            leftBg.setFillColor(sf::Color(5, 80, 110));
            window.draw(leftBg);

            sf::RectangleShape rightBg({winWidth - leftWidth, winHeight});
            rightBg.setPosition({leftWidth, 0.f});
            rightBg.setFillColor(sf::Color(7, 107, 148));
            window.draw(rightBg);

            for (auto& btn : opButtons) {
                window.draw(btn.shape);
                window.draw(btn.text);
            }

            float areaX = 0.f, areaY = 0.f, areaWidth = leftWidth, padding = 40.f;
            if (selectedTreeType == "AVL Tree") {
                auto* avl = dynamic_cast<AVLTree*>(currentTree.get());
                if (avl) drawTreeCompact(avl->getRoot(), window, font, highlightKey, highlightTimer, areaX, areaY, areaWidth, winHeight, padding);
            } else if (selectedTreeType == "Splay Tree") {
                auto* splay = dynamic_cast<SplayTree*>(currentTree.get());
                if (splay) drawTreeCompact(splay->getRoot(), window, font, highlightKey, highlightTimer, areaX, areaY, areaWidth, winHeight, padding);
            } else if (selectedTreeType == "Dancing Tree") {
                auto* dancing = dynamic_cast<DancingTree*>(currentTree.get());
                if (dancing) drawTreeCompact(dancing->getRoot(), window, font, highlightKey, highlightTimer, areaX, areaY, areaWidth, winHeight, padding);
            } else if (selectedTreeType == "T-Tree") {
                auto* ttree = dynamic_cast<TTree*>(currentTree.get());
                if (ttree) drawTreeCompact(ttree->getRoot(), window, font, highlightKey, highlightTimer, areaX, areaY, areaWidth, winHeight, padding);
            }
        }

        if (currentScreen == Screen::InputKey) {
            sf::RectangleShape overlay({winWidth, winHeight});
            overlay.setFillColor({0, 0, 0, 150});
            window.draw(overlay);

            float inputBgWidth = 600, inputBgHeight = 200;
            sf::RectangleShape inputBg({inputBgWidth, inputBgHeight});
            inputBg.setOrigin({inputBgWidth / 2, inputBgHeight / 2});
            inputBg.setPosition({winWidth / 2, winHeight / 2});
            inputBg.setFillColor(sf::Color(240, 250, 255));
            inputBg.setOutlineColor(sf::Color(100, 100, 100));
            inputBg.setOutlineThickness(2.f);
            window.draw(inputBg);

            sf::Text prompt(font, message, 28);
            prompt.setFillColor(sf::Color::Black);
            prompt.setPosition({inputBg.getPosition().x - inputBgWidth / 2 + 20, inputBg.getPosition().y - inputBgHeight / 2 + 20});
            window.draw(prompt);

            std::string fullInput = inputKey + (enteringValue ? " " : "") + inputValue;
            sf::Text inputText(font, fullInput + "_", 28);
            inputText.setFillColor(sf::Color::Blue);
            inputText.setPosition({inputBg.getPosition().x - inputBgWidth / 2 + 20, inputBg.getPosition().y - inputBgHeight / 2 + 80});
            window.draw(inputText);
        }

        if (messageTimer > 0.f && currentScreen != Screen::InputKey) {
            sf::Text notifText(font, message, 24);
            sf::FloatRect textBounds = notifText.getLocalBounds();
            float notifWidth = textBounds.size.x + 40, notifHeight = 50;
            sf::RectangleShape notifBg({notifWidth, notifHeight});
            notifBg.setOrigin({notifWidth / 2.f, 0});
            notifBg.setPosition({winWidth / 2, 20});
            notifBg.setFillColor({30, 30, 30, 200});
            window.draw(notifBg);
            
            notifText.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
            notifText.setPosition({notifBg.getPosition().x, notifBg.getPosition().y + notifHeight / 2.f});
            notifText.setFillColor(sf::Color::White);
            window.draw(notifText);
            messageTimer -= dt;
        }

        if (highlightTimer > 0.f) {
            highlightTimer -= dt;
            if (highlightTimer <= 0.f) highlightKey = -1;
        }

        window.display();
    }
    return 0;
}


int main() {
    return run();
}