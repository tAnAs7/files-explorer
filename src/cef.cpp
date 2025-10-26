#include <windows.h>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

bool containsFile(const fs::path& dirPath) {
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.path())) return true;
        if (fs::is_directory(entry.path()) && containsFile(entry.path())) return true;
    }
    return false;
}

void removeEmptyFolders(const fs::path& dirPath, std::ostringstream& log) {
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (fs::is_directory(entry.path())) {
            removeEmptyFolders(entry.path(), log);
            if (!containsFile(entry.path())) {
                std::error_code ec;
                fs::remove_all(entry.path(), ec);
                if (!ec)
                    log << "[DELETE] " << entry.path().string() << "\n";
                else
                    log << "[ERROR] " << entry.path().string() << " (" << ec.message() << ")\n";
            }
        }
    }
}

bool isPipeOrRedirected(HANDLE handle) {
    DWORD fileType = GetFileType(handle);
    if (fileType == FILE_TYPE_PIPE) return true;
    if (fileType == FILE_TYPE_DISK) return true; // redirected to file
    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    fs::path root = fs::current_path();
    std::ostringstream log;

    bool hasPipe =
        isPipeOrRedirected(GetStdHandle(STD_OUTPUT_HANDLE)) ||
        isPipeOrRedirected(GetStdHandle(STD_ERROR_HANDLE));

    log << "[START] Checking directory: " << root.string() << "\n";

    try {
        removeEmptyFolders(root, log);
        log << "[DONE] Completed.\n";
    } catch (const std::exception& e) {
        log << "[FATAL] Error: " << e.what() << "\n";
    }

    std::string result = log.str();

    if (hasPipe) {
        // Called by GUI via pipe -> send log to stdout
        std::cout << result;
    } else {
        // Run independently -> display via MessageBox
        MessageBoxA(nullptr, result.c_str(), "Clean Empty Folders", MB_OK | MB_ICONINFORMATION);
    }

    return 0;
}