#include "ButtonFactory.h"
#include "SceneManager.h"


Button* ButtonFactory::CreateButton(SceneUpdate scene, const Rectangle& rect, const std::string& text, int fontSize, Color textColor, Color boxColor, Color hoverColor)
{
	auto button = std::make_unique<Button>(rect, text, [scene]()
		{
			SceneManager::GetInstance().SetActiveScene(scene);
		}, fontSize, textColor, boxColor, hoverColor);

	Button* buttonPtr = button.get();
	buttons.push_back(std::move(button));
	return buttonPtr;
}


Button* ButtonFactory::CreateButton(Action lambda, const Rectangle& rect, const std::string& text, int fontSize, Color textColor, Color boxColor, Color hoverColor)
{
    auto button = std::make_unique<Button>(rect, text, lambda, fontSize, textColor, boxColor, hoverColor);

    Button* buttonPtr = button.get();
    buttons.push_back(std::move(button));
    return buttonPtr;
}
