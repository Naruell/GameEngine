#include "MapEditor.h"

// Temporary Libraries for Prototyping
#include <iostream>

#include <sstream>
#include <memory>
#include "Engine.h"

namespace MapEditor
{
    bool MapEditor::LoadLevel(const std::string& filePath)
    {
        if(mIsOpened)
        {
            Engine::GetLogger().LogError("Error in MapEditor::LoadLevel() : Map is already opened. If you want to load another level, you have to close it first.");
            return false;
        }

        std::stringstream ss;
        ss << Engine::GetLoader().LoadStringFromFile(filePath, Loader::Mode::Default);

        /* External Library section */
        {
            Json::CharReaderBuilder builder;
            builder["collectComments"] = false;
            JSONCPP_STRING err;

            bool isLoaded = Json::parseFromStream(builder, ss, &mJsonMapData, &err);
            if(!isLoaded)
            {
                Engine::GetLogger().LogError("Error in MapEditor::LoadLevel() : Failed to parse Json from \"" + filePath + "\".");
                return false;
            }
        }

        while(!mEditHistory.empty())
        {
            mEditHistory.pop();
        }

        while(!mRedoCount.empty())
        {
            mRedoCount.pop();
        }

        mIsOpened = true;
        mMapFilePath = filePath;
        return true;
    }

    void MapEditor::SaveLevel()
    {
        std::string jsonResult;
        {
            std::stringstream ss;

            Json::StreamWriterBuilder builder;
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            writer->write(mJsonMapData, &ss);

            jsonResult = ss.str();
        }

        Engine::GetLoader().SaveStringToFile(mMapFilePath, jsonResult, Loader::Mode::Default);
    }

    void MapEditor::CloseLevel()
    {
        /* External Library section */
        {
            mJsonMapData.clear();
        }

        while(!mEditHistory.empty())
        {
            mEditHistory.pop();
        }

        while(!mRedoCount.empty())
        {
            mRedoCount.pop();
        }

        mIsOpened = false;
        mMapFilePath.clear();
    }

    bool MapEditor::CreateLevel(const std::string &filePath, int floor, int width, int height) const
    {
        std::string jsonResult;

        /* External Library Section */
        {
            Json::Value newMap;
            newMap["FLOOR"] = floor;
            newMap["WIDTH"] = width;
            newMap["HEIGHT"] = height;

            Json::Value tile = Json::arrayValue;;
            for(int y = 0; y < height; ++y)
            {
                Json::Value row = Json::arrayValue;;
                for(int x = 0; x < width; ++x)
                {
                    row.append(0);
                }
                tile.append(row);
            }
            newMap["TILE"] = tile;

            newMap["OBJECT"] = Json::arrayValue;

            std::stringstream ss;

            Json::StreamWriterBuilder builder;
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            writer->write(newMap, &ss);

            jsonResult = ss.str();
        }

        Engine::GetLoader().SaveStringToFile(filePath, jsonResult, Loader::Mode::Default);
        return true;
    }

    void MapEditor::Draw()
    {
        /* Prototyping with Console */
        /* External Library section */
        {
            system("cls");
            std::cout << "FLOOR : " << mJsonMapData["FLOOR"].asInt() << std::endl
                      << "WIDTH : " << mJsonMapData["WIDTH"].asInt() << std::endl
                      << "HEIGHT : " << mJsonMapData["HEIGHT"].asInt() << std::endl << std::endl;

            std::cout << "TILE : " << std::endl;
            for(int y = 0; y < mJsonMapData["HEIGHT"].asInt(); ++y)
            {
                for(int x = 0; x < mJsonMapData["WIDTH"].asInt(); ++x)
                {
                    std::cout << mJsonMapData["TILE"][y][x].asInt() << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << "OBJECT : " << std::endl;
            for(int i = 0; i < mJsonMapData["OBJECT"].size(); ++i)
            {
                std::cout << "\"" + mJsonMapData["OBJECT"][i]["TYPE"].asString() + "\" : ";
                std::cout << "{ " + mJsonMapData["OBJECT"][i]["X"].asString() + ", " + mJsonMapData["OBJECT"][i]["Y"].asString() + " }, ";
            }
            std::cout << std::endl;

            std::cout << "Redo Count : " << GetRedoCount() << std::endl;
            std::cout << "Redo Count Stack : " << GetRedoCountStackSize() << std::endl << std::endl;

            std::cout << "Command List : [Resize, ResizeWidth, ResizeHeight, SetTile, AddObject, DeleteObject, SwapObject, Redo, Save]" << std::endl;
            std::cout << "Please Enter the Command : ";
        }
    }

    // Temporary Hash Func for string compare
    constexpr unsigned int HashFunc(const char* str)
    {
        return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * HashFunc(str + 1) : 8603;
    }

    void MapEditor::Update()
    {
        /* Prototyping with Console */
        {
            std::string command;
            int arg1, arg2, arg3, arg4;
            std::cin >> command;

            switch(HashFunc(command.c_str()))
            {
                case HashFunc("Resize"):
                    std::cout << "Please Input Width and Height : ";
                    std::cin >> arg1 >> arg2;
                    SaveHistory(Resize(arg1, arg2));
                    break;

                case HashFunc("ResizeWidth"):
                    std::cout << "Please Input Width : ";
                    std::cin >> arg1;
                    SaveHistory(ResizeWidth(arg1));
                    break;

                case HashFunc("ResizeHeight"):
                    std::cout << "Please Input Height : ";
                    std::cin >> arg1;
                    SaveHistory(ResizeHeight(arg1));
                    break;

                case HashFunc("SetTile"):
                    std::cout << "Please Input TileID, posX and PosY : ";
                    std::cin >> arg1 >> arg2 >> arg3;
                    SaveHistory(SetTile(static_cast<TileType>(arg1), Vector2DInt{arg2, arg3}));
                    break;

                case HashFunc("AddObject"):
                    std::cout << "Please Input ObjectID, posX and PosY : ";
                    std::cin >> arg1 >> arg2 >> arg3;
                    SaveHistory(AddObject(static_cast<ObjectType>(arg1), Vector2DInt{arg2, arg3}));
                    break;

                case HashFunc("DeleteObject"):
                    std::cout << "Please Input Object posX and posY : ";
                    std::cin >> arg1 >> arg2;
                    SaveHistory(DeleteObject(Vector2DInt{arg1, arg2}));
                    break;

                case HashFunc("SwapObject"):
                    std::cout << "Please Input Object1 posX, posY and Object2 posX, posY : ";
                    std::cin >> arg1 >> arg2 >> arg3 >> arg4;
                    SaveHistory(SwapObject(Vector2DInt{arg1, arg2}, Vector2DInt{arg3, arg4}));
                    break;

                case HashFunc("Redo"):
                    Redo();
                    break;

                case HashFunc("Save"):
                    SaveLevel();
                    break;

                default:
                    Engine::GetLogger().LogError("Error in MapEditor::Update() : Unknown Command.");
                    break;
            }
        }

    }

    void MapEditor::Redo()
    {
        if(mEditHistory.empty())
        {
            Engine::GetLogger().LogWarning("Warning in MapEditor::Redo() : Edit History is empty.");
            return;
        }

        int redoCount = mRedoCount.top();
        mRedoCount.pop();

        while(redoCount > 0)
        {
            EditHistory history = mEditHistory.top();
            mEditHistory.pop();
            switch(history.mEditCommand)
            {
                case EditCommand::None:
                    break;

                case EditCommand::ResizeWidth:
                    ResizeWidth(history.mArgument1.value, true);
                    break;

                case EditCommand::ResizeHeight:
                    ResizeHeight(history.mArgument1.value, true);
                    break;

                case EditCommand::AddTile:
                    AddTile(history.mArgument1.tileType, history.mArgument2.location, true);
                    break;

                case EditCommand::DeleteTile:
                    DeleteTile(history.mArgument1.location, true);
                    break;

                case EditCommand::SetTile:
                    SetTile(history.mArgument1.tileType, history.mArgument2.location, true);
                    break;

                case EditCommand::AddObject:
                    AddObject(history.mArgument1.objectType, history.mArgument2.location, true);
                    break;

                case EditCommand::DeleteObject:
                    DeleteObject(history.mArgument1.location, true);
                    break;

                case EditCommand::SwapObject:
                    SwapObject(history.mArgument1.location, history.mArgument2.location, true);
                    break;
            }
            redoCount--;
        }
    }

    void MapEditor::SaveHistory(EditHistory history)
    {
        mEditHistory.push(history);
    }

    EditHistory MapEditor::Resize(int width, int height, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        EditHistory history;
        history.mEditCommand = EditCommand::None;
        mRedoCount.top()++;

        SaveHistory(ResizeWidth(width));
        SaveHistory(ResizeHeight(height));

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::ResizeWidth(int width, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        int originalWidth = mJsonMapData["WIDTH"].asInt();

        EditHistory history;
        history.mEditCommand = EditCommand::ResizeWidth;
        history.mArgument1.value = originalWidth;
        if(!isRedo) { mRedoCount.top()++; }

        if(width <= 0)
        {
            Engine::GetLogger().LogError("Error in MapEditor::ResizeWidth : Width cannot be equal or less than zero.");
            return history;
        }

        if(!isRedo)
        {
            if(width != originalWidth)
            {
                if(width > originalWidth)
                {
                    for(int y = 0; y < mJsonMapData["HEIGHT"].asInt(); ++y)
                    {
                        for(int x = originalWidth; x < width; ++x)
                        {
                            SaveHistory(AddTile(TileType::EMPTY, Vector2DInt{x, y}));
                        }
                    }
                }
                else
                {
                    for(int y = 0; y < mJsonMapData["HEIGHT"].asInt(); ++y)
                    {
                        for(int x = originalWidth - 1; x >= width; --x)
                        {
                            SaveHistory(DeleteTile(Vector2DInt{x, y}));
                        }
                    }
                }
            }
        }

        mJsonMapData["WIDTH"] = width;

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::ResizeHeight(int height, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        int originalHeight = mJsonMapData["HEIGHT"].asInt();

        EditHistory history;
        history.mEditCommand = EditCommand::ResizeHeight;
        history.mArgument1.value = originalHeight;
        if(!isRedo) { mRedoCount.top()++; }

        if(height <= 0)
        {
            Engine::GetLogger().LogError("Error in MapEditor::ResizeHeight : Height cannot be equal or less than zero.");
            return history;
        }

        if(!isRedo)
        {
            if(height != originalHeight)
            {
                if(height > originalHeight)
                {
                    for(int y = mJsonMapData["HEIGHT"].asInt(); y < height; ++y)
                    {
                        for(int x = 0; x < mJsonMapData["WIDTH"].asInt(); ++x)
                        {
                            SaveHistory(AddTile(TileType::EMPTY, Vector2DInt{x, y}));
                        }
                    }
                }
                else
                {
                    for(int y = mJsonMapData["HEIGHT"].asInt() - 1; y >= height; --y)
                    {
                        for(int x = mJsonMapData["WIDTH"].asInt() - 1; x >= 0; --x)
                        {
                            SaveHistory(DeleteTile(Vector2DInt{x, y}));
                        }
                    }
                }
            }
        }

        mJsonMapData["HEIGHT"] = height;

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::AddTile(TileType tileType, Vector2DInt position, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        EditHistory history;
        history.mEditCommand = EditCommand::DeleteTile;
        history.mArgument1.location = position;
        if(!isRedo) { mRedoCount.top()++; }

        mJsonMapData["TILE"][position.y].insert(position.x, static_cast<int>(tileType));

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::DeleteTile(Vector2DInt position, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        EditHistory history;
        history.mEditCommand = EditCommand::AddTile;
        history.mArgument1.tileType = static_cast<TileType>(mJsonMapData["TILE"][position.y][position.x].asInt());
        history.mArgument2.location = position;
        if(!isRedo) { mRedoCount.top()++; }

        Json::Value temp;
        mJsonMapData["TILE"][position.y].removeIndex(position.x, &temp);
        /* Removing Empty Raw */
        if(mJsonMapData["TILE"][position.y].empty())
        {
            mJsonMapData["TILE"].removeIndex(position.y, &temp);
        }

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::SetTile(TileType tileType, Vector2DInt position, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        EditHistory history;
        history.mEditCommand = EditCommand::SetTile;
        history.mArgument1.tileType = static_cast<TileType>(mJsonMapData["TILE"][position.y][position.x].asInt());
        history.mArgument2.location = position;
        if(!isRedo) { mRedoCount.top()++; }

        mJsonMapData["TILE"][position.y][position.x] = static_cast<int>(tileType);

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::AddObject(ObjectType objectType, Vector2DInt position, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        // Map Editor Action Here
        EditHistory history;
        history.mEditCommand = EditCommand::DeleteObject;
        history.mArgument1.location = Vector2DInt{position.x, position.y};
        if(!isRedo) { mRedoCount.top()++; }

        Json::Value newObject = Json::objectValue;
        newObject["TYPE"] = static_cast<int>(objectType);
        newObject["X"] = position.x;
        newObject["Y"] = position.y;
        mJsonMapData["OBJECT"].append(newObject);

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::DeleteObject(Vector2DInt position, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        Json::Value target;
        int targetIndex;
        for(int i = 0; i < mJsonMapData["OBJECT"].size(); ++i)
        {
            if(mJsonMapData["OBJECT"][i]["X"].asInt() == position.x &&
               mJsonMapData["OBJECT"][i]["Y"].asInt() == position.y)
            {
                target = mJsonMapData["OBJECT"][i];
                targetIndex = i;
            }
        }

        EditHistory history;
        history.mEditCommand = EditCommand::AddObject;
        history.mArgument1.objectType = static_cast<ObjectType>(target["TYPE"].asInt());
        history.mArgument2.location = Vector2DInt{position.x, position.y};
        if(!isRedo) { mRedoCount.top()++; }

        Json::Value temp;
        mJsonMapData["OBJECT"].removeIndex(targetIndex, &temp);

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }

    EditHistory MapEditor::SwapObject(Vector2DInt object1pos, Vector2DInt object2pos, bool isRedo)
    {
        bool isOpenFromHere = false;
        if(!mRedoCountOpen && !isRedo)
        {
            isOpenFromHere = true;
            mRedoCount.push(0);
            mRedoCountOpen = true;
        }

        // Map Editor Action Here
        EditHistory history;
        history.mEditCommand = EditCommand::SwapObject;
        history.mArgument1.location = Vector2DInt{object1pos.x, object1pos.y};
        history.mArgument2.location = Vector2DInt{object2pos.x, object2pos.y};
        if(!isRedo) { mRedoCount.top()++; }

        Json::Value target1;
        Json::Value target2;
        int target1Index;
        int target2Index;

        for(int i = 0; i < mJsonMapData["OBJECT"].size(); ++i)
        {
            if(mJsonMapData["OBJECT"][i]["X"].asInt() == object1pos.x &&
               mJsonMapData["OBJECT"][i]["Y"].asInt() == object1pos.y)
            {
                target1 = mJsonMapData["OBJECT"][i];
                target1Index = i;
            }

            else if(mJsonMapData["OBJECT"][i]["X"].asInt() == object2pos.x &&
                    mJsonMapData["OBJECT"][i]["Y"].asInt() == object2pos.y)
            {
                target2 = mJsonMapData["OBJECT"][i];
                target2Index = i;
            }
        }

        mJsonMapData["OBJECT"][target1Index]["X"] = target2["X"];
        mJsonMapData["OBJECT"][target1Index]["Y"] = target2["Y"];
        mJsonMapData["OBJECT"][target2Index]["X"] = target1["X"];
        mJsonMapData["OBJECT"][target2Index]["Y"] = target1["Y"];

        if(isOpenFromHere)
        {
            mRedoCountOpen = false;
        }
        return history;
    }
}