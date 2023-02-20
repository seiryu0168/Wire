#include "InterSceneData.h"
#include"Engine/SAFE_DELETE_RELEASE.h"
#include<vector>
namespace InterSceneData
{

    struct Data
    {
        std::string dataName = "";
        std::string* s = nullptr;
        int* i = nullptr;
        float* f = nullptr;
        bool* b = nullptr;
    };

    std::vector<Data*> dataList_;


    int AddData(std::string dataName,std::string* s,int* i, float* f,bool* b)
    {
        bool existData = false;
        Data* pData = new Data;
        pData->i = new int;
        pData->f = new float;
        if (s != nullptr)
        {
            pData->s = new std::string;
            *pData->s = *s;
        }

        if (i != nullptr)
        {
            pData->i = new int;
            *pData->i = *i;
           
            existData = true;
        }
        if (f != nullptr)
        {
            pData->f = new float;
            *pData->f = *f;
            existData = true;
        }

        if (b != nullptr)
        {
            pData->b = new bool;
            *pData->b = *b;
            existData = true;
        }

        if (existData)
        {
            pData->dataName = dataName;
            dataList_.push_back(pData);
            return 1;
        }
        else
            delete pData;

        return 0;
    }

    int GetintData(std::string dataName)
    {
        for (int i = 0; i < dataList_.size(); i++)
        {
            if (dataList_[i]->dataName == dataName&&dataList_[i]->i!=nullptr)
            {
                return *(dataList_[i]->i);
            }

        }
        return -2147483648;
    }

    bool GetboolData(std::string dataName)
    {
        for (int i = 0; i < dataList_.size(); i++)
        {
            if (dataList_[i]->dataName == dataName && dataList_[i]->b!= nullptr)
            {
                return *(dataList_[i]->b);
            }

        }
        return 3;
    }

}
