# SPDX-License-Identifier: MIT
#
# CMake configuration for Raspberry Pi Pico
# Proyecto: Ultrasonidos + LEDs + FSM (switch)
#

cmake_minimum_required(VERSION 3.12)

# Import Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

# Nombre del proyecto
project(ultrasonidos C CXX ASM)

# Inicializar el SDK
pico_sdk_init()

# Definir el ejecutable y el archivo fuente
add_executable(ultrasonidos
    proximidad.c
)

# Linkear librerías necesarias
target_link_libraries(ultrasonidos
    pico_stdlib
)

# Habilitar salida USB, deshabilitar UART
pico_enable_stdio_usb(ultrasonidos 1)
pico_enable_stdio_uart(ultrasonidos 0)

# Generar archivos bin/elf/uf2
pico_add_extra_outputs(ultrasonidos)
