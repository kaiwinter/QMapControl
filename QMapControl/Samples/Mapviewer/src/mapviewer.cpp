#include "mapviewer.h"
/*!
 * \example mapviewer.cpp
 * This application is just a simple map viewer. A Mapadapter is created (OpenStreetmaps) 
 * and added to a layer. The layer is given to the MapControl.
 * Two Buttons are available to adjust the zoom level.
 * 
 * You can find this example here: MapAPI/Samples/Mapviewer
 * \image html sample_mapviewer.png "screenshot"
 */
Mapviewer::Mapviewer(QWidget *parent)
	: QMainWindow(parent)
{
	// create MapControl
	mc = new MapControl(QSize(380, 540));
    mc->showScale(true);
	// create mapadapter, for mainlayer and overlay
	mapadapter = new OSMMapAdapter();
	MapAdapter* mapadapter_overlay = new YahooMapAdapter("us.maps3.yimg.com", "/aerial.maps.yimg.com/png?v=2.2&t=h&s=256&x=%2&y=%3&z=%1");
	
	// create a layer with the mapadapter and type MapLayer
	mainlayer = new MapLayer("Custom Layer", mapadapter);
	
	// create yahoo satellite overlay
	overlay = new MapLayer("Overlay", mapadapter_overlay);
	overlay->setVisible(false);
	
	// add Layer to the MapControl
	mc->addLayer(mainlayer);
	mc->addLayer(overlay);

	addZoomButtons();
	createActions();
	createMenu();
	
	// show mapcontrol in mainwindow
	setCentralWidget(mc);
}
void Mapviewer::addZoomButtons()
{
	// create buttons as controls for zoom
	QPushButton* zoomin = new QPushButton("+");
	QPushButton* zoomout = new QPushButton("-");
	zoomin->setMaximumWidth(50);
	zoomout->setMaximumWidth(50);
	
	connect(zoomin, SIGNAL(clicked(bool)),
			  mc, SLOT(zoomIn()));
	connect(zoomout, SIGNAL(clicked(bool)),
			  mc, SLOT(zoomOut()));
	
	// add zoom buttons to the layout of the MapControl
	QVBoxLayout* innerlayout = new QVBoxLayout;
	innerlayout->addWidget(zoomin);
	innerlayout->addWidget(zoomout);
	mc->setLayout(innerlayout);
}

void Mapviewer::createActions()
{
	QActionGroup* mapproviderGroup = new QActionGroup(this);
	osmAction = new QAction(tr("OpenStreetMap"), mapproviderGroup);
	yahooActionMap = new QAction(tr("Yahoo: Map"), mapproviderGroup);
	yahooActionSatellite = new QAction(tr("Yahoo: Satellite"), mapproviderGroup);
	googleActionMap = new QAction(tr("Google: Map"), mapproviderGroup);
	wmsAction = new QAction(tr("WMS"), mapproviderGroup);
	osmAction->setCheckable(true);
	yahooActionMap->setCheckable(true);
	yahooActionSatellite->setCheckable(true);
	googleActionMap->setCheckable(true);
	osmAction->setChecked(true);
	wmsAction->setCheckable(true);
	connect(mapproviderGroup, SIGNAL(triggered(QAction*)),
			  this, SLOT(mapproviderSelected(QAction*)));
	
	yahooActionOverlay = new QAction(tr("Yahoo: street overlay"), this);
	yahooActionOverlay->setCheckable(true);
	yahooActionOverlay->setEnabled(false);
	connect(yahooActionOverlay, SIGNAL(toggled(bool)),
			  overlay, SLOT(setVisible(bool)));
}

void Mapviewer::createMenu()
{
	mapMenu = menuBar()->addMenu(tr("&Map Provider"));
	mapMenu->addAction(osmAction);
	mapMenu->addAction(yahooActionMap);
	mapMenu->addAction(yahooActionSatellite);
	mapMenu->addAction(googleActionMap);
	mapMenu->addAction(wmsAction);
	mapMenu->addSeparator();
	mapMenu->addAction(yahooActionOverlay);
}

void Mapviewer::mapproviderSelected(QAction* action)
{
	if (action == osmAction)
	{
		int zoom = mapadapter->adaptedZoom();
		mc->setZoom(0);
		
		mapadapter = new OSMMapAdapter();
		mainlayer->setMapAdapter(mapadapter);
		
		mc->updateRequestNew();
		mc->setZoom(zoom);
		yahooActionOverlay->setEnabled(false);
		overlay->setVisible(false);
		yahooActionOverlay->setChecked(false);
		
	} else if (action == yahooActionMap)
	{
		int zoom = mapadapter->adaptedZoom();
		mc->setZoom(0);
		
		mapadapter = new YahooMapAdapter();
		mainlayer->setMapAdapter(mapadapter);
	
		mc->updateRequestNew();
		mc->setZoom(zoom);
		yahooActionOverlay->setEnabled(false);
		overlay->setVisible(false);
		yahooActionOverlay->setChecked(false);
	} else if (action == yahooActionSatellite)
	{
		int zoom = mapadapter->adaptedZoom();
		QPointF a = mc->currentCoordinate();
		mc->setZoom(0);
		
		mapadapter = new YahooMapAdapter("us.maps3.yimg.com", "/aerial.maps.yimg.com/png?v=1.7&t=a&s=256&x=%2&y=%3&z=%1");
		mainlayer->setMapAdapter(mapadapter);
		
		mc->updateRequestNew();
		mc->setZoom(zoom);
		yahooActionOverlay->setEnabled(true);
	} else if (action == googleActionMap)
	{
		int zoom = mapadapter->adaptedZoom();
		mc->setZoom(0);
		mapadapter = new GoogleMapAdapter();
        //mapadapter = new TileMapAdapter("mt0.google.com", "/mt?v=w2t.80&hl=de&x=%2&s=&y=%3&zoom=%1&s=G", 256, 17, 0);
		mainlayer->setMapAdapter(mapadapter);
		
		mc->updateRequestNew();
		mc->setZoom(zoom);
		yahooActionOverlay->setEnabled(false);
		overlay->setVisible(false);
		yahooActionOverlay->setChecked(false);
	} else if (action == wmsAction)
	{
		int zoom = mapadapter->adaptedZoom();
		mc->setZoom(0);
		
		mapadapter = new WMSMapAdapter("www2.demis.nl", "/wms/wms.asp?wms=WorldMap&LAYERS=Countries,Borders,Cities,Rivers,Settlements,Hillshading,Waterbodies,Railroads,Highways,Roads&FORMAT=image/png&VERSION=1.1.1&SERVICE=WMS&REQUEST=GetMap&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage&SRS=EPSG:4326&TRANSPARENT=FALSE", 256);
		mainlayer->setMapAdapter(mapadapter);
	
		mc->updateRequestNew();
		mc->setZoom(zoom);
		yahooActionOverlay->setEnabled(false);
		overlay->setVisible(false);
		yahooActionOverlay->setChecked(false);
	}
}

Mapviewer::~Mapviewer()
{
}
