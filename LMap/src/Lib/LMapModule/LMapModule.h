#ifndef LMAPMODULE_H
#define LMAPMODULE_H

#include <QObject>

#include "MapDefine.h"
#include "LMapModule_Global.h"

#include <osg/Group>
#include <osgViewer/Viewer>

class LMAPMODULE_EXPORT LMapModule : public QObject
{
public:
    LMapModule(QObject * parent = nullptr);
    ~LMapModule() override;

    void init();


    void setRootNode(osg::ref_ptr<osg::Group> rootNode);
    void setMapNode(osg::ref_ptr<osgEarth::MapNode> mapNode);
    void setViewer(osg::ref_ptr<osgViewer::Viewer> viewer);

    osg::ref_ptr<osg::Group>        getRootNode() const;
    osg::ref_ptr<osgEarth::MapNode> getMapNode() const;
    osg::ref_ptr<osgViewer::Viewer> getViewer() const;

     // 添加tiff图层
    void addGeoTiff(LayerInfo & layerInfo);
    // 添加MBTiles图层
    void addMBTiles(LayerInfo & layerInfo);
    // 添加TMS图层
    void addTMS(LayerInfo & layerInfo);
    // 添加WMS图层
    void addWMS(LayerInfo & layerInfo);
    // 添加XYZ图层，即{z}/{x}/{y}.png
    void addXYZ(LayerInfo & layerInfo); 
    // 添加高程数据
    void addHeight(); 
    private:
    void parserLayerConf(QString path);


private:
    osg::ref_ptr<osg::Group>        m_rootNode;
    osg::ref_ptr<osgEarth::MapNode> m_mapNode;
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    QList<LayerInfo>                m_layerInfos;
};

#endif // LMapModule_H
