#include "InterSceneData.h"
#include"Engine/SAFE_DELETE_RELEASE.h"
#include<vector>
namespace InterSceneData
{

   /* struct Data
    {
        std::string name_;
        int dataSize_;
        GameObject* chunkClass_;
    };*/

    std::vector<Data*> dataList_;


    /*template<class T>
    int InteAddData(T* obj, std::string name,size_t dataSize)
    {

        Data* pData = new Data;
        for (int i = 0; i < dataList_.size(); i++)
        {
            if (name == dataList_[i]->name_)
            {
                SAFE_DELETE(pData);
                return -1;
            }
        }
        memcpy(&pData->chunkClass_, obj, dataSize);
        dataList_.push_back(pData);

            return 1;
    }*/
}
