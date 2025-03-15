# XMC1100 Minimal Example for VS Code (Linux)

This repository contains a minimal example for compiling, flashing, and debugging source code for the **Infineon XMC1100** microcontroller using **VS Code** under **Linux**. The project was migrated from **DAVE IDE** and is set up to work with the **ARM GCC toolchain** and the **J-Link GDB Debugger**.

## Prerequisites

Before using this repository, ensure you have installed the following dependencies:

1. **ARM GCC Toolchain**

   - Install via package manager:
     ```sh
     sudo apt install gcc-arm-none-eabi
     ```
   - Alternatively, download from [Arm Developer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

2. **J-Link Debugger (Segger J-Link Software & Driver Package)**

   - Download from [Segger](https://www.segger.com/downloads/jlink/)
   - Install following the instructions for Linux

3. **VS Code**

   - Install from [VS Code official website](https://code.visualstudio.com/)
   - Required Extensions:
     - **C/C++** (by Microsoft)
     - **Cortex-Debug** (for debugging with J-Link)

4. **CMake & Ninja**

   - Install via package manager:
     ```sh
     sudo apt install cmake ninja-build
     ```

## Project Structure

```
├── .vscode/              # VS Code settings and tasks
├──────── cmake-kits.json
├──────── launch.json     # Debugger configuration for Cortex-Debug
├──────── settings.json   # CMake Config    
├──────── tasks.json      # Build/Clean/JlinkGDBExe
├── main.c                # Aplication Entry
├── Dave/Generated        # DaveIDE Generated Sources
├── build/                # Build directory
├── CMakeLists.txt        # CMake build system (if used)
├── Makefile              # Makefile for building the project
├── linker_script.ld      # Linkerscript for XMCxxxx series
├── build.sh              # clean build shell script
└── README.md             # This file
```

## Setup & Usage

### 1. Cloning the Repository

```sh
git clone <repository-url>
cd <repository-folder>
```

### 2. Building the Project

Using the script:

```sh
./build.sh
```

Step by Step:

```sh
rm -rf build/
cmake -B build -GNinja
nincmake --build buildja
```

### 3. Flash and Debugging with VS Code

1. Open the project in VS Code.
2. Configure the debugger in `.vscode/launch.json`:
   ```json
   {
     "configurations": [
        {
            "name": "Debug XMC1100 (J-Link SWD)",
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "jlink",
            "device": "XMC1100",
            "interface": "swd",
            "runToEntryPoint": "main",
            "cwd": "${workspaceFolder}",
            "executable": "${workspaceFolder}/build/XMC1100_Project.elf",
            "serverArgs": [
                "-if", "SWD",
                "-speed", "1000"
            ],
            "svdFile": "${workspaceFolder}/XMC1100.svd",
            "postLaunchCommands": [
                "monitor reset halt",
                "load",
                "monitor reset",
                "break main",
                "continue"
            ],
            "showDevDebugOutput": "raw"
        }
    ]
   }
   ```
3. Start debugging via **Run → Start Debugging** or press `F5`.

## Notes

- Ensure that `JLinkGDBServer` is running before debugging, see `.vscode/tasks.json`
- The project is configured for a **migrated DAVE IDE setup**. If you need to adjust paths, check the `Makefile`, `CMakeLists.txt`, and `.vscode/` settings.

## License

Just use it if you need it.

## Contributions

Feel free to submit issues or pull requests to improve this repository.

## References

- [Infineon XMC1100 Documentation](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/xmc1000/xmc1100/)
- [Segger J-Link](https://www.segger.com/products/debug-probes/j-link/)
- [GNU ARM Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

