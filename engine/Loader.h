// Todo : Implement other file managing methods (Image, Font, etc...)
// Todo : Confirm how to give programmer information of the error (Currently I'm considering throwing an exception.)
// Todo : srand and rand function in default Encrypt and Decrypt function could possibly changed to Engine's Random Component.
#pragma once
#include <string>

class Loader
{
public:
    enum class Mode
    {
        Default,
        Encrypt,
        Decrypt
    };

    [[nodiscard]] std::string Encrypt(const std::string& String) const;
    [[nodiscard]] std::string Decrypt(const std::string& cipherString) const;

    void RegisterEncryptionFunc(std::string(*encryptionFunc)(const std::string&));
    void RegisterDecryptionFunc(std::string(*decryptionFunc)(const std::string&));

    [[nodiscard]] std::string LoadStringFromFile(const std::string& filePath, Mode loadMode = Mode::Decrypt) const;
    void                      SaveStringToFile(const std::string& filePath, const std::string& data, Mode saveMode = Mode::Encrypt) const;

    /* Examples(To be implemented) for loader methods */
    // [[nodiscard]] Image LoadImageFromPNGFile(const std::string& filePath, Mode loadMode = Mode::Default) const;
    // [[nodiscard]] Font  LoadFontFromFile(const std::string& filePath, Mode loadMode = Mode::Default) const;

private:
    const unsigned int mRandSeed = 1025;
    const unsigned int mRandRange = 77;

    std::string(*mEncryptionFunc)(const std::string&) = nullptr;
    std::string(*mDecryptionFunc)(const std::string&) = nullptr;
};