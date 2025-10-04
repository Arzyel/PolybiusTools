#ifndef FILE_OPENER_H
#define FILE_OPENER_H

#include <string>

#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS 1
#include <windows.h>
#include <shellapi.h>
#else
#define PLATFORM_WINDOWS 0
#include <cstdlib>
#endif

class FileOpener {
public:
    /**
     * Opens a text file with the system's default text editor
     */
    static bool openTextFile(const std::string& filepath) {
#if PLATFORM_WINDOWS
        return openTextFileWindows(filepath);
#elif __APPLE__
        return openTextFileMac(filepath);
#else
        return openTextFileLinux(filepath);
#endif
    }

private:
    // Windows - Use Notepad
    static bool openTextFileWindows(const std::string& filepath) {
        std::wstring wide_path(filepath.begin(), filepath.end());
        HINSTANCE result = ShellExecuteW(NULL, L"open", wide_path.c_str(), NULL, NULL, SW_SHOW);

        if ((INT_PTR)result > 32) {
            return true;
        }

        // Fallback to direct notepad command
        std::string command = "notepad \"" + filepath + "\"";
        return (system(command.c_str()) == 0);
    }

    // Linux - Use xdg-open (system default)
    static bool openTextFileLinux(const std::string& filepath) {
        std::string command = "xdg-open \"" + filepath + "\"";
        return (system(command.c_str()) == 0);
    }

    // macOS - Use open -t (default text editor)
    static bool openTextFileMac(const std::string& filepath) {
        std::string command = "open -t \"" + filepath + "\"";
        return (system(command.c_str()) == 0);
    }
};

#endif // FILE_OPENER_H