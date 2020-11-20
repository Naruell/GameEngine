#pragma once
#include <vector>
#include <memory>
#include "DataType.h"

namespace PathFinding
{
    struct Field
    {
        Field(int width, int height, const std::vector<short>& field) : mWidth(width), mHeight(height), mVector(field) {}

        int mWidth, mHeight;
        /* zero or less for blocked area, higher number for more costs */
        std::vector<short> mVector;
    };

    class Node
    {
    public:
        Node(Vector2DInt startPos, Vector2DInt destinationPos, int depth);
        Node(Vector2DInt startPos, int depth);
        Node(const Node& other) = default;

    public:
        [[nodiscard]] bool  IsSameLocation(const Node& other) const noexcept;
        void                CalculateDistance(const Node& destination) noexcept;

        [[nodiscard]] Vector2DInt            GetLocation() const noexcept;
        [[nodiscard]] int                    GetDistance() const noexcept;
        void                                 SetDepth(int depth) noexcept;
        [[nodiscard]] int                    GetDepth() const noexcept;
        void                                 SetParent(std::unique_ptr<Node>* parent) noexcept;
        [[nodiscard]] std::unique_ptr<Node>* GetParent() noexcept;

    private:
        Vector2DInt mLocation;
        int mDistance;
        int mDepth;

        std::unique_ptr<Node>* mParent = nullptr;
    };

    class Module
    {
    public:
        Module(const Node& startNode, const Node& endNode, const Field& field);

        void               Set(const Node& startNode, const Node& endNode, const Field& field);
        void               Reset();
        [[nodiscard]] bool Run();

        [[nodiscard]] std::vector<Node>& GetPath();

    private:
        [[nodiscard]] static bool CompareNode(const std::unique_ptr<Node>& node1, const std::unique_ptr<Node>& node2) noexcept;

        void InsetOpenNodeList(std::unique_ptr<Node>& node) noexcept;
        void InsetCloseNodeList(std::unique_ptr<Node>& node) noexcept;

        [[nodiscard]] bool FindFromOpenNodeList(const Node& node) const noexcept;
        [[nodiscard]] bool FindFromCloseNodeList(const Node& node) const noexcept;

        void OptimizePath();

        std::vector<std::unique_ptr<Node>> mOpenNodeList;
        std::vector<std::unique_ptr<Node>> mCloseNodeList;
        
        Node mStartNode;
        Node mEndNode;
        Field mField;

        bool mIsAllowDiagonal = true;
        bool mIsAllowCrossCorner = true;
        
        std::vector<Node> mPath;
    };
}