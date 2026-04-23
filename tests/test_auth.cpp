#include <gtest/gtest.h>
#include "auth.h"

// Valid public key
static const char* VALID_KEY = R"(-----BEGIN PUBLIC KEY-----
MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAMab1d4qlEyt7GTStjJwg3jRf97t11BA
WjYnD2iuZ0J19BY1sVR4IqP6aW/k4wFAeAzr5kluslUsOarsNaxSTVsCAwEAAQ==
-----END PUBLIC KEY-----)";

// Invalid key
static const char* INVALID_KEY = R"(-----BEGIN PUBLIC KEY-----
INVALIDKEYDATA
-----END PUBLIC KEY-----)";

TEST(Auth, ValidKey)
{
    EXPECT_TRUE(verify_public_key(VALID_KEY));
}

TEST(Auth, InvalidKey)
{
    EXPECT_FALSE(verify_public_key(INVALID_KEY));
}

TEST(Auth, EmptyKey)
{
    EXPECT_FALSE(verify_public_key(""));
}