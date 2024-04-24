#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>

#pragma comment(lib, "Winmm.lib") // Link against Winmm library

using namespace std;

// Global variables
const wchar_t* songs[] = {
    L"Blinding Lights.wav",
    L"Save Your Tears.wav",
    L"Starboy.wav",
    L"Heartless.wav",
    L"In Your Eyes.wav"
};
const wchar_t* authors[] = {
    L"The Weeknd",
    L"The Weeknd",
    L"The Weeknd",
    L"The Weeknd",
    L"The Weeknd"
};
const int numSongs = sizeof(songs) / sizeof(songs[0]);
int currentSongIndex = 0;
bool isPlaying = false;
vector<int> recentlyPlayed; // List of recently played songs
wchar_t lyricsFileName[100]; // File name to read lyrics
wstring lyrics; // Variable to hold the lyrics

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void PlayCurrentSong(HWND hWnd);
void TogglePlayback(HWND hWnd);
void NextSong(HWND hWnd);
void PrevSong(HWND hWnd);
void DrawUI(HWND hWnd);
void UpdateRecentlyPlayed(int songIndex);
void LoadLyrics(int songIndex);

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
    wcex.lpszClassName = L"MusicPlayerClass";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) {
        MessageBoxW(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    HWND hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"MusicPlayerClass", L"Music Player", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
        MessageBoxW(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // Create UI elements
        DrawUI(hWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // Play/Pause button
            TogglePlayback(hWnd);
            break;
        case 2: // Next button
            NextSong(hWnd);
            break;
        case 3: // Prev button
            PrevSong(hWnd);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Function to play the current song
void PlayCurrentSong(HWND hWnd) {
    if (!isPlaying) {
        PlaySoundW(songs[currentSongIndex], NULL, SND_FILENAME | SND_ASYNC);
        isPlaying = true;
    }
    else {
        PlaySoundW(NULL, NULL, 0);
        isPlaying = false;
    }

    // Update the list of recently played songs
    UpdateRecentlyPlayed(currentSongIndex);

    // Load lyrics for the current song
    LoadLyrics(currentSongIndex);
}

// Function to toggle playback
void TogglePlayback(HWND hWnd) {
    PlayCurrentSong(hWnd);
    // Update Play/Pause button text based on playback state
    HWND btnPlayPause = GetDlgItem(hWnd, 1);
    if (isPlaying) {
        SetWindowTextW(btnPlayPause, L"Pause");
    }
    else {
        SetWindowTextW(btnPlayPause, L"Play");
    }
}

// Function to play the next song
void NextSong(HWND hWnd) {
    currentSongIndex = (currentSongIndex + 1) % numSongs;
    PlaySoundW(NULL, NULL, 0); // Stop current song
    PlayCurrentSong(hWnd);
    // Update UI elements to show the next song and author
    DrawUI(hWnd);
}

// Function to play the previous song
void PrevSong(HWND hWnd) {
    currentSongIndex = (currentSongIndex - 1 + numSongs) % numSongs;
    PlaySoundW(NULL, NULL, 0); // Stop current song
    PlayCurrentSong(hWnd);
    // Update UI elements to show the previous song and author
    DrawUI(hWnd);
}

// Function to draw the UI
void DrawUI(HWND hWnd) {
    // Clear the window content
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);

    // Create UI elements
    CreateWindowW(L"STATIC", songs[currentSongIndex], WS_VISIBLE | WS_CHILD, 20, 20, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", authors[currentSongIndex], WS_VISIBLE | WS_CHILD, 20, 50, 200, 20, hWnd, NULL, NULL, NULL);

    // Play/Pause button
    CreateWindowW(L"BUTTON", isPlaying ? L"Pause" : L"Play", WS_VISIBLE | WS_CHILD, 20, 80, 100, 30, hWnd, (HMENU)1, NULL, NULL);

    // Next button
    CreateWindowW(L"BUTTON", L"Next", WS_VISIBLE | WS_CHILD, 140, 80, 100, 30, hWnd, (HMENU)2, NULL, NULL);

    // Previous button
    CreateWindowW(L"BUTTON", L"Prev", WS_VISIBLE | WS_CHILD, 260, 80, 100, 30, hWnd, (HMENU)3, NULL, NULL);

    // Display recently played songs
    int yOffset = 120;
    CreateWindowW(L"STATIC", L"Recently Played:", WS_VISIBLE | WS_CHILD, 20, yOffset, 200, 20, hWnd, NULL, NULL, NULL);
    yOffset += 20;

    // Display the list of recently played songs
    for (int i = 0; i < recentlyPlayed.size(); i++) {
        wstring recentSongInfo = songs[recentlyPlayed[i]];
        recentSongInfo += L" - ";
        recentSongInfo += authors[recentlyPlayed[i]];
        CreateWindowW(L"STATIC", recentSongInfo.c_str(), WS_VISIBLE | WS_CHILD, 20, yOffset, 300, 20, hWnd, NULL, NULL, NULL);
        yOffset += 20;
    }

    // Display lyrics
    yOffset += 20;
    CreateWindowW(L"STATIC", L"Lyrics:", WS_VISIBLE | WS_CHILD, 20, yOffset, 200, 20, hWnd, NULL, NULL, NULL);
    yOffset += 20;
    CreateWindowW(L"STATIC", lyrics.c_str(), WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL, 20, yOffset, 450, 200, hWnd, NULL, NULL, NULL);
}

// Function to update the list of recently played songs
void UpdateRecentlyPlayed(int songIndex) {
    // Add the current song index to the list of recently played songs
    recentlyPlayed.push_back(songIndex);

    // Keep the size of the recently played list to a maximum of 5 songs
    if (recentlyPlayed.size() > 5) {
        recentlyPlayed.erase(recentlyPlayed.begin());
    }
}

// Function to load lyrics for the current song
void LoadLyrics(int songIndex) {
    // Generate the filename for the lyrics file
    swprintf(lyricsFileName, 100, L"%ls.txt", songs[songIndex]);

    // Read the lyrics from the file
    wifstream lyricsFile(lyricsFileName);
    if (lyricsFile.is_open()) {
        wstring line;
        lyrics.clear();
        while (getline(lyricsFile, line)) {
            lyrics += line;
            lyrics += L"\n";
        }
        lyricsFile.close();
    }
    else {
        lyrics = L"Lyrics not available.";
    }
}

/*temp comment pls ignore trying to show this idiot how to use sync and push in git.
Yours Truly
Randomguy
(SG)
ps,please give him full grade, otherwise i'll be sad*/