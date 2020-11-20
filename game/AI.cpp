#include "AI.h"

#include <algorithm>
#include "Engine.h"

namespace PathFinding
{

    Node::Node(Vector2DInt startPos, Vector2DInt destinationPos, int depth) : mLocation(startPos), mDepth(depth)
    {
        int dx = destinationPos.x - startPos.x;
        int dy = destinationPos.y - startPos.y;
        mDistance = (dx * dx) + (dy * dy);
    }

    Node::Node(Vector2DInt startPos, int depth) : mLocation(startPos), mDepth(depth), mDistance(-1) {}

    bool Node::IsSameLocation(const Node &other) const noexcept
    {
        return ((mLocation.x == other.mLocation.x) && (mLocation.y == other.mLocation.y));
    }

    void Node::CalculateDistance(const Node &destination) noexcept
    {

        int dx = destination.mLocation.x - mLocation.x;
        int dy = destination.mLocation.y - mLocation.y;

        mDistance = (dx * dx) + (dy * dy);
    }

    Vector2DInt Node::GetLocation() const noexcept
    {
        return mLocation;
    }

    int Node::GetDistance() const noexcept
    {
        return mDistance;
    }

    void Node::SetDepth(int depth) noexcept
    {
        mDepth = depth;
    }

    int Node::GetDepth() const noexcept
    {
        return mDepth;
    }

    void Node::SetParent(std::unique_ptr<Node>* parent) noexcept
    {
        mParent = parent;
    }

    std::unique_ptr<Node>* Node::GetParent() noexcept
    {
        return mParent;
    }

    Module::Module(const Node &startNode, const Node &endNode, const Field &field) : mStartNode(startNode), mEndNode(endNode), mField(field)
    {
        mCloseNodeList.reserve(mField.mWidth * mField.mHeight);
    }

    void Module::Set(const Node& startNode, const Node& endNode, const Field &field)
    {
        Reset();

        mStartNode = startNode;
        mEndNode = endNode;
        mField = field;

        mCloseNodeList.reserve(mField.mWidth * mField.mHeight);
    }

    void Module::Reset()
    {
        mOpenNodeList.clear();
        mCloseNodeList.clear();

        mPath.clear();
    }

    bool Module::Run()
    {
        if(!mPath.empty())
        {
            Engine::GetLogger().LogWarning("Warning in PathFinding::Module::Run() : CurrentPath isn't empty, did you set the module?");
        }

        int depth = 0;
        mStartNode.SetDepth(depth);

        std::unique_ptr<Node> node = std::make_unique<Node>(mStartNode);
        InsetOpenNodeList(node);

        std::vector<std::unique_ptr<Node>> childNodes;

        while(true)
        {
            if(mOpenNodeList.empty())
            {
                return false;
            }

            node = std::move(mOpenNodeList.back());
            mOpenNodeList.pop_back();

            if(mEndNode.IsSameLocation(*node))
            {
                while(node->GetParent() != nullptr)
                {
                    mPath.push_back(*node);
                    node = std::move(*node->GetParent());
                }

                OptimizePath();
                return true;
            }
            depth++;
            childNodes.clear();

            for(int y = -1; y <= 1; ++y)
            {
                for(int x = -1; x <= 1; ++x)
                {
                    if(x == 0 && y == 0)
                        continue;

                    if(node->GetLocation().x + x >= mField.mWidth || node->GetLocation().x + x < 0 ||
                       node->GetLocation().y + y >= mField.mHeight || node->GetLocation().y + y < 0)
                        continue;

                    /* 만약 대각선에 있는 타일이라면 */
                    if( (x + y + 2) % 2 == 0)
                    {
                        if(!mIsAllowDiagonal)
                            continue;

                        else if(!mIsAllowCrossCorner)
                        {
                            if(mField.mVector[node->GetLocation().x       + mField.mWidth * (node->GetLocation().y + y)] <= 0 &&
                               mField.mVector[(node->GetLocation().x + x) + mField.mWidth * node->GetLocation().y      ] <= 0)
                            continue;
                        }
                    }

                    Vector2DInt loc = node->GetLocation() + Vector2DInt{x, y};
                    std::unique_ptr<Node> temp = std::make_unique<Node>(loc, depth);
                    childNodes.push_back(std::move(temp));
                }
            }
            InsetCloseNodeList(node);

            for(std::unique_ptr<Node>& elem : childNodes)
            {
                if(mField.mVector[elem->GetLocation().x + mField.mWidth * elem->GetLocation().y] <= 0)
                    continue;

                if(FindFromCloseNodeList(*elem))
                    continue;

                elem->CalculateDistance(mEndNode);
                elem->SetParent(&mCloseNodeList.back());
                InsetOpenNodeList(elem);
            }
            std::sort(mOpenNodeList.begin(), mOpenNodeList.end(), CompareNode);
        }
    }

    std::vector<Node>& Module::GetPath()
    {
        return mPath;
    }

    bool Module::CompareNode(const std::unique_ptr<Node>& node1, const std::unique_ptr<Node>& node2) noexcept
    {
        if(node1->GetDistance() == node2->GetDistance())
        {
            return (node1->GetDepth() > node2->GetDepth());
        }
        else
        {
            return (node1->GetDistance() > node2->GetDistance());
        }
    }

    void Module::InsetOpenNodeList(std::unique_ptr<Node>& node) noexcept
    {
        for(const std::unique_ptr<Node>& elem : mOpenNodeList)
        {
            if(elem->IsSameLocation(*node))
            {
                InsetCloseNodeList(node);
                return;
            }
        }

        mOpenNodeList.push_back(std::move(node));
    }

    void Module::InsetCloseNodeList(std::unique_ptr<Node>& node) noexcept
    {
        mCloseNodeList.push_back(std::move(node));
    }

    bool Module::FindFromOpenNodeList(const Node& node) const noexcept
    {
        for(const std::unique_ptr<Node>& elem : mOpenNodeList)
        {
            if(elem->IsSameLocation(node))
            {
                return true;
            }
        }
        return false;
    }

    bool Module::FindFromCloseNodeList(const Node& node) const noexcept
    {
        for(const std::unique_ptr<Node>& elem : mCloseNodeList)
        {
            if(elem->IsSameLocation(node))
            {
                return true;
            }
        }
        return false;
    }

    void Module::OptimizePath()
    {
        /* 경로 검사를 위해 시작 노드를 임시로 삽입함 */
        mPath.insert(mPath.begin(), mStartNode);
        int curIndex = 0;
        int mergeIndex = -1;
        for(const Node& path : mPath)
        {
            mergeIndex = -1;

            for(int i = 2; i < 6; ++i)
            {
                if(mPath.size() <= curIndex + i)
                    continue;

                int nodeX = path.GetLocation().x;
                int nodeY = path.GetLocation().y;
                int checkNodeX = mPath[curIndex + i].GetLocation().x;
                int checkNodeY = mPath[curIndex + i].GetLocation().y;

                /* 인근한 노드이고 */
                if(std::abs(nodeX - checkNodeX) <= 1 &&
                   std::abs(nodeY - checkNodeY) <= 1)
                {
                    int dx = nodeX - checkNodeX;
                    int dy = nodeY - checkNodeY;
                    /* 대각선이라면 */
                    if( (dx + dy + 2) % 2 == 0)
                    {
                        if(mIsAllowDiagonal)
                        {
                            /* 코너를 건너는걸 허용한다면 */
                            if(mIsAllowCrossCorner)
                            {
                                mergeIndex = i;
                            }
                            else
                            {
                                if(mField.mVector[path.GetLocation().x       + mField.mWidth * (path.GetLocation().y + dy)] > 0 ||
                                   mField.mVector[(path.GetLocation().x + dx) + mField.mWidth * path.GetLocation().y      ] > 0)
                                {
                                    mergeIndex = i;
                                }
                            }
                        }
                    }
                    else
                    {
                        mergeIndex = i;
                    }
                }
            }

            for(int i = 1; i < mergeIndex; ++i)
            {
                mPath.erase(mPath.begin() + curIndex + 1);
            }

            curIndex++;
        }

        /* 임시로 삽입한 시작 노드를 제거함 */
        mPath.erase(mPath.begin());
    }
}