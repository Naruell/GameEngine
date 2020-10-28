#include <cassert>
#include <functional>

#include "Engine.h"

int main()
{
    Engine::Instance().Init();

    Engine::GetLogger().LogDebug("Hello! world!");
    Engine::Instance().SetTargetFPS(60);

    Engine::GetTimer().RegisterTick("Hi");
    Engine::GetLogger().LogDebug("Tick for \"Hi\" : " + std::to_string(Engine::GetTimer().GetDeltaTick("Hi")));
    Engine::Instance().Update();
    Engine::GetLogger().LogDebug("Tick for \"Hi\" : " + std::to_string(Engine::GetTimer().GetDeltaTick("Hi")));

    std::string dummyData = R"({"Data" : "Hello World!"})";
    std::string encryptedData = Engine::GetLoader().Encrypt(dummyData);
    std::string decryptedData = Engine::GetLoader().Decrypt(encryptedData);
    Engine::GetLogger().LogDebug("Encrypted data : " + encryptedData);
    Engine::GetLogger().LogDebug("Decrypted data : " + decryptedData);

    /* Code for encrypting data file */
    /*
    Engine::GetLoader().SetFileData(encryptedData);
    if(Engine::GetLoader().SaveDataFile(Loader::Mode::Default))
    {
        Engine::GetLogger().LogEvent("Successfully saved data");
    }
    */
    std::string JSONStr = Engine::GetLoader().LoadStringFromFile("savefile.txt");
    Engine::GetLogger().LogDebug("Loaded data from file : " + JSONStr);

    while(Engine::GetTimer().GetElapsedTime() <= 10)
    {
        Engine::Instance().Update();
    }

    Engine::GetLogger().PrintLogCount();
    Engine::Instance().Shutdown();

    return 0;
}