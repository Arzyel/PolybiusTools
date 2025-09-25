# ModTool

An open-source modding tool for Paradox games mainly eu4, written in C++ using Qt.

---

## Requirements

- Qt 5.15.2 (MSVC 2019 64-bit)
- Visual Studio 2022 (C++20)
- CMake 3.25+ or Qt Creator 5.x

---

## License

- Source code: MIT License ([LICENSE](LICENSE))  
- Qt 5 libraries: LGPL v3 ([LICENSE_QT](LICENSE_QT))  

---

## Building

### Qt Creator
1. Open `ModTool.pro`.  
2. Select **MSVC 2022**.  
3. Build in **Release mode**.

### Visual Studio
1. Open `.vcxproj` or create a new VS project.  
2. Configure **Qt paths** via Qt VS Tools extension.  
3. Build in **Release mode**.

---

## Running

1. Copy `ModTool.exe` from the Release folder.  
2. Copy required Qt DLLs (release, not debug) from your Qt MSVC installation into the same folder.  
3. Launch `ModTool.exe`.  

**Tip:** Use `windeployqt ModTool.exe` to automatically gather DLLs.

---

## Optional: Release Zip

Include:

- `ModTool.exe`
- Release Qt DLLs
- `LICENSE` + `LICENSE_QT`
