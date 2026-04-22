#include <gtest/gtest.h>
#include <cstdio>
#include <memory>
#include <array>

#ifdef _WIN32
    #define popen _popen
    #define pclose _pclose
#endif

#ifdef _WIN32
    std::string exe_path = "auth_app.exe";
#else
    std::string exe_path = "./auth_app";
#endif

std::string cmd = exe_path + " some_args";

std::string run(const std::string& cmd)
{
    std::array<char, 128> buf;
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";

    while (fgets(buf.data(), buf.size(), pipe))
        result += buf.data();

    pclose(pipe);
    return result;
}

// Валидный публичный ключ
static const char* VALID_KEY = R"(-----BEGIN PUBLIC KEY-----
MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAMab1d4qlEyt7GTStjJwg3jRf97t11BA
WjYnD2iuZ0J19BY1sVR4IqP6aW/k4wFAeAzr5kluslUsOarsNaxSTVsCAwEAAQ==
-----END PUBLIC KEY-----)";

// Невалидный ключ
static const char* INVALID_KEY = R"(-----BEGIN PUBLIC KEY-----
COOLINVALIDKEYFORTESTScoolinvalidkeyfortests123123
-----END PUBLIC KEY-----)";

TEST(CLI, NoArgs)
{
    auto out = run(exe_path);
    EXPECT_NE(out.find("FAILED"), std::string::npos);
}

TEST(CLI, ValidKey)
{
    std::string cmd = exe_path + " \"" + std::string(VALID_KEY) + "\"";
    auto out = run(cmd);
    EXPECT_NE(out.find("SUCCESS"), std::string::npos);
}

TEST(CLI, InvalidKey)
{
    std::string cmd = exe_path + " \"" + std::string(INVALID_KEY) + "\"";
    auto out = run(cmd);
    EXPECT_NE(out.find("FAILED"), std::string::npos);
}

TEST(CLI, TooManyArgs)
{
    auto out = run(exe_path + " a b");
    EXPECT_NE(out.find("FAILED"), std::string::npos);
}