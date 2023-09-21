
#include <iostream>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>

/*
类型名, 描述, 文件后缀
"collada",  "COLLADA - Digital Asset Exchange Schema", "dae"
"x",        "X Files", "x"
"stp",      "Step Files", "stp"
"obj",      "Wavefront OBJ format", "obj"
"objnomtl", "Wavefront OBJ format without material file", "obj"
"stl",      "Stereolithography", "stl"
"stlb",     "Stereolithography (binary)", "stl"
"ply",      "Stanford Polygon Library", "ply"
"plyb",     "Stanford Polygon Library (binary)", "ply"
"3ds",      "Autodesk 3DS (legacy)", "3ds"
"gltf2",    "GL Transmission Format v. 2", "gltf"
"glb2",     "GL Transmission Format v. 2 (binary)", "glb"
"gltf",     "GL Transmission Format", "gltf"
"glb",      "GL Transmission Format (binary)", "glb"
"assbin",   "Assimp Binary File", "assbin"
"assxml",   "Assimp XML Document", "assxml"
"x3d",      "Extensible 3D", "x3d"
"fbx",      "Autodesk FBX (binary)", "fbx"
"fbxa",     "Autodesk FBX (ascii)", "fbx"
"m3d",      "Model 3D (binary)", "m3d"
"m3da",     "Model 3D (ascii)", "a3d"
"3mf",      "The 3MF-File-Format", "3mf"
"pbrt",     "pbrt-v4 scene description file", "pbrt"
"assjson",  "Assimp JSON Document", "json"
*/

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "Usage:" << argv[0] << " 输入文件路径 输出类型名 输出文件路径\n";
        std::cout << "输出类型名:"
            "collada"
            ",x"
            ",stp"
            ",obj"
            ",objnomtl"
            ",stl"
            ",stlb"
            ",ply"
            ",plyb"
            ",3ds"
            ",gltf2"
            ",glb2"
            ",gltf"
            ",glb"
            ",assbin"
            ",assxml"
            ",x3d"
            ",fbx"
            ",fbxa"
            ",m3d"
            ",m3da"
            ",3mf"
            ",pbrt"
            ",assjson"
            << std::endl;
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
    Assimp::Exporter mAiExporter;
    aiReturn res = mAiExporter.Export(scene, argv[2], argv[3]);
    if (res != aiReturn::aiReturn_SUCCESS)
    {
        std::cout << "Export error" << std::endl;
        return 2;
    }

    std::cout << "OK" << std::endl;
    return 0;
}