// Todo : Change Load & create level's loader mode (currently default mode.)
// Todo : Replace Vector2DInt, TileType and ObjectType (After merge to SKEngine.)
// Todo : Remove #include<iostream> (after Graphic parts are implemented.)
// Todo : Change Map Editing Method field as private (after Graphic parts are implemented.)
// Todo : Remove Temporary Methods for debugging
// Todo : Remove Hard Coding for Json parser ("WIDTH" for example.)
#pragma once
#include <string>
#include <stack>

/* External Library */
#include "json.h"

// Should be replaced
struct Vector2DInt
{
    int x;
    int y;
};

// Should be replaced
enum class ObjectType
{
    KEY,
    DOOR,
    TREASURE
};

// Should be replace
enum class TileType
{
    EMPTY,
    WALL
};

namespace MapEditor
{
    enum class EditCommand
    {
        None,
        ResizeWidth,
        ResizeHeight,
        AddTile,
        DeleteTile,
        SetTile,
        AddObject,
        DeleteObject,
        SwapObject
    };

    union EditHistoryArg
    {
        int value;
        Vector2DInt location;
        ObjectType objectType;
        TileType tileType;
    };

    struct EditHistory
    {
        EditCommand mEditCommand;

        union EditHistoryArg mArgument1;
        union EditHistoryArg mArgument2;
    };

    class MapEditor
    {
    public:
        /* General Class Method */
        [[nodiscard]] bool LoadLevel(const std::string& filePath); /* Use External Library */
        void               SaveLevel(); /* Use External Library */
        void               CloseLevel(); /* Use External Library */
        [[nodiscard]] bool CreateLevel(const std::string& filePath, int floor, int width, int height) const; /* Use External Library */

        void               Draw(); /* Use External Library */
        void               Update();

    // Should be changed private after Graphic parts are implemented.
    public:
        /* Map Editing Method */
        void               SaveHistory(EditHistory history);

        void               Redo();

        EditHistory        Resize(int width, int height, bool isRedo = false);
        EditHistory        ResizeWidth(int width, bool isRedo = false); /* Use External Library */
        EditHistory        ResizeHeight(int height, bool isRedo = false); /* Use External Library */
        EditHistory        AddTile(TileType tileType, Vector2DInt position, bool isRedo = false); /* Use External Library */
        EditHistory        DeleteTile(Vector2DInt position, bool isRedo = false); /* Use External Library */
        EditHistory        SetTile(TileType tileType, Vector2DInt position, bool isRedo = false);

        EditHistory        AddObject(ObjectType objectType, Vector2DInt position, bool isRedo = false); /* Use External Library */
        EditHistory        DeleteObject(Vector2DInt position, bool isRedo = false); /* Use External Library */
        EditHistory        SwapObject(Vector2DInt object1pos, Vector2DInt object2pos, bool isRedo = false); /* Use External Library */

    // Temporary Methods for debugging
    public:
        [[nodiscard]] int  GetRedoCount() const noexcept { if(mRedoCount.empty()) return 0; return mRedoCount.top(); };
        [[nodiscard]] int  GetRedoCountStackSize() const noexcept { return mRedoCount.size(); }

    private:
        bool                    mIsOpened = false;
        std::stack<EditHistory> mEditHistory;
        std::stack<int>         mRedoCount;
        bool                    mRedoCountOpen = false;
        std::string             mMapFilePath;

    private:
        Json::Value             mJsonMapData; /* Use External Library */
    };
}