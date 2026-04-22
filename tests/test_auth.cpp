#include <gtest/gtest.h>
#include <cstdio>
#include <memory>
#include <array>

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
    auto out = run("./auth_app");
    EXPECT_NE(out.find("FAILED"), std::string::npos);
}

TEST(CLI, ValidKey)
{
    std::string cmd = "./auth_app \"" + std::string(VALID_KEY) + "\"";
    auto out = run(cmd);
    EXPECT_NE(out.find("SUCCESS"), std::string::npos);
}

TEST(CLI, InvalidKey)
{
    std::string cmd = "./auth_app \"" + std::string(INVALID_KEY) + "\"";
    auto out = run(cmd);
    EXPECT_NE(out.find("FAILED"), std::string::npos);
}

TEST(CLI, TooManyArgs)
{
    auto out = run("./auth_app a b");
    EXPECT_NE(out.find("FAILED"), std::string::npos);
}