

#include "MyElevationLayer.h"
namespace osgEarth {
void MyElevationLayer::init()
{
    GDALElevationLayer::init();
    // 必须设置有效值上下限，否则默认为(-32767, 32767)之间，
    // 超出该范围的使用填充值（无效值）,setNoDataValue可以设置无效值，默认为0.
    this->setMaxValidValue(10000000);
    this->setMinValidValue(-10000000);
}

//! Establishes a connection to the repository
Status MyElevationLayer::openImplementation()
{
    return GDALElevationLayer::openImplementation();
}

//! Closes down any GDAL connections
Status MyElevationLayer::closeImplementation()
{
    return GDALElevationLayer::closeImplementation();
}

//! Gets a heightfield for the given tile key
GeoHeightField MyElevationLayer::createHeightFieldImplementation(const TileKey & key, ProgressCallback * progress) const
{
    // 调用父类方法获取原始数据
    GeoHeightField          geoheight = GDALElevationLayer::createHeightFieldImplementation(key, progress);
    const osg::FloatArray * arr       = geoheight.getHeightField()->getFloatArray();
    float *                 dataptr   = (float *)arr->getDataPointer();
    int                     datasize  = arr->getNumElements();
    // datasize=66049 = 257*257 每个高程瓦片数据为257x257的栅格数据
    for (int i = 0; i < datasize; ++i)
    {
        float h = *dataptr;
        // 对原始高程数据进行放大操作，这里对于负值在-100m截断，否则海岸线不忍直视。
        *dataptr++ = fmaxf(-100, h * _scale);
    }
    return geoheight;
}

void MyElevationLayer::setScale(float scale)
{
    _scale = scale;
    //dirty();            // 并不会更新已绘制瓦片
    dirtyDataExtents(); // 并不会更新已绘制瓦片
}
} // namespace osgEarth