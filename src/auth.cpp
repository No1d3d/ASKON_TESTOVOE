#include "auth.h"

#include <openssl/evp.h>
#include <openssl/pem.h>

#include <vector>

// Embedded private key
static const char* PRIVATE_KEY_PEM = R"(-----BEGIN PRIVATE KEY-----
MIIBVQIBADANBgkqhkiG9w0BAQEFAASCAT8wggE7AgEAAkEAxpvV3iqUTK3sZNK2
MnCDeNF/3u3XUEBaNicPaK5nQnX0FjWxVHgio/ppb+TjAUB4DOvmSW6yVSw5quw1
rFJNWwIDAQABAkAgsuuFRnFgJopzFxYUO94JeESbSYFXnyC7JbctvvkVQdgLxPvH
DFMOcSu0wkWNGvn41MDUixbV6/a7xAZxPUoRAiEA4ycj+dmJ0CviluDoooN6fvoa
nlTtvjoyd8AO/VcWZjUCIQDf1LWTs32TRPuJuO7Q2mLGKav+CucVZGlM0F5IY5IX
TwIgYy/vJKVZ6R2QlsNksbnpnlKFlweMvJja+rFRmkwRytUCIQDdd6z16Noudkng
elAp7HthEWhxY75D52ddLqqrO4IYdwIhAKrf0JqvmVlcb+lHfqW08r/BvxKdlTVL
FbbONKtwrEuf
-----END PRIVATE KEY-----)";

static EVP_PKEY* load_private()
{
    BIO* bio = BIO_new_mem_buf(PRIVATE_KEY_PEM, -1);
    if (!bio) return nullptr;

    EVP_PKEY* key = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    return key;
}

static EVP_PKEY* load_public(const std::string& pem)
{
    BIO* bio = BIO_new_mem_buf(pem.data(), pem.size());
    if (!bio) return nullptr;

    EVP_PKEY* key = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    return key;
}

bool verify_public_key(const std::string& public_key_pem)
{
    EVP_PKEY* priv = load_private();
    EVP_PKEY* pub = load_public(public_key_pem);

    if (!priv || !pub)
    {
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    const std::string msg = "auth_check";

    EVP_MD_CTX* sign_ctx = EVP_MD_CTX_new();
    if (!sign_ctx)
    {
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    if (EVP_DigestSignInit(sign_ctx, nullptr, EVP_sha256(), nullptr, priv) != 1)
    {
        EVP_MD_CTX_free(sign_ctx);
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    EVP_DigestSignUpdate(sign_ctx, msg.data(), msg.size());

    size_t sig_len = 0;
    if (EVP_DigestSignFinal(sign_ctx, nullptr, &sig_len) != 1)
    {
        EVP_MD_CTX_free(sign_ctx);
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    std::vector<unsigned char> sig(sig_len);
    if (EVP_DigestSignFinal(sign_ctx, sig.data(), &sig_len) != 1)
    {
        EVP_MD_CTX_free(sign_ctx);
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    EVP_MD_CTX_free(sign_ctx);

    EVP_MD_CTX* verify_ctx = EVP_MD_CTX_new();
    if (!verify_ctx)
    {
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    if (EVP_DigestVerifyInit(verify_ctx, nullptr, EVP_sha256(), nullptr, pub) != 1)
    {
        EVP_MD_CTX_free(verify_ctx);
        EVP_PKEY_free(priv);
        EVP_PKEY_free(pub);
        return false;
    }

    EVP_DigestVerifyUpdate(verify_ctx, msg.data(), msg.size());

    int ok = EVP_DigestVerifyFinal(verify_ctx, sig.data(), sig_len);

    EVP_MD_CTX_free(verify_ctx);
    EVP_PKEY_free(priv);
    EVP_PKEY_free(pub);

    return ok == 1;
}