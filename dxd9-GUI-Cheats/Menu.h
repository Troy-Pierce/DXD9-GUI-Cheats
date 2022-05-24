#pragma once


namespace MenuGui
{
	enum class ButtonType{ Button, PageButton, ToggleableButton, ScrollButton};

	class Menu;

	class Button {
	public:
		Button(const char* name, void(*function)());
		const char* buttonName;
		void(*activationFunction)();

		virtual ButtonType type() const;
	};

	class Page {
	public:
		Page(const char* name, Menu& menu, Page* page);
		const char* pageName;
		std::vector<Button*> buttonList{};
		Menu& parentMenu;
		Page* parentPage;
	};

	class ScrollButton : public Button {
	public:
		ScrollButton(const char* name, int& changeValue, int maxValue, int minValue);
		int maxVal;
		int minVal;
		int& value;
		ButtonType type() const override;
	};

	class PageButton : public Button {
	public:
		PageButton(const char* name, void(*function)());
		ButtonType type() const override;
	};

	class ToggleableButton : public Button {
	public:
		ToggleableButton(const char* name, void(*function)(), void(*endFunction)());
		bool enabled{ false };
		void(*deactivationFunction)();
		void toggleButton();
		ButtonType type() const override;
	};

	class Menu {
	public:
		Menu(const char* name, Vector2 location, Vector2 size);
		const char* menuName;
		Vector2 menuLocation;
		Vector2 menuSize;
		bool isVisible{ true };
		bool disableKeys{ false };
		bool ejectMenu{ false };
		static std::vector<Menu*> menuList;
		void drawFunction();

		void openPage(Page& page);
		Page* createPage(const char* name, Page* page);
		Button* createButtonCode(const char* name, Page& parentPage, void(*function)() );
		PageButton* createButtonPage(const char* name, Page& parentPage, void(*function)());
		ToggleableButton* createButtonToggleable(const char* name, Page& parentPage, void(*function)(), void(*endFunction)());
		ScrollButton* createScrollButton(const char* name, Page& parentPage, int& changeValue, int maxValue, int minValue);
		void activateSelectedButton();
		void nextButton();
		void previousButton();
		void backPage();
		Button* currentButton{ nullptr };
	private:
		Page* currentPage{ nullptr };
		Button* topMostButton{ nullptr };
		
	};
	
	//class Page {
	//public:
	//	Page();
	//	const char* pageName;
	//	std::vector<Button*> buttonList{};
	//	void openPage();
	//};


};

