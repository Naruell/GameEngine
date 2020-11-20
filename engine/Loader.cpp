#include <fstream>
#include <sstream>
#include "Loader.h"
#include "Engine.h"

std::string Loader::Encrypt(const std::string &String) const
{
    if(mEncryptionFunc != nullptr)
    {
        if(mDecryptionFunc == nullptr)
        {
            Engine::GetLogger().LogWarning("Warning in Loader::Encrypt() : You didn't register decryption function. Encryption and Decryption might be different.");
        }

        return mEncryptionFunc(String);
    }

    srand(mRandSeed);

    std::string encryptedStr;
    encryptedStr.reserve(String.length());

    for(char c : String)
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
            Engine::GetLogger().LogWarning("Warning in Loader::Decrypt() : You didn't register encryption function. Encryption and Decryption might be different.");
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

std::string Loader::LoadStringFromFile(const std::string& filePath, Mode loadMode) const
{
    if(filePath.empty())
    {
        Engine::GetLogger().LogError("Error in Loader::LoadStringFromFile() : File path empty.");
        return "";
    }

    std::ifstream fileCheckStream(filePath, std::ios::binary | std::ios::ate);
    if(!fileCheckStream)
    {
        Engine::GetLogger().LogError("Error in Loader::LoadStringFromFile() : Cannot open file \"" + filePath + "\".");
        return "";
    }

    size_t fileSize = fileCheckStream.tellg();
    std::string fileData;
    fileData.resize(fileSize);
    fileCheckStream.close();

    std::ifstream fileReadStream(filePath);
    std::stringstream stringStream;

    stringStream << fileReadStream.rdbuf();
    fileData = stringStream.str();

    switch (loadMode)
    {
        case Mode::Default:
            break;

        case Mode::Encrypt:
            fileData = Encrypt(fileData);
            break;

        case Mode::Decrypt:
            fileData = Decrypt(fileData);
            break;
    }
    fileReadStream.close();

    return fileData;
}

void Loader::SaveStringToFile(const std::string& filePath, const std::string& data, Loader::Mode saveMode) const
{
    if(filePath.empty())
    {
        Engine::GetLogger().LogError("Error in Loader::SaveStringToFile() : File path empty.");
        return;
    }

    std::ofstream fileWriteStream(filePath);
    if(!fileWriteStream)
    {
        Engine::GetLogger().LogError("Error in Loader::SaveStringToFile() : Cannot open file \"" + filePath + "\".");
        return;
    }

    std::string saveData = data;
    switch (saveMode)
    {
        case Mode::Default:
            break;

        case Mode::Encrypt:
            saveData = Encrypt(saveData);
            break;

        case Mode::Decrypt:
            saveData = Decrypt(saveData);
            break;
    }
    fileWriteStream << saveData;
}