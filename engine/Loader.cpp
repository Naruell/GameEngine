#include "Loader.h"
#include "Engine.h"

std::string Loader::Encrypt(const std::string &JSONString) const
{
    if(mEncryptionFunc != nullptr)
    {
        if(mDecryptionFunc == nullptr)
        {
            Engine::GetLogger().LogError("Error in Loader::Encrypt() : You didn't register decryption function. Encryption and Decryption might be different.");
        }

        return mEncryptionFunc(JSONString);
    }

    srand(mRandSeed);

    std::string encryptedStr;
    encryptedStr.reserve(JSONString.length());

    for(char c : JSONString)
    {
        encryptedStr += static_cast<char>(static_cast<int>(c) + rand() % mRandRange);
    }

    return encryptedStr;
}

std::string Loader::Decrypt(const std::string &cipherString) const
{
    if(mDecryptionFunc != nullptr)
    {
        if(mEncryptionFunc == nullptr)
        {
            Engine::GetLogger().LogError("Error in Loader::Decrypt() : You didn't register encryption function. Encryption and Decryption might be different.");
        }

        return mDecryptionFunc(cipherString);
    }

    srand(mRandSeed);

    std::string decryptedStr;
    decryptedStr.reserve(cipherString.length());

    for(char c : cipherString)
    {
        decryptedStr += static_cast<char>(static_cast<int>(c) - rand() % mRandRange);
    }

    return decryptedStr;
}

void Loader::RegisterEncryptionFunc(std::string (*encryptionFunc)(const std::string &))
{
    mEncryptionFunc = encryptionFunc;
}

void Loader::RegisterDecryptionFunc(std::string (*decryptionFunc)(const std::string &))
{
    mDecryptionFunc = decryptionFunc;
}

