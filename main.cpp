#include "iostream"
#include "filesystem"
#include "cstdlib"
#include "format"

#include "windows.h"
#include "urlmon.h"
#include "wininet.h"

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

namespace fs = std::filesystem;

std::string version(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("HTTP Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "[ERROR] InternetOpen failed" << std::endl;
        return "";
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, 0);
    if (!hConnect) {
        std::cerr << "[ERROR] InternetOpenUrl failed" << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string response;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return response;
}

int main() {
	std::cout << "\033[2J\033[H";

	std::string menu_choice;
	bool fishstrapInstalled;

	if (fs::exists(std::format("C:/Users/{}/AppData/Local/Fishstrap", std::getenv("USERNAME")))) {
		fishstrapInstalled = true;
		std::cout << "1. Launch Fishstrap\n2. Downgrade Fishstrap\n\n> ";
		std::cin >> menu_choice;
	} else {
		fishstrapInstalled = false;
		std::cout << "1. Install Fishstrap\n2. Downgrade Fishstrap\n\n> ";
		std::cin >> menu_choice;
	}
	
	if (menu_choice == "1" && fishstrapInstalled == false) {
		std::cout << "[LOGS] Installing Fishstrap...\n";

		HRESULT response = URLDownloadToFileA(NULL, "https://github.com/returnrqt/fishstrap/releases/download/v2.9.0.0/Fishstrap-v2.9.0.0.exe", std::format("{}\\Fishstrap.exe", fs::current_path().string()).c_str(), 0, NULL);

		if (response == S_OK) {
			std::cout << "[LOGS] Installed successfully, loading bootstrapper...\n";
			system(std::format("{}\\Fishstrap.exe", fs::current_path().string()).c_str());
		}
	} else if (menu_choice == "1" && fishstrapInstalled == true) {
		std::cout << "[LOGS] Launching Fishstrap...\n";
		system(std::format("C:/Users/{}/AppData/Local/Fishstrap/Fishstrap.exe", std::getenv("USERNAME")).c_str());
	}

	if (menu_choice == "2" and fishstrapInstalled == true) {
		std::cout << "[LOGS] Fetching downgrade version...\n";
		std::string v = version("https://raw.githubusercontent.com/lowkeyskid/roblox-downgrade-version/refs/heads/main/v.txt");
		std::cout << "[LOGS] " << v;

		std::cout << "[LOGS] Installing downgrade files...\n";
		// TODO:
		//system(std::format("curl https://rdd.latte.to//?channel=LIVE&binaryType=WindowsPlayer&version={} -O -J -L", v).c_str());
	}

	return 0;
}
