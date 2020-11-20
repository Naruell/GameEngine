#define CATCH_CONFIG_MAIN
#include <catch.hpp>

/* Engine Codes */
#include "Engine.h"

/* Game Codes */
#include "MapEditor.h"

/* */
#include <iostream>
#include <string>
#include <sstream>
#include "json.h"

TEST_CASE( "TEST INIT")
{
    SECTION("ENGINE INIT")
    {
        Engine::Instance().Init();
    }
}

TEST_CASE( "Returning Reference of Engine / Components" )
{
    SECTION("Engine")
    {
        std::function<Engine& ()> get_engine_func =
                []() -> Engine&
                {
                    Engine& instance = Engine::Instance();
                    return instance;
                };

        Engine& engine = get_engine_func();
        REQUIRE( &engine == &Engine::Instance() );
    }


    SECTION("Timer")
    {
        std::function<Timer& ()> get_timer_func =
                []() -> Timer&
                {
                    Timer& instance = Engine::GetTimer();
                    return instance;
                };

        Timer& timer = get_timer_func();
        REQUIRE( &timer == &Engine::GetTimer() );
    }


    SECTION("Logger")
    {
        std::function<Logger& ()> get_logger_func =
                []() -> Logger&
                {
                    auto & instance = Engine::GetLogger();
                    return instance;
                };

        Logger& logger = get_logger_func();
        logger.LogError("HELLL");
        logger.LogDebug("DEBUG");
        REQUIRE( &logger == &Engine::GetLogger() );
    }
}

TEST_CASE("Engine Test")
{
    SECTION("User Define En/Decryption function")
    {
        std::string data = Engine::GetLoader().LoadStringFromFile("test.txt", Loader::Mode::Default);
        REQUIRE(data.empty() == false);

        REQUIRE(data == Engine::GetLoader().Decrypt(Engine::GetLoader().Encrypt(data)));

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

        REQUIRE(data == Engine::GetLoader().Decrypt(Engine::GetLoader().Encrypt(data)));
    }
}

TEST_CASE("jsoncpp")
{
    const std::string dummyStr = R"({"ID" : 5585711, "Name" : "Naruell"})";

    SECTION("parse_from_string")
    {
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        Json::Value value;
        JSONCPP_STRING err;

        bool isOpen = reader->parse(dummyStr.c_str(), dummyStr.c_str() + dummyStr.size(), &value, &err);

        REQUIRE(isOpen == true);

        if(isOpen)
        {
            REQUIRE(value["ID"].isInt() == true);
            REQUIRE(value["ID"].asInt() == 5585711);

            REQUIRE(value["Name"].isString() == true);
            REQUIRE(value["Name"].asString() == "Naruell");
        }
    }


    SECTION("parse_from_string_stream")
    {
        std::stringstream stringStream;
        stringStream << dummyStr;

        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        Json::Value value;
        JSONCPP_STRING err;

        bool isOpen = Json::parseFromStream(builder, stringStream, &value, &err);

        REQUIRE(isOpen == true);

        if(isOpen)
        {
            REQUIRE(value["ID"].isInt() == true);
            REQUIRE(value["ID"].asInt() == 5585711);

            REQUIRE(value["Name"].isString() == true);
            REQUIRE(value["Name"].asString() == "Naruell");
        }
    }

    SECTION("Remove Nested Array Member")
    {
        std::stringstream ss;
        std::string JsonStr = R"({"Array" : [[0, 1, 2], [3, 4, 5], [6, 7, 8]]})";
        ss << JsonStr;

        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        Json::Value value;
        JSONCPP_STRING err;

        bool isOpen = Json::parseFromStream(builder, ss, &value, &err);
        REQUIRE(isOpen == true);

        for(int y = 0; y < 3; ++y)
        {
            for(int x = 0; x < 3; ++x)
            {
                std::cout << value["Array"][y][x].asInt() << " ";
            }
            std::cout << std::endl;
        }

        Json::Value v1;
        value["Array"][1].removeIndex(1, &v1);
        for(int x = 0; x < 2; ++x)
        {
            std::cout << value["Array"][1][x].asInt() << " ";
        }
        std::cout << std::endl;

        value["Array"][0].insert(0, -3);
        for(int x = 0; x < 4; ++x)
        {
            std::cout << value["Array"][0][x].asInt() << " ";
        }
        std::cout << std::endl;
    }
}

TEST_CASE("Map Editor")
{
    SECTION("Create Level")
    {
        MapEditor::MapEditor me;

        bool isCreated = me.CreateLevel("created_test_map.json", 99, 5, 3);
        REQUIRE(isCreated == true);
    }

    SECTION("Load Level")
    {
        MapEditor::MapEditor me;

        bool isLoaded = me.LoadLevel("created_test_map.json");
        REQUIRE(isLoaded == true);

        me.Draw();
    }

    SECTION("Resize Level")
    {
        MapEditor::MapEditor me;

        bool isLoaded = me.LoadLevel("map_test.json");
        REQUIRE(isLoaded == true);

        me.Draw();
        me.SaveHistory(me.ResizeWidth(7));
        me.Draw();
        std::cout << me.GetRedoCount() << std::endl;
        me.Redo();
        me.Draw();
        std::cout << me.GetRedoCount() << std::endl;

        me.SaveHistory(me.ResizeHeight(7));
        me.Draw();
        std::cout << me.GetRedoCount() << std::endl;
        me.Redo();
        me.Draw();
        std::cout << me.GetRedoCount() << std::endl;
    }
}