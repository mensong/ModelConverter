
#include <iostream>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>

/*
类型名           文件后缀                  描述                                           
"collada"        ".dae"                    "COLLADA - Digital Asset Exchange Schema"      
"x"              ".x"                      "X Files"                                      
"stp"            ".stp"                    "Step Files"                                   
"obj"            ".obj"                    "Wavefront OBJ format"                         
"objnomtl"       ".obj"                    "Wavefront OBJ format without material file"   
"stl"            ".stl"                    "Stereolithography"                            
"stlb"           ".stl"                    "Stereolithography (binary)"                   
"ply"            ".ply"                    "Stanford Polygon Library"                     
"plyb"           ".ply"                    "Stanford Polygon Library (binary)"            
"3ds"            ".3ds"                    "Autodesk 3DS (legacy)"                        
"gltf2"          ".gltf"                   "GL Transmission Format v. 2"                  
"glb2"           ".glb"                    "GL Transmission Format v. 2 (binary)"         
"gltf"           ".gltf"                   "GL Transmission Format"                       
"glb"            ".glb"                    "GL Transmission Format (binary)"              
"assbin"         ".assbin"                 "Assimp Binary File"                           
"assxml"         ".assxml"                 "Assimp XML Document"                          
"x3d"            ".x3d"                    "Extensible 3D"                                
"fbx"            ".fbx"                    "Autodesk FBX (binary)"                        
"fbxa"           ".fbx"                    "Autodesk FBX (ascii)"                         
"m3d"            ".m3d"                    "Model 3D (binary)"                            
"m3da"           ".a3d"                    "Model 3D (ascii)"                             
"3mf"            ".3mf"                    "The 3MF-File-Format"                          
"pbrt"           ".pbrt"                   "pbrt-v4 scene description file"               
"assjson"        ".json"                   "Assimp JSON Document"                         
*/

int main(int argc, char** argv)
{
    Assimp::Exporter mAiExporter;
    
    if (argc != 4)
    {
        std::cout << "Usage:" << argv[0] << " 输入文件路径 输出类型名 输出文件路径\n";

        std::cout << "类型名"<< "\t\t文件后缀" << "\t\t描述" << std::endl;
        size_t formatCount = mAiExporter.GetExportFormatCount();
        for (size_t i = 0; i < formatCount; i++)
        {
            const aiExportFormatDesc* fd = mAiExporter.GetExportFormatDescription(i);
            std::string sId = fd->id;
            std::string sExt = fd->fileExtension;
            std::string sDesc = fd->description;
            std::cout << sId << "\t\t." << sExt << "\t\t" << sDesc << std::endl;
        }

        return 1;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(argv[1], 
        aiProcessPreset_TargetRealtime_Fast);
    if (!scene) 
    {
        std::cout << "Import error" << std::endl;
        return 1;
    }
    
    aiReturn res = mAiExporter.Export(scene, argv[2], argv[3]);
    if (res != aiReturn::aiReturn_SUCCESS)
    {
        std::cout << "Export error" << std::endl;
        return 2;
    }

    std::cout << "OK" << std::endl;
    return 0;
}