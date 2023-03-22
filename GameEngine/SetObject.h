#pragma once
#include"Engine/GameObject/GameObject.h" 
#include<vector>
class SetObject
{
private:
    struct objectData
    {
        std::string objectName;
        Transform objectTransform;
    
    };
    std::vector<objectData> objectDataList_;
    GameObject* pScene_;
public:
    SetObject(GameObject* scene);

    //デストラクタ
    ~SetObject();

    void Load(std::string filePath);
    std::vector<std::string> SplitLine(const std::string& line, const char& delimiter);
    std::vector<std::string> SplitLine(const std::string& line, std::vector<char> delimiters);
    void ObjectSet();
};

