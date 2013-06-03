/*
1. Import a dae file and export it to a bvh file
2. Compare the exported bvh with the original bvh
*/

#include <Importer.hpp>
#include <Exporter.hpp>
#include <scene.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  if (argc < 3)
    {
      printf("Usage: %s [InputFile] [OutputFile format]\n", argv[0]);
      return -1;
    }
  
  Assimp::Importer Ip;
  const aiScene* pScene = Ip.ReadFile(argv[1], 0u);

  Assimp::Exporter Ep;
  bool flag;

  string outfile("./out.");
  outfile = outfile + argv[2];
  cout << "Write to file: " << outfile << endl;
  flag = Ep.Export(pScene, argv[2], outfile.c_str()); 
  if (!flag)
    {
      printf("Error exporting the file\n");
      return -1;
    }

  return 0;
}
