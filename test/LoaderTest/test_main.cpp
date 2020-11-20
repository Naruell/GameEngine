#define CATCH_CONFIG_MAIN
#include <catch.hpp>

/* Engine Codes */
#include "Engine.h" // Includes Loader as a Component

/* */
#include <iostream>
#include <vector>
#include <string>

TEST_CASE("Loader")
{
    SECTION("Test Init")
    {
        Engine::Instance().Init();
    }

    SECTION("Default Encrypt & Decrypt")
    {
        std::cout << "--------------------------------- Default Encrypt & Decrypt ---------------------------------" << std::endl;
        std::vector<std::string> testCase =
        {
                R"({I love C/C++!!})",
                R"(~!@#$%^&*()_-+=)",
                R"(ABCDEFGHIJKLMNOPQRSTUVWZYZ)",
                R"(abcdefghijklmnopqrstuvwzyz)",
                R"(ㄱㄴㄷㄹㅁㅂㅅㅇㅈㅊㅋㅌㅍㅎ)"
        };

        for(const std::string& testStr : testCase)
        {
            std::string encryptedStr = Engine::GetLoader().Encrypt(testStr);
            std::string decryptedStr = Engine::GetLoader().Decrypt(testStr);

            std::cout <<  testStr   << std::endl;
            std::cout <<  encryptedStr  << std::endl;
            std::cout <<  decryptedStr  << std::endl;

            std::string refinedStr = Engine::GetLoader().Decrypt(encryptedStr);
            std::cout <<  Engine::GetLoader().Decrypt(refinedStr) << std::endl;

            REQUIRE(testStr == refinedStr);
        }
    }

    SECTION("User Defined Encrypt & Decrypt")
    {
        std::cout << "--------------------------------- User Defined Encrypt & Decrypt ---------------------------------" << std::endl;
        std::string(*customEncryptionFunc)(const std::string&) = [](const std::string& raw)
        {
            std::string encryptedStr;
            for(char c : raw)
            {
                encryptedStr += static_cast<char>(static_cast<int>(c) + 1);
            }
            return encryptedStr;
        };

        std::string(*customDecryptionFunc)(const std::string&) = [](const std::string& raw)
        {
            std::string encryptedStr;
            for(char c : raw)
            {
                encryptedStr += static_cast<char>(static_cast<int>(c) - 1);
            }
            return encryptedStr;
        };

        Engine::GetLoader().RegisterEncryptionFunc(customEncryptionFunc);
        Engine::GetLoader().RegisterDecryptionFunc(customDecryptionFunc);

        std::vector<std::string> testCase =
                {
                        R"({I love C/C++!!})",
                        R"(~!@#$%^&*()_-+=)",
                        R"(ABCDEFGHIJKLMNOPQRSTUVWZYZ)",
                        R"(abcdefghijklmnopqrstuvwzyz)",
                        R"(ㄱㄴㄷㄹㅁㅂㅅㅇㅈㅊㅋㅌㅍㅎ)"
                };

        for(const std::string& testStr : testCase)
        {
            std::string encryptedStr = Engine::GetLoader().Encrypt(testStr);
            std::string decryptedStr = Engine::GetLoader().Decrypt(testStr);

            std::cout <<  testStr   << std::endl;
            std::cout <<  encryptedStr  << std::endl;
            std::cout <<  decryptedStr  << std::endl;

            std::string refinedStr = Engine::GetLoader().Decrypt(encryptedStr);
            std::cout <<  Engine::GetLoader().Decrypt(refinedStr) << std::endl << std::endl;

            REQUIRE(testStr == refinedStr);
        }

    }

    SECTION("Loading File with Each Loader::Mode")
    {
        std::cout << "--------------------------------- Loading File with Each Loader::Mode ---------------------------------" << std::endl;
        const std::string originalFilePath = "Original";
        const std::string encryptedFilePath = "Encrypted";
        const std::string decryptedFilePath = "Decrypted";


        const std::string targetStr = Engine::GetLoader().LoadStringFromFile(originalFilePath, Loader::Mode::Default);
        std::cout << "Target Str : " << targetStr << std::endl << std::endl;

        Engine::GetLoader().RegisterEncryptionFunc(nullptr);
        Engine::GetLoader().RegisterDecryptionFunc(nullptr);

        const std::string encryptedStr = Engine::GetLoader().LoadStringFromFile(encryptedFilePath, Loader::Mode::Decrypt);
        const std::string decryptedStr = Engine::GetLoader().LoadStringFromFile(decryptedFilePath, Loader::Mode::Encrypt);

        REQUIRE(encryptedStr == targetStr);
        REQUIRE(decryptedStr == targetStr);
    }
}