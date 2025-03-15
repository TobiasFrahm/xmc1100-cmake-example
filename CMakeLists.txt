cmake_minimum_required(VERSION 3.16)
project(XMC1100_Project LANGUAGES C CXX ASM)

# Setze den Cross-Compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0)

# Pfad zum ARM-GCC Toolchain (Anpassen!)
set(ARM_TOOLCHAIN_PATH "/usr/bin")
# clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_PATH}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_PATH}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${ARM_TOOLCHAIN_PATH}/arm-none-eabi-gcc)

add_definitions(-DXMC1100_Q024x0064)

# Compiler Flags
set(COMMON_FLAGS "-mcpu=cortex-m0 -mthumb -mfloat-abi=soft -ffunction-sections -fdata-sections -Wall -O0")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -Wall -std=gnu99 -Wa,-adhlns -pipe -fmessage-length=0 -g -gdwarf-2" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++98 -fdata-sections -g0 -fno-exceptions -fno-rtti -Wa,-adhlns -pipe -c fmessage-length=0 -g -gdwarf-2" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -T${CMAKE_SOURCE_DIR}/linker_script.ld -nostartfiles -Wl,-Map=${PROJECT_NAME}.map" CACHE STRING "" FORCE)

# Include-Verzeichnisse aus der DAVE IDE Konfiguration
include_directories(
    ${CMAKE_SOURCE_DIR}/Libraries/XMCLib/inc
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Infineon/XMC1100_series/Include
    ${CMAKE_SOURCE_DIR}/Dave/Generated
    ${CMAKE_SOURCE_DIR}/Libraries
)

# Füge deine Quellcode-Dateien hinzu (Anpassen!)
file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_SOURCE_DIR}/Libraries/Newlib/*.c
    ${CMAKE_SOURCE_DIR}/Startup/*.c
    ${CMAKE_SOURCE_DIR}/Startup/*.S
    ${CMAKE_SOURCE_DIR}/main.c
    #Dave Code
    ${CMAKE_SOURCE_DIR}/Dave/Generated/*.c
)

# Entferne CMake-Erkennungsdateien
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*CMakeFiles.*")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*CompilerIdC.*")
message(STATUS "Quellcode-Dateien: ${PROJECT_SOURCES}")

add_executable(${PROJECT_NAME}.elf EXCLUDE_FROM_ALL ${PROJECT_SOURCES})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)
message(STATUS "Target erstellt: ${PROJECT_NAME}.elf mit Quellen: ${PROJECT_SOURCES}")

# Linker-Einstellungen
target_link_libraries(${PROJECT_NAME}.elf PRIVATE m)

# Erzeuge zusätzliche Binärdateien (HEX und BIN)
add_custom_target(${PROJECT_NAME}.hex ALL
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND ${ARM_TOOLCHAIN_PATH}/arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
)

add_custom_target(${PROJECT_NAME}.bin ALL
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND ${ARM_TOOLCHAIN_PATH}/arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

# Größenanalyse
add_custom_target(size ALL
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND ${ARM_TOOLCHAIN_PATH}/arm-none-eabi-size -B ${PROJECT_NAME}.elf
)
