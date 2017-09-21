#pragma once
#include <vector>
#include <set>
#include <map>
#include <boost/unordered_map.hpp>
#include "DataDefine.h"
#include "Global.h"
#include "Type.h"
class ChartManager;
class ChartXYData
{
public:
	ChartXYData();
	~ChartXYData();
	void setXYData(std::vector<CMeDPoint>& data);
	const std::vector<CMeDPoint>& getData();
	void setColor(CMeColor color);
	CMeColor getColor();
	void setID(const SENSOR_TYPE_KEY& id)
	{
		ID = id;
	}
	SENSOR_TYPE_KEY getID()
	{
		return ID;
	}
private:
	std::vector<CMeDPoint> mData;
	CMeColor mColor;
	SENSOR_TYPE_KEY ID;
	//bool mVisible;
};

class MarkPoint
{
public:
	MarkPoint();
	~MarkPoint();
	void paint();
};
enum ChartType
{
	E_CHART_LINE,				//��
	E_CHART_STRIP,				//��״
	E_CHART_PIE,				//��״
	E_CHART_STRIP_ADD,			//������״ͼ
	E_CHART_AREA,				//���ͼ
	E_CHART_POLAR_COORDINATE,	//������
	E_CHART_RADAR,				//�״�
	E_CHART_RADAR_FILL,			//�״����
	E_CHART_YINYANG,			//����ͼ
	E_CHART_HILO,				//С����
	E_CHART_HILO_OC,			//С��������
	E_CHART_BUBBLE,				//����
};
enum LineStyle
{
	E_LINE_DOT,
	E_LINE_LINE,
	E_LINE_DOT_LINE,
};
enum MoveStyle
{
	E_X_SCROLL,
	E_X_SHOWALL,
	E_X_HANDLE,
};
enum OperateMode 
{
	E_OPE_DRAG,
	E_OPE_SELECT,
};
class ChartXY
{
public:
	ChartXY(HDC hDC);
	~ChartXY();
	
	enum MouseInArea{
		E_AREA_NULL,
		E_AREA_X,
		E_AREA_Y,
		E_AREA_VIEW,
	};
public:
	void paintEvent();
	void mousePressEvent(CMeDPoint msPos);
	void mouseReleaseEvent(CMeDPoint msPos);
	void mouseMoveEvent(CMeDPoint msPos);
	void wheelEvent(CMeDPoint msPos, int deltaWh);
	void resize(CSize size);
	void showAll();
	void zoomIn(CMeDPoint cPt);
	void ZoomOut(CMeDPoint cPt);

	//virtual bool eventFilter(QObject *obj, QEvent *e);
protected:
	void drawAxesXY();
	void drawLine();
	void drawStrip();
	void drawSpripAdd();
	void drawPie();
	void drawArea();
	void drawPolarCoordinate();
	void drawRadar();
	void drawRadarFill();
	void drawYinYang();
	void drawHilo();
	void drawHiloOC();
	void drawBubble();

	CMeDPoint screen2xy(CMeDPoint pt);
	CMeDPoint xy2screen(CMeDPoint pt);

	void calcRecView();

public:
	void setXRange(double min, double max);
	void setYRange(double min, double max);
	void getXRange(double& min, double& max);
	void getYRange(double& min, double& max);
	//void addChartData(ChartXYData* data);
	//const ChartXYData* chartData(int index);
	//void removeChartData(int index);
	//void removeAllChartData();
	int calNumDigit(double num);
	//��������
	void setChartMgr(ChartManager* mgr);
	const ChartManager* getChartMgr();
	//update visible and column
	void setVisible(SENSOR_TYPE_KEY id, bool bShow);
	bool getVisible(SENSOR_TYPE_KEY id);

	boost::unordered_map<SENSOR_TYPE_KEY, bool> getMapVisible() { return m_mapVisible; }
	void setMapVisible(const boost::unordered_map<SENSOR_TYPE_KEY, bool>& m) { m_mapVisible = m; }

	//void setXID(int id);
	void setXID(const SENSOR_TYPE_KEY& id);
	SENSOR_TYPE_KEY getXID();
	//��������
	void updateData();
	void refreshData();

	LineStyle getLineStyle() { return m_eLineStyle; }
	void setLineStyle(LineStyle e) { m_eLineStyle = e; }
	MoveStyle getMoveStyle() { return m_eMoveStyle; }
	void setMoveStyle(MoveStyle e) { m_eMoveStyle = e; }
	ChartType getChartType() { return m_eChartType; }
	void setChartType(ChartType e) { m_eChartType = e; }
	void setOperateMode(OperateMode e) {
		m_eOpeMode = e;
	}
	OperateMode getOperateMode() {
		return m_eOpeMode;
	}
	void setCheckVal(bool v) {
		m_bCheckVal = v;
	}
	bool getCheckVal() {
		return m_bCheckVal;
	}
	void setQieXian(bool v)
	{
		m_bQieLine = v;
	}
	bool getQieXian()
	{
		return m_bQieLine;
	}
	void setStatistics(bool v)
	{
		m_bStatistics = v;
	}
	bool getStatistics()
	{
		return m_bStatistics;
	}
	void calcXYRange();
private:
	ChartType m_eChartType;
	MoveStyle m_eMoveStyle;
	LineStyle m_eLineStyle;
	OperateMode m_eOpeMode;
	CRect m_recView;
	bool m_bCheckVal;
	bool m_bQieLine;
	double m_dCkLineX;
	std::vector<std::string> m_vecValQL;
	std::vector<CMeLine> m_vecQieLine;
	bool m_bStatistics;
	//��Χ
	double m_minX;
	double m_maxX;
	double m_minY;
	double m_maxY;
	bool m_bMouseLBtn;
	CMeDPoint m_oldMsPt;
	//�����������
	MouseInArea m_eMouseArea;
	CRect m_recXAxes;
	CRect m_recYAxes;
	HDC m_hDC;
	CDC* m_pDC;
	CSize m_size;
	CDC* m_pMemDC;
	ChartManager* m_chartMgr;
	//����ʾ���� ID
	boost::unordered_map<SENSOR_TYPE_KEY, bool> m_mapVisible;
	SENSOR_TYPE_KEY m_nXID;
	CString m_strX;
	CString m_strY;
	//std::vector<GROUPDATA> m_allData;
	//����ֵ ����ʱҪ���¼���
	std::vector<ChartXYData> m_vecLineData;
};


