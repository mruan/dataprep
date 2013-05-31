#include "utils.hpp"

#include <stdio.h>

map<string, uint32> AnimUtils::mBoneIdx;
void help(char* program_name)
{
  printf("usage: %s file_path\n", program_name);
}

void buildBoneIdx()
{
  AnimUtils::mBoneIdx["Hips"]      = 0;
  AnimUtils::mBoneIdx["LHipJoint"] = 1;
  AnimUtils::mBoneIdx["LeftUpLeg"] = 2;
  AnimUtils::mBoneIdx["LeftLeg"]   = 3;
  AnimUtils::mBoneIdx["LeftFoot"]  = 4;
  AnimUtils::mBoneIdx["RHipJoint"] = 5;
  AnimUtils::mBoneIdx["RightUpLeg"]= 6;
  AnimUtils::mBoneIdx["RightLeg"]  = 7;
  AnimUtils::mBoneIdx["RightFoot"] = 8;
  AnimUtils::mBoneIdx["LowerBack"] = 9;
  AnimUtils::mBoneIdx["Spine"]     =10;
  AnimUtils::mBoneIdx["Spine1"]    =11;
  AnimUtils::mBoneIdx["Neck"]      =12;
  AnimUtils::mBoneIdx["Neck1"]     =13;
  AnimUtils::mBoneIdx["Head"]      =14;
  AnimUtils::mBoneIdx["LeftShoulder"]=15;
  AnimUtils::mBoneIdx["LeftArm"]   =16;
  AnimUtils::mBoneIdx["LeftForeArm"]=17;
  AnimUtils::mBoneIdx["LeftHand"]  =18;
  AnimUtils::mBoneIdx["RightShoulder"]=19;
  AnimUtils::mBoneIdx["RightArm"]  =20;
  AnimUtils::mBoneIdx["RightForeArm"]=21;
  AnimUtils::mBoneIdx["RightHand"] =22;
}


int main(int argc, char** argv)
{
  if (argc< 2)
    {
      help(argv[0]);
    }
  
  buildBoneIdx();
  AnimUtils util(argv[1]);
  //  AnimUtils daeUtil(argv[2]);

  util.DumpAllFrames();
  return 0;
}
