#ifndef MAPDEFINE_H
#define MAPDEFINE_H

#include <string>
#include <osg/ref_ptr>
#include <osgearth/imagelayer>

enum class MapDataType
{
    Type_Tiff,
    Type_XYZ,
    Unknown
};

struct LayerInfo
{
    bool                               isShowState{true};   // 图层显示状态
    std::string                        name;                // 图层名称
    unsigned int                       zValue{0};      // 图层当前的图层层数
    std::string                        path;            // 地图数据Url
    MapDataType                        type;           // 地图类型
    osg::ref_ptr<osgEarth::ImageLayer> imageLayer{nullptr}; // 当前图层

    static MapDataType parseMapLayerType(const QString & typeStr)
    {
        if (typeStr == "Type_Tiff")
            return MapDataType::Type_Tiff;
        if (typeStr == "Type_XYZ")
            return MapDataType::Type_XYZ;

        return MapDataType::Unknown;
    }
};

#endif