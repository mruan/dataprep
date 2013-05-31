
#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <stdio.h>

// Assimp libraries
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#define VERBOSE 1
#define DebugMsg(p, x) if(p >= VERBOSE) printf(x)

using namespace std;

typedef unsigned int uint32;

class AnimUtils
{
public:
static map<string, uint32> mBoneIdx;

private:
  const aiScene* m_pScene;
  Assimp::Importer mIp;
  aiMatrix4x4 mGlobalInvTf;

  vector<aiMatrix4x4> mBoneOffset;
  vector<aiMatrix4x4> mBoneFinalTf;

  vector<const aiNodeAnim*> mBoneAnim;

public:
  //  uint32 GetTotalFrames();

  const vector<aiMatrix4x4>& GetTfs(){return mBoneFinalTf;}

  AnimUtils(const char* filename)
  {
    m_pScene = mIp.ReadFile(filename, aiProcess_GenSmoothNormals);
    
    if(m_pScene)
      {
	mGlobalInvTf = m_pScene->mRootNode->mTransformation;
	mGlobalInvTf.Inverse();
	
	// only looks at the first mesh and its bones
	if(m_pScene->mMeshes[0]->HasBones()) 
	  {
	    LoadBones(m_pScene->mMeshes[0]);
	  }

	if(m_pScene->HasAnimations())
	  { // Only Load the first Animation
	    BuildChannelDict(m_pScene->mAnimations[0]);
	  }
      }
  }
  
  //  ~AnimUtils(){delete m_pScene;}

  void LoadBones(const aiMesh* pMesh)
  {
    //    DebugMsg(2, "Loading bones from mesh");
    //    printf("Loading bones from mesh\n");

    mBoneOffset.resize(mBoneIdx.size());
    mBoneFinalTf.resize(mBoneIdx.size());
    for(uint32 i=0; i< pMesh->mNumBones; i++)
      {
	string BoneNm(pMesh->mBones[i]->mName.data);

	if(mBoneIdx.find(BoneNm) != mBoneIdx.end())
	  {
	    uint32 boneIdx = mBoneIdx[BoneNm];
	    mBoneOffset[boneIdx] = pMesh->mBones[i]->mOffsetMatrix;

	    //	    printf("Idx: %2u, Name: %s\n", boneIdx, BoneNm.c_str());
	  }
	else
	  {
	    //	    printf("Not needed: %s\n", BoneNm.c_str());
	  }
      }
    // Don't worry about the weights
  }
  
  void BuildChannelDict(const aiAnimation* pAnim)
  {

    //    DebugMsg(2, "Counting bones in animations");
    //    printf("Counting bones in animations\n");

    mBoneAnim.resize(mBoneIdx.size());
    for(uint32 i=0; i< pAnim->mNumChannels; ++i)
      {
	const aiNodeAnim* pNodeAnim = pAnim->mChannels[i];
	string name(pNodeAnim->mNodeName.data);
	uint32 boneIdx = mBoneIdx[name];
	mBoneAnim[boneIdx] = pNodeAnim;

	//	printf("Idx: %2u, Name: %s\n", boneIdx, name.c_str());
      }
  }
  
  void DumpAllFrames()
  {
    aiNode* pNode = m_pScene->mRootNode;
    std::string nodeName (pNode->mName.data);
    while (mBoneIdx.find(nodeName) == mBoneIdx.end())
      {
	pNode = pNode->mChildren[0];
	nodeName = std::string(pNode->mName.data);
      }
    
    const aiAnimation* pAnim = m_pScene->mAnimations[0];
    aiMatrix4x4 I(1.0f, 0.0f, 0.0f, 0.0f,
		  0.0f, 1.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f,
		  0.0f, 0.0f, 0.0f, 1.0f);

    // for all frames:
    for(uint32 j=0; j< pAnim->mChannels[0]->mNumRotationKeys; j++)
      {
	//	printf("\n[[Frame %2d]]\n", j);
	ReadNodeHeirarchy(j, pNode, I);
      }
  }

  void ReadNodeHeirarchy(uint32 frameIdx, 
			 const aiNode* pNode, 
			 const aiMatrix4x4& parTf)
  {
    string nodeName(pNode->mName.data);
    if (mBoneIdx.find(nodeName) == mBoneIdx.end())
      return;

    uint32 boneIdx = mBoneIdx[nodeName];

    const aiNodeAnim* pAnim = mBoneAnim[boneIdx];
    aiVectorKey& sc= pAnim->mScalingKeys[frameIdx % pAnim->mNumScalingKeys];
    aiMatrix4x4 matScale;
    aiMatrix4x4::Scaling(sc.mValue, matScale);
    
    aiQuatKey& qt = pAnim->mRotationKeys[frameIdx % pAnim->mNumRotationKeys];
    aiMatrix4x4 matRotat(qt.mValue.GetMatrix());
    
    aiVectorKey& tr = pAnim->mPositionKeys[frameIdx % pAnim->mNumPositionKeys];
    aiMatrix4x4 matTrans;
    aiMatrix4x4::Translation(tr.mValue, matTrans);

    aiMatrix4x4 nodeTf  = matTrans * matRotat * matScale;
    
    aiMatrix4x4 globalTf= parTf * nodeTf;
    
    aiMatrix4x4 finalTf = mGlobalInvTf * globalTf * mBoneOffset[boneIdx];
    mBoneFinalTf[boneIdx] = finalTf;

    // Print out info
    //    cout << nodeName << endl;
    //    pprintMat4x4(finalTf);
    pprintMat16(finalTf);

    for(uint i=0; i< pNode->mNumChildren; ++i)
    ReadNodeHeirarchy(frameIdx, pNode->mChildren[i], globalTf);
  }

  /* Some helper functions */
  static void pprintMat4x4(const aiMatrix4x4& mat)
  {
    printf("%8.5f %8.5f %8.5f %8.5f\n", mat.a1, mat.a2, mat.a3, mat.a4);
    printf("%8.5f %8.5f %8.5f %8.5f\n", mat.b1, mat.b2, mat.b3, mat.b4);
    printf("%8.5f %8.5f %8.5f %8.5f\n", mat.c1, mat.c2, mat.c3, mat.c4);
    printf("%8.5f %8.5f %8.5f %8.5f\n", mat.d1, mat.d2, mat.d3, mat.d4);
  }

  // This is easier for matlab to process
  static void pprintMat16(const aiMatrix4x4& mat)
  {
    printf("%.5f %.5f %.5f %.5f " , mat.a1, mat.a2, mat.a3, mat.a4);
    printf("%.5f %.5f %.5f %.5f " , mat.b1, mat.b2, mat.b3, mat.b4);
    printf("%.5f %.5f %.5f %.5f " , mat.c1, mat.c2, mat.c3, mat.c4);
    printf("%.5f %.5f %.5f %.5f\n", mat.d1, mat.d2, mat.d3, mat.d4);
  }
};
