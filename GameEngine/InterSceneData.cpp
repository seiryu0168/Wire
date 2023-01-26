#include "InterSceneData.h"
#include"Engine/SAFE_DELETE_RELEASE.h"
#include<vector>
namespace InterSceneData
{

    struct Data
    {
        std::string dataName;
        std::string* s;
        int* i;
        float* f;
    };

    std::vector<Data*> dataList_;


    int AddData(std::string dataName,std::string* s,int* i, float* f)
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

}
