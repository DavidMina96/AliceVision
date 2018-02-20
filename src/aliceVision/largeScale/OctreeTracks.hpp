// This file is part of the AliceVision project.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/structures/Pixel.hpp>
#include <aliceVision/structures/Point3d.hpp>
#include <aliceVision/structures/StaticVector.hpp>
#include <aliceVision/structures/Voxel.hpp>
#include <aliceVision/common/PreMatchCams.hpp>
#include <aliceVision/largeScale/Fuser.hpp>

namespace aliceVision {
namespace largeScale {

class OctreeTracks : public Fuser
{
public:
    enum NodeType
    {
        BranchNode,
        LeafNode
    };

    class Node
    {
    public:
        NodeType type_ : 2;

        Node(NodeType type);
        ~Node(){};
    };

    class Branch : public Node
    {
    public:
        Node* children[2][2][2];

        Branch();
        ~Branch();
    };

    class trackStruct : public Node
    {
    public:
        int npts;
        Point3d point;
        StaticVector<Pixel> cams;
        float minPixSize;
        float minSim;

        trackStruct(trackStruct* t);
        trackStruct(float sim, float pixSize, const Point3d& p, int rc);
        ~trackStruct();
        void resizeCamsAdd(int nadd);
        void addPoint(float sim, float pixSize, const Point3d& p, int rc);
        void addTrack(trackStruct* t);
        void addDistinctNonzeroCamsFromTrackAsZeroCams(trackStruct* t);
        int indexOf(int val);
        void doPrintf();
    };

    Node* root_;
    int size_;
    int leafsNumber_;

    // trackStruct* at( int x, int y, int z );
    trackStruct* getTrack(int x, int y, int z);
    void addTrack(int x, int y, int z, trackStruct* t);
    void addPoint(int x, int y, int z, float sim, float pixSize, Point3d& p, int rc);
    StaticVector<trackStruct*>* getAllPoints();
    void getAllPointsRecursive(StaticVector<trackStruct*>* out, Node* node);

    Point3d O, vx, vy, vz;
    float sx, sy, sz, svx, svy, svz;
    float avPixSize;

    Point3d vox[8];
    int numSubVoxsX;
    int numSubVoxsY;
    int numSubVoxsZ;
    bool doUseWeaklySupportedPoints;
    bool doUseWeaklySupportedPointCam;
    bool doFilterOctreeTracks;
    int minNumOfConsistentCams;
    float simWspThr;

    OctreeTracks(const Point3d* _voxel, MultiViewParams* _mp, PreMatchCams* _pc, Voxel dimensions);
    ~OctreeTracks();

    float computeAveragePixelSizeForVoxel();
    bool getVoxelOfOctreeFor3DPoint(Voxel& out, Point3d& tp);

    void filterMinNumConsistentCams(StaticVector<trackStruct*>* tracks);

    void filterOctreeTracks2(StaticVector<trackStruct*>* tracks);
    void updateOctreeTracksCams(StaticVector<trackStruct*>* tracks);
    StaticVector<trackStruct*>* fillOctreeFromTracks(StaticVector<trackStruct*>* tracksIn);
    StaticVector<trackStruct*>* fillOctree(int maxPts, std::string depthMapsPtsSimsTmpDir);
    StaticVector<int>* getTracksCams(StaticVector<OctreeTracks::trackStruct*>* tracks);
    void getNPointsByLevelsRecursive(Node* node, int level, StaticVector<int>* nptsAtLevel);
};

} // namespace largeScale
} // namespace aliceVision
