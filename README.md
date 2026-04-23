# ASKON_TESTOVOE
Test Task For Askon

## Описание
Консольное приложение на C++, которое проверяет авторизацию по публичному ключу. Приватный ключ зашит в коде.

## Сборка и запуск

### Требования
- CMake 3.20+
- Conan 2.x
- Компилятор: Windows - MSVS / Linux - GCC

### Шаги сборки
   # windows
   ```
   conan install . -of build --build=missing
   cmake --preset conan-default
   cmake --build --preset conan-release
   ctest --preset conan-release --output-on-failure -V
   ```
   
   # linux
   ```
   conan profile detect --force
   conan install . --build=missing
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   cd build && ctest --output-on-failure -V
   ```

Вывод: "Authorization SUCCESS" или "Authorization FAILED"
