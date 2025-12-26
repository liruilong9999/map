#include <osgViewer/viewer>
#include <osgEarth/Notify>
#include <osgEarth/EarthManipulator>
#include <osgEarth/MapNode>
#include <osgEarth/Threading>
#include <osgEarth/ShaderGenerator>
#include <osgEarth/Controls>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include <iostream>

#include <osgEarth/Common>
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/URI>

#include <osgEarth/MBTiles>
#include <osgEarth/Metrics>
#include <osgEarth/GDAL>
#include <osgEarth/TMS>
#include <osgEarth/XYZ>
#include <osgEarth/WMS>
#include <osgEarth/Composite>
#include <osgEarth/Profile>

#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include "LMapModule.h"
#include "MyElevationLayer.h"

#define LAYER_CONF_PATH "./data/layer.xml"

LMapModule::LMapModule(QObject * parent)
    : QObject(parent)
{
    m_rootNode                      = new osg::Group;
    osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map;
    m_mapNode                       = new osgEarth::MapNode(map.get());
    m_rootNode->addChild(m_mapNode.get());
    init();
}

LMapModule::~LMapModule()
{
    // 1. 从场景树中移除
    if (m_rootNode.valid() && m_mapNode.valid())
    {
        m_rootNode->removeChild(m_mapNode.get());
    }

    // 2. 主动释放引用
    m_mapNode  = nullptr;
    m_rootNode = nullptr;
}

void LMapModule::init()
{
    // 加载地图数据
    parserLayerConf(LAYER_CONF_PATH);

    for (LayerInfo & layerInfo : m_layerInfos)
    {
        switch (layerInfo.type)
        {
        case MapDataType::Type_Tiff :
            addGeoTiff(layerInfo);
            break;
        case MapDataType::Type_XYZ :
            addXYZ(layerInfo);
            break;
        default :
            continue;
        }
    }


}

void LMapModule::setRootNode(osg::ref_ptr<osg::Group> rootNode)
{
    m_rootNode = rootNode;
}

void LMapModule::setMapNode(osg::ref_ptr<osgEarth::MapNode> mapNode)
{
    m_mapNode = mapNode;
}

void LMapModule::setViewer(osg::ref_ptr<osgViewer::Viewer> viewer)
{
    m_viewer = viewer;
}

osg::ref_ptr<osg::Group> LMapModule::getRootNode() const
{
    return m_rootNode;
}

osg::ref_ptr<osgEarth::MapNode> LMapModule::getMapNode() const
{
    return m_mapNode;
}

osg::ref_ptr<osgViewer::Viewer> LMapModule::getViewer() const
{
    return m_viewer;
}

void LMapModule::addGeoTiff(LayerInfo & layerInfo)
{
    osg::ref_ptr<osgEarth::GDALImageLayer> gdalLayer = new osgEarth::GDALImageLayer();
    gdalLayer->setURL(osgEarth::URI(layerInfo.path));
    layerInfo.imageLayer = gdalLayer;

    if (layerInfo.imageLayer.valid())
    {
        // 添加到地图
        m_mapNode->getMap()->addLayer(layerInfo.imageLayer.get());
    }
}

void LMapModule::addMBTiles(LayerInfo & layerInfo)
{
    osg::ref_ptr<osgEarth::MBTilesImageLayer> mbtLayer = new osgEarth::MBTilesImageLayer();
    mbtLayer->setURL(osgEarth::URI(layerInfo.path));
    layerInfo.imageLayer = mbtLayer;
    if (layerInfo.imageLayer.valid())
    {
        // 添加到地图
        m_mapNode->getMap()->addLayer(layerInfo.imageLayer.get());
    }
}

void LMapModule::addTMS(LayerInfo & layerInfo)
{
    osg::ref_ptr<osgEarth::TMSImageLayer> someLayer = new osgEarth::TMSImageLayer();
    someLayer->setURL(osgEarth::URI(layerInfo.path));
    layerInfo.imageLayer = someLayer;
    if (layerInfo.imageLayer.valid())
    {
        // 添加到地图
        m_mapNode->getMap()->addLayer(layerInfo.imageLayer.get());
    }
}

void LMapModule::addWMS(LayerInfo & layerInfo)
{
    osg::ref_ptr<osgEarth::WMSImageLayer> someLayer = new osgEarth::WMSImageLayer;
    someLayer->setURL(osgEarth::URI(layerInfo.path));
    someLayer->setFormat("png");
    someLayer->setStyle("default");
    someLayer->setLayers("country_bounds");
    layerInfo.imageLayer = someLayer;
    if (layerInfo.imageLayer.valid())
    {
        // 添加到地图
        m_mapNode->getMap()->addLayer(layerInfo.imageLayer.get());
    }
}

void LMapModule::addXYZ(LayerInfo & layerInfo)
{
    osg::ref_ptr<osgEarth::XYZImageLayer> someLayer = new osgEarth::XYZImageLayer;
    someLayer->setURL(osgEarth::URI(layerInfo.path));
    someLayer->setProfile(osgEarth::Profile::create(osgEarth::Profile::SPHERICAL_MERCATOR));
    layerInfo.imageLayer = someLayer;
    if (layerInfo.imageLayer.valid())
    {
        // 添加到地图
        m_mapNode->getMap()->addLayer(layerInfo.imageLayer.get());
    }
}

void LMapModule::addHeight()
{
    // 暂不考虑高程数据
    // if (!m_mapNode || !m_viewer)
    //    return;
    // osgEarth::Map *              map         = m_mapNode->getMap();
    // osgEarth::MyElevationLayer * myElevation = new osgEarth::MyElevationLayer;
    // myElevation->setURL(osgEarth::URI("./data/env/dem_cog_0.0833f.tif"));
    // myElevation->setDataVariance(osg::Object::DataVariance::DYNAMIC); // this is not working for update height.
    // map->addLayer(myElevation);
    // g_elevationLayer                        = myElevation;
    // osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);
    // mapNode->addChild(makeGUI());
    // g_mapNode = mapNode;
    // m_viewer->setSceneData(mapNode);
}

void LMapModule::parserLayerConf(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Open xml failed:" << path;
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qWarning() << "Parse xml failed";
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "mapLayers")
    {
        qWarning() << "Invalid root node";
        return;
    }

    QDomNodeList layerNodes = root.elementsByTagName("mapLayer");
    for (int i = 0; i < layerNodes.count(); ++i)
    {
        QDomElement elem = layerNodes.at(i).toElement();
        if (elem.isNull())
            continue;

        LayerInfo cfg;
        cfg.name   = elem.attribute("name").toStdString();
        cfg.zValue = elem.attribute("zValue").toInt();
        cfg.path   = elem.attribute("path").toStdString();
        cfg.type   = cfg.parseMapLayerType(elem.attribute("type"));

        m_layerInfos.append(cfg);
    }
}
