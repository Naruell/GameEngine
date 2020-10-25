// Todo : Add file managing functions
#pragma once
#include <string>

class Loader
{
public:
    [[nodiscard]] std::string Encrypt(const std::string& JSONString) const;
    [[nodiscard]] std::string Decrypt(const std::string& cipherString) const;

    void RegisterEncryptionFunc(std::string(*encryptionFunc)(const std::string&));
    void RegisterDecryptionFunc(std::string(*decryptionFunc)(const std::string&));

private:
    const unsigned int mRandSeed = 1025;
    const unsigned int mRandRange = 77;

    std::string(*mEncryptionFunc)(const std::string&) = nullptr;
    std::string(*mDecryptionFunc)(const std::string&) = nullptr;
};