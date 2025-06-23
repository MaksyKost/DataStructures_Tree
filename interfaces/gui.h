#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../src/tree_structure.h"
#include <functional>

class TreeVisualizer {
public:
    TreeVisualizer();
    int run();

private:
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
        Button(const std::string& str, const sf::Font& font, unsigned int size);
    };

    struct DrawNode {
        float x, y;
        int key, value;
        int leftIdx = -1, rightIdx = -1;
        std::string label;
    };

    void drawTree();
    void handleEvents();
    void render();
    void createOpButtons();
    void setupTree(const std::string& type);
    void handleInputEvent(const sf::Event::TextEvent& text);

    void drawTreeCompact(const TreeNode* root);
    void drawTreeCompact(const TTreeNode* root);

    sf::RenderWindow window;
    sf::Font font;
    Screen currentScreen;
    std::unique_ptr<TreeStructure> currentTree;
    std::string selectedTreeType;
    std::string inputKey, inputValue, message;
    float messageTimer;
    int highlightKey;
    float highlightTimer;
    std::vector<std::string> treeTypes;
    int mainMenuSelected;
    std::vector<Button> opButtons;
    sf::Clock clock;
    bool inputForInsert;
    bool enteringValue;
};