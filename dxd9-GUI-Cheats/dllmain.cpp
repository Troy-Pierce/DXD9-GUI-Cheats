// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

// external vars

dxd9::tEndScene dxd9::originalEndScene = nullptr;
LPDIRECT3DDEVICE9 dxd9::pDevice = nullptr;
void* dxd9::d3d9Device[119]{ 0 };
BYTE dxd9::EndScene[7]{ 0 };
ID3DXFont* dxd9::dxdFont = nullptr;

std::vector<MenuGui::Menu*> MenuGui::Menu::menuList{};


int red{ rand() % 256 }, green{ rand() % 256 }, blue{ rand() % 256 };
bool redDec{ false }, greenDec{ false }, blueDec{ false };
Gamepad* gamepad = new Gamepad(0);

MenuGui::Menu* menu;
MenuGui::Page* settingsPage;
void openSettingsPage() {
    menu->openPage(*settingsPage);
}

void requestEject() {
    menu->ejectMenu = true;
}

void APIENTRY hookEndScene(LPDIRECT3DDEVICE9 oldPDevice) {
    if (!dxd9::pDevice) {
        dxd9::pDevice = oldPDevice;
        if (!dxd9::dxdFont) {
            /*HRESULT hResult = D3DXCreateFont(dxd9::pDevice, 24, 0, FW_REGULAR, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial", &dxd9::dxdFont);*/
            D3DXCreateFont(dxd9::pDevice, 24, 0, FW_REGULAR, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial", &dxd9::dxdFont);
            
        }
    }
    SetWindowText(dxd9::GetWindow(), L"Drawing");
    dxd9::DrawString("Cheats Enabled", 10, 5, red, green, blue);

    if (gamepad->isConnected()) {
        dxd9::DrawString("Controller Connected", 150, 5, red, green, blue);
    }
    for (MenuGui::Menu* pMenu : MenuGui::Menu::menuList) {
        if (pMenu->isVisible) {
            pMenu->drawFunction();
        }
    }

    dxd9::originalEndScene(dxd9::pDevice);

}

DWORD WINAPI Thread(HMODULE hModule) {
    // hook

    if (dxd9::GetDevice(dxd9::d3d9Device, sizeof(dxd9::d3d9Device))) {
        memcpy(dxd9::EndScene, (char*)dxd9::d3d9Device[42], 7);
        dxd9::originalEndScene = (dxd9::tEndScene)hook::TrampHook((char*)dxd9::d3d9Device[42], (char*)hookEndScene, 7);
        
    }
    
    menu = new MenuGui::Menu("Generic Menu", Vector2{ 350, 100 }, Vector2{300, 600});
    MenuGui::Page* pPage = menu->createPage("Home Page", nullptr);
    settingsPage = menu->createPage("Settings", pPage);
    menu->createButtonCode("code button", *pPage, []() -> void {
        });
    menu->createButtonToggleable("toggle button", *pPage, []()-> void {
        }, []()->void {
        });
    menu->createButtonPage("Settings", *pPage, openSettingsPage);
    RECT hwndSize;
    GetWindowRect(dxd9::GetWindow(), &hwndSize);
    menu->createScrollButton("X Position", *settingsPage, menu->menuLocation.x, hwndSize.right - hwndSize.left - menu->menuSize.x - 5, 0);
    menu->createScrollButton("Y Postion", *settingsPage, menu->menuLocation.y, hwndSize.bottom - hwndSize.top - menu->menuSize.y - 5, 0);
    menu->createButtonCode("Eject Menu", *pPage, requestEject);
    
    //Menu::init();
    /*XInputEnable(true);*/
    //SetWindowText(dxd9::GetWindow(), L"Generic Cheats");

    while (true) {
        // loop execution
        gamepad->update();
        if (menu->disableKeys) {
            Sleep(100);
            menu->disableKeys = false;
        }
        if (GetAsyncKeyState(VK_NUMPAD0) & 1 || (gamepad->isButtonPressed(gamepad->LTHUMB)&&gamepad->isButtonPressed(gamepad->RTHUMB))) {
            menu->isVisible = !menu->isVisible;
            if (gamepad->isConnected()) {
                gamepad->vibration(0.1, 0.1);
                Sleep(200);
                gamepad->vibration(0, 0);
            }
        }
        if (menu->isVisible) {
            if (GetAsyncKeyState(VK_NUMPAD8) & 1 || gamepad->isButtonPressed(gamepad->DPAD_UP)) {
                menu->previousButton();
                if (gamepad->isConnected()) {
                    Sleep(200);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD2) & 1 || gamepad->isButtonPressed(gamepad->DPAD_DOWN)) {
                menu->nextButton();
                if (gamepad->isConnected()) {
                    Sleep(200);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD7) & 1 || gamepad->isButtonPressed(gamepad->LSHOULDER)) {
                menu->backPage();
                if (gamepad->isConnected()) {
                    Sleep(200);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD5) & 1 || gamepad->isButtonPressed(gamepad->RSHOULDER)) {
                menu->activateSelectedButton();
                if (gamepad->isConnected()) {
                    Sleep(200);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD6) & 1 || gamepad->isButtonPressed(gamepad->DPAD_RIGHT)) {
                if (menu->currentButton->type() == MenuGui::ButtonType::ScrollButton) {
                    if (((MenuGui::ScrollButton*) menu->currentButton)->value != ((MenuGui::ScrollButton*) menu->currentButton)->maxVal) {
                        ++((MenuGui::ScrollButton*) menu->currentButton)->value;
                    }
                    Sleep(20);
                }
                else {
                    Sleep(200);
                }
                
            }
            if (GetAsyncKeyState(VK_NUMPAD4) & 1 || gamepad->isButtonPressed(gamepad->DPAD_LEFT)) {
                if (menu->currentButton->type() == MenuGui::ButtonType::ScrollButton) {
                    if (((MenuGui::ScrollButton*) menu->currentButton)->value != ((MenuGui::ScrollButton*) menu->currentButton)->minVal) {
                        --((MenuGui::ScrollButton*) menu->currentButton)->value;
                    }
                    Sleep(20);
                }
                else {
                    Sleep(200);
                }
                
            }
        }
        if (menu->ejectMenu) {
            menu->isVisible = false;
            break;
        }
        if (red == 256 || red == 0) {
            redDec = !redDec;
        }
        if (green == 256 || green == 0) {
            greenDec = !greenDec;
        }
        if (blue == 256 || blue == 0) {
            blueDec = !blueDec;
        }
        if (redDec) {
            --red;
        }
        else {
            ++red;
        }
        if (blueDec) {
            --blue;
        }
        else {
            ++blue;
        }
        if (greenDec) {
            --green;
        }
        else {
            ++green;
        }
        Sleep(5);
    }

    // unhook
    mem::Patch((BYTE*)dxd9::d3d9Device[42], dxd9::EndScene, 7);
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Thread, hModule, NULL, NULL));
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

