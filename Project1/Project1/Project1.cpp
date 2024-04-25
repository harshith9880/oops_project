#include <Windows.h>
#include <iostream> // For output window

using namespace std;

// Global variables
const wchar_t* songs[] = {
    L"Blinding Lights.wav",
    L"Save Your Tears.wav",
    L"SampleSong1.wav", // Sample song 1
    L"SampleSong2.wav"  // Sample song 2
};
extern const wchar_t* authors[];
extern const int numSongs;
extern int currentSongIndex;
bool isPlaying = false; // Define isPlaying variable

// Function prototypes
void NextSong(HWND hWnd);
void PrevSong(HWND hWnd);
void DrawUI(HWND hWnd);

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Entry point for Windows GUI applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    HWND hWnd = CreateWindowEx(0, L"MainWindowClass", L"Music Player", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Show and update the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Output to console window
    AllocConsole();
    FILE* consoleOut;
    freopen_s(&consoleOut, "CONOUT$", "w", stdout);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Function to play the next song
void NextSong(HWND hWnd) {
    if (isPlaying) {
        // Stop current song
        // Implementation of stopping current song
    }

    // Implementation of selecting next song

    // Play next song
    // Implementation of playing next song

    // Update UI
    DrawUI(hWnd);
}

// Function to play the previous song
void PrevSong(HWND hWnd) {
    if (isPlaying) {
        // Stop current song
        // Implementation of stopping current song
    }

    // Implementation of selecting previous song

    // Play previous song
    // Implementation of playing previous song

    // Update UI
    DrawUI(hWnd);
}

// Function to draw the UI
void DrawUI(HWND hWnd) {
    // Clear the window content
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);

    // Create "Next" button
    CreateWindowW(L"BUTTON", L"Next", WS_VISIBLE | WS_CHILD, 20, 120, 100, 30, hWnd, (HMENU)2, NULL, NULL);

    // Create "Previous" button
    CreateWindowW(L"BUTTON", L"Previous", WS_VISIBLE | WS_CHILD, 140, 120, 100, 30, hWnd, (HMENU)3, NULL, NULL);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // Draw UI elements
        DrawUI(hWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 2: // Next button
            // Play next song
            NextSong(hWnd);
            break;
        case 3: // Previous button
            // Play previous song
            PrevSong(hWnd);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


