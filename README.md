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
   conan profile detect --force
   cmake --preset conan-release
   del /f CMakeUserPresets.json
   cmake --build . --preset conan-release
   ctest --preset conan-release --output-on-failure -V
   ```
   
   # linux
   ```
   conan profile detect --force
   cmake --preset conan-release
   rm -rf CMakeUserPresets.json
   cmake --build . --preset conan-release
   ctest --preset conan-release --output-on-failure -V
   ```

Вывод: "Authorization SUCCESS" или "Authorization FAILED"
