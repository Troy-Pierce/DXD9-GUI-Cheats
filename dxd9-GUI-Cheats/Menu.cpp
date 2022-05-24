#include "pch.h"

MenuGui::ButtonType MenuGui::ScrollButton::type() const {
	return ButtonType::ScrollButton;
}

MenuGui::ScrollButton::ScrollButton(const char* name, int& changeValue, int maxValue, int minValue) : Button(name, nullptr), maxVal{ maxValue }, minVal{ minValue }, value{ changeValue }{

}

void MenuGui::ToggleableButton::toggleButton() {
	this->enabled = !this->enabled;
	if (this->enabled) {
		this->activationFunction();
	}
	else {
		this->deactivationFunction();
	}
}

MenuGui::ButtonType MenuGui::ToggleableButton::type() const {
	return ButtonType::ToggleableButton;
}

MenuGui::ToggleableButton::ToggleableButton(const char* name, void(*function)(), void(*endFunction)()) : Button(name, function), deactivationFunction{ endFunction } {

}

MenuGui::ButtonType MenuGui::PageButton::type() const {
	return ButtonType::PageButton;
}

MenuGui::PageButton::PageButton(const char* name, void(*function)()) : Button(name, function) {

}

MenuGui::ButtonType MenuGui::Button::type() const {
	return ButtonType::Button;
}

MenuGui::Button::Button(const char* name, void(*function)()) : buttonName{ name }, activationFunction{ function } {

}

MenuGui::Page::Page(const char* name, Menu& menu, Page* page) : pageName{ name }, parentMenu{ menu }, parentPage{ page } {}

void MenuGui::Menu::nextButton() {
	std::vector<Button*>::const_iterator indexOfCurrent = std::find(this->currentPage->buttonList.cbegin(), this->currentPage->buttonList.cend(), this->currentButton);
	if ((indexOfCurrent+1) != this->currentPage->buttonList.cend()) {
		this->currentButton = *++indexOfCurrent;
		std::vector<Button*>::const_iterator indexOfTop = std::find(this->currentPage->buttonList.cbegin(), this->currentPage->buttonList.cend(), this->topMostButton);
		if (std::distance(indexOfTop, indexOfCurrent) >= 14) {
			this->topMostButton = *++indexOfTop;
		}
	}
}

void MenuGui::Menu::previousButton() {
	std::vector<Button*>::const_iterator indexOfCurrent = std::find(this->currentPage->buttonList.cbegin(), this->currentPage->buttonList.cend(), this->currentButton);
	if (indexOfCurrent != this->currentPage->buttonList.cbegin()) {
		if (this->currentButton == this->topMostButton) {
			this->topMostButton = *(indexOfCurrent - 1);
		}
		this->currentButton = *--indexOfCurrent;
	}
}


void MenuGui::Menu::openPage(Page& page) {
	this->currentPage = &page;
	if (page.buttonList.size() > 0) {
		this->currentButton = page.buttonList[0];
		this->topMostButton = page.buttonList[0];
	}
}

void MenuGui::Menu::backPage() {
	this->disableKeys = true;
	if (this->currentPage->parentPage != nullptr) {
		this->openPage(*this->currentPage->parentPage);
	}
}

void MenuGui::Menu::activateSelectedButton() {
	this->disableKeys = true;
	if (this->currentButton->type() != ButtonType::ScrollButton) {
		if (this->currentButton->type() == ButtonType::ToggleableButton) {
			((ToggleableButton*)this->currentButton)->toggleButton();
		}
		else {
			this->currentButton->activationFunction();
		}
	}
}

MenuGui::Button* MenuGui::Menu::createButtonCode(const char* name, Page& parentPage, void(*function)()) {
	Button* pButton = new Button(name, function);
	if (this->currentPage == &parentPage) {
		if (this->currentButton == nullptr) {
			this->currentButton = pButton;
			this->topMostButton = pButton;
		}
	}
	parentPage.buttonList.push_back(pButton);
	return pButton;
}

MenuGui::PageButton* MenuGui::Menu::createButtonPage(const char* name, Page& parentPage, void(*function)()) {
	PageButton* pButton = new PageButton(name, function);
	if (this->currentPage == &parentPage) {
		if (this->currentButton == nullptr) {
			this->currentButton = (Button*) pButton;
			this->topMostButton = (Button*) pButton;
		}
	}
	parentPage.buttonList.push_back((Button*) pButton);
	return pButton;
}

MenuGui::ToggleableButton* MenuGui::Menu::createButtonToggleable(const char* name, Page& parentPage, void(*function)(), void(*endFunction)()) {
	ToggleableButton* pButton = new ToggleableButton(name, function, endFunction);
	if (this->currentPage == &parentPage) {
		if (this->currentButton == nullptr) {
			this->currentButton = (Button*)pButton;
			this->topMostButton = (Button*)pButton;
		}
	}
	parentPage.buttonList.push_back((Button*)pButton);
	return pButton;
}

MenuGui::ScrollButton* MenuGui::Menu::createScrollButton(const char* name, Page& parentPage, int& changeValue, int maxValue, int minValue) {
	ScrollButton* pButton = new ScrollButton(name, changeValue, maxValue, minValue);
	if (this->currentPage == &parentPage) {
		if (this->currentButton == nullptr) {
			this->currentButton = (Button*)pButton;
			this->topMostButton = (Button*)pButton;
		}
	}
	parentPage.buttonList.push_back((Button*)pButton);
	return pButton;
}

MenuGui::Page* MenuGui::Menu::createPage(const char* name, Page* page) {
	Page* pPage = new Page(name, *this, page);
	if (this->currentPage == nullptr) {
		this->openPage(*pPage);
	}
	return pPage;
}

void MenuGui::Menu::drawFunction() {
	dxd9::DrawBorder(this->menuLocation.x, this->menuLocation.y, this->menuSize.x, this->menuSize.y, 5, 10, 10, 10); // Border
	dxd9::DrawBox(this->menuLocation.x+5, this->menuLocation.y, this->menuSize.x-5, (this->menuSize.y-5)-510, 255, 232, 114); // Top
	dxd9::DrawBox(this->menuLocation.x + 5, this->menuLocation.y + ((this->menuSize.y-5)-510), this->menuSize.x - 5, this->menuSize.y-5 - ((this->menuSize.y - 5) / 7), 30, 30, 30); // body
	dxd9::DrawBox(this->menuLocation.x + 5, this->menuLocation.y + ((this->menuSize.y - 5) - 510), this->menuSize.x - 5, 2, 0, 0, 0); // top-body seperator
	dxd9::DrawString(this->menuName, this->menuLocation.x + ((this->menuSize.x / 4) -  strlen(this->menuName)), this->menuLocation.y + 10, 0, 0, 0); // Menu Title

	if (this->currentPage != nullptr) {
		dxd9::DrawString(this->currentPage->pageName, this->menuLocation.x + ((this->menuSize.x / 4) - strlen(this->menuName)), this->menuLocation.y + 60, 0, 0, 0); // Page title
		if (this->currentPage->buttonList.size() > 0) {
			if (this->currentButton == nullptr) {
				this->currentButton = this->currentPage->buttonList[0];
				this->topMostButton = this->currentPage->buttonList[0];
			}
			std::vector<Button*> vec = this->currentPage->buttonList;
			std::vector<Button*>::const_iterator iter = vec.cbegin();
			std::vector<Button*>::const_iterator indexOfTop = std::find(iter, vec.cend(), this->topMostButton);
			int multiple = 0;
			if (indexOfTop != vec.cend()) {
				
				std::advance(iter, std::distance(iter, indexOfTop));
				while (std::distance(indexOfTop, iter) <= (15 - 2)) {
					if (iter == vec.cend()) {
						break;
					}
					if (*iter != this->currentButton) {
						dxd9::DrawBox(this->menuLocation.x + 5, (this->menuLocation.y + ((this->menuSize.y - 5) / 7)) + (10 + multiple * 35), this->menuSize.x - 5, 35, 30, 30, 30);
						dxd9::DrawString((*iter)->buttonName, this->menuLocation.x + 10, (this->menuLocation.y + ((this->menuSize.y - 5) / 7)) + (5 + multiple * 35) + 10, 255, 255, 255);
					}
					else {
						dxd9::DrawBox(this->menuLocation.x + 5, (this->menuLocation.y + ((this->menuSize.y - 5) / 7)) + (10 + multiple * 35), this->menuSize.x - 5, 35, 100, 100, 100);
						dxd9::DrawString((*iter)->buttonName, this->menuLocation.x + 10, (this->menuLocation.y + ((this->menuSize.y - 5) / 7)) + (5 + multiple * 35) + 10, 0, 0, 0);
					}
					if ((*iter)->type() == ButtonType::PageButton) {
						dxd9::DrawString(">", this->menuLocation.x+this->menuSize.x - 5 - 10, (this->menuLocation.y + (this->menuSize.y / 7)) + (5 + multiple * 35) + 10, 255, 255, 255);
					}
					else if((*iter)->type() == ButtonType::ToggleableButton){
						if (((ToggleableButton*)*iter)->enabled) {
							dxd9::DrawString("*", this->menuLocation.x + this->menuSize.x - 5 - 10, (this->menuLocation.y + (this->menuSize.y / 7)) + (5 + multiple * 35) + 15, 0, 255, 0);
						}
						else {
							dxd9::DrawString("*", this->menuLocation.x + this->menuSize.x - 5 - 10 , (this->menuLocation.y + (this->menuSize.y / 7)) + (5 + multiple * 35) + 15, 255, 0, 0);
						}
					}
					else if ((*iter)->type() == ButtonType::ScrollButton) {
						dxd9::DrawString(std::to_string(((ScrollButton*)(*iter))->value).c_str(), this->menuLocation.x + this->menuSize.x - 5 - 50, (this->menuLocation.y + (this->menuSize.y / 7) + 10) + (5 + multiple * 35), 255, 255, 255);
					}
					++iter;
					++multiple;
				}
			}
		}
	}
}

MenuGui::Menu::Menu(const char* name, Vector2 location, Vector2 size) : menuName{ name }, menuLocation{ location }, menuSize{ size }, currentPage{ nullptr } {
	this->currentButton = nullptr;
	this->currentPage = nullptr;
	this->menuList.push_back(this);
}

