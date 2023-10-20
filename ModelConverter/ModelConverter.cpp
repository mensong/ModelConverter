
#include <iostream>
#include <fstream>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/importerdesc.h>
#include <vector>
#include <map>
#include <string>

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


//////////////////////////////////////////////////////////////////////////////////////////////
///
///
#define LEFTSTRIP 0
#define RIGHTSTRIP 1
#define BOTHSTRIP 2

inline int _isspace(int c)
{
    return (c > 0) && (::isspace(c));
}

std::string trim(const std::string& str, int striptype, const std::string& chars)
{
    size_t len = (size_t)str.size(), i, j, charslen = (size_t)chars.size();
    if (len == 0)
        return str;

    if (charslen == 0)
    {
        i = 0;
        if (striptype != RIGHTSTRIP)
        {
            while (i < len && _isspace(str[i]))
            {
                i++;
            }
        }

        j = len;
        if (striptype != LEFTSTRIP)
        {
            do
            {
                j--;
            } while (j >= i && _isspace(str[j]));

            j++;
        }


    }
    else
    {
        const char* sep = chars.c_str();

        i = 0;
        if (striptype != RIGHTSTRIP)
        {
            while (i < len && memchr(sep, str[i], charslen))
            {
                i++;
            }
        }

        j = len;
        if (striptype != LEFTSTRIP)
        {
            do
            {
                j--;
            } while (j >= i && memchr(sep, str[j], charslen));
            j++;
        }


    }

    if (i == 0 && j == len)
    {
        return str;
    }
    else
    {
        return str.substr(i, j - i);
    }
}

void split_whitespace(const std::string& str, std::vector< std::string >& result, int maxsplit)
{
    std::string::size_type i, j, len = str.size();
    for (i = j = 0; i < len; )
    {

        while (i < len && _isspace(str[i])) i++;
        j = i;

        while (i < len && !_isspace(str[i])) i++;



        if (j < i)
        {
            if (maxsplit-- <= 0) break;

            result.push_back(str.substr(j, i - j));

            while (i < len && _isspace(str[i])) i++;
            j = i;
        }
    }
    if (j < len)
    {
        result.push_back(str.substr(j, len - j));
    }
}

void split(const std::string& str, std::vector<std::string>& result, const std::string& sep = "", int maxsplit = -1)
{
    result.clear();

    if (maxsplit <= 0) 
        maxsplit = INT_MAX;//result.max_size();


    if (sep.size() == 0)
    {
        split_whitespace(str, result, maxsplit);
        return;
    }

    std::string::size_type i, j, len = str.size(), n = sep.size();

    i = j = 0;

    while (i + n <= len)
    {
        if (str[i] == sep[0] && str.substr(i, n) == sep)
        {
            if (maxsplit-- <= 0) break;

            result.push_back(str.substr(j, i - j));
            i = j = i + n;
        }
        else
        {
            i++;
        }
    }

    result.push_back(str.substr(j, len - j));
}

void toLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int main(int argc, char** argv)
{
    int errCode = 0;

    std::map<std::string, std::string> exportExtToType;

    Assimp::Exporter mAiExporter;

    //读取默认配置
    size_t formatCount = mAiExporter.GetExportFormatCount();
    for (size_t i = 0; i < formatCount; i++)
    {
        const aiExportFormatDesc* fd = mAiExporter.GetExportFormatDescription(i);
        std::string sExt = std::string(".") + fd->fileExtension;
        toLower(sExt);
        exportExtToType[sExt.c_str()] = fd->id;
    }

    //读取配置文件
    std::ifstream configFile("ModelConverter.config");
    if (configFile.is_open()) 
    { // 检查文件是否成功打开
        std::string line;
        while (std::getline(configFile, line)) 
        { // 逐行读取文件内容
            line = trim(line, BOTHSTRIP, "");
            if (line.size() == 0 || line[0] == '#')
                continue;
            size_t idx = line.find('#');
            if (idx != std::string::npos)
                line = line.substr(0, idx);
            idx = line.find('=');
            if (idx == std::string::npos)
                continue;

            std::string k = trim(line.substr(0, idx), BOTHSTRIP, "");
            toLower(k);
            std::string v = trim(line.substr(idx + 1), BOTHSTRIP, "");
            toLower(v);
            if (k.size() == 0 || v.size() == 0)
                continue;

            exportExtToType[k.c_str()] = v.c_str();
        }
        configFile.close(); // 关闭文件
    }
        
    Assimp::Importer importer;
    
    if (argc != 3)
    {
        std::cout << "Usage:" << argv[0] << " 输入文件路径 输出文件路径\n";
                
        std::cout << "可输入文件类型" << std::endl;
        std::cout << "文件后缀" << "\t\t类型名" << "\t\t描述" << std::endl;
        size_t supportCount = importer.GetImporterCount();
        for (size_t i = 0; i < supportCount; i++)
        {
            const aiImporterDesc* desc = importer.GetImporterInfo(i);
            std::string sName = desc->mName;
            std::string sExts = desc->mFileExtensions;
            std::string sDesc = desc->mComments;
            std::cout << sExts << "|\t\t" << sName << "|\t\t" << sDesc << std::endl;
        }

        std::cout << std::endl << std::endl;

        std::cout << "可导出类型"<< "\t\t文件后缀" << "\t\t描述" << std::endl;
        size_t formatCount = mAiExporter.GetExportFormatCount();
		for (size_t i = 0; i < formatCount; i++)
		{
			const aiExportFormatDesc* fd = mAiExporter.GetExportFormatDescription(i);
			std::string sId = fd->id;
			std::string sExt = fd->fileExtension;
			std::string sDesc = fd->description;
			std::cout << sId << "|\t\t." << sExt << "|\t\t" << sDesc << std::endl;
		}

        return ++errCode;
    }

    const aiScene* scene = importer.ReadFile(argv[1], 
        aiProcessPreset_TargetRealtime_Fast);
    if (!scene) 
    {
        std::cout << "Import error" << std::endl;
        return ++errCode;
    }

    std::string exportFilepath = argv[2];

    //获得导出类型
    size_t idx = exportFilepath.find_last_of('.');
    if (idx == std::string::npos)
    {
        std::cout << "Error export file path" << std::endl;
        return ++errCode;
    }
    std::string ext = exportFilepath.substr(idx);
    toLower(ext);
    auto itFinder = exportExtToType.find(ext.c_str());
    if (itFinder == exportExtToType.end())
    {
        std::cout << "Unsupport export type" << std::endl;
        return ++errCode;
    }
    std::string formatId = itFinder->second;
    
    //执行导出
    aiReturn res = mAiExporter.Export(scene, formatId, exportFilepath);
    if (res != aiReturn::aiReturn_SUCCESS)
    {
        std::cout << "Export error" << std::endl;
        return ++errCode;
    }

    std::cout << "OK" << std::endl;
    return errCode;
}