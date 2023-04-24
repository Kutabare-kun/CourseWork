#pragma once
#include <vector>
#include <memory>
#include "Button.h"
#include "Enums.h"


class ButtonFactory
{
private:
	using Action = std::function<void()>;
	std::vector<std::unique_ptr<Button>> buttons;

public:
	Button* CreateButton(SceneUpdate scene, const Rectangle& rect, const std::string& text, int fontSize = 20, Color textColor = BLACK, Color boxColor = LIGHTGRAY, Color hoverColor = GRAY);
	Button* CreateButton(Action lambda, const Rectangle& rect, const std::string& text, int fontSize = 20, Color textColor = BLACK, Color boxColor = LIGHTGRAY, Color hoverColor = GRAY);
};

