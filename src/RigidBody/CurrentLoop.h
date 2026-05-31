#if !defined(AFX_CURRENTLOOP_H__17E6C225_F662_41DB_8E7F_0CC865639AC4__INCLUDED_)
#define AFX_CURRENTLOOP_H__17E6C225_F662_41DB_8E7F_0CC865639AC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurrentLoop.h : header file
//

namespace SolveIt {

void PolyDraw95(HDC  hdc,				//for handle of a device context
                const POINT* lppt,		// array of points
                const BYTE* lpbTypes,	// line and curve identifiers
                int  cCount);			// count of points


//must be a multiple of 3:
const long MAXPOINTS = (3*4*6);
class CFieldsManager;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCurrentLoop dialog

class CCurrentLoop : public CDialog
{
// Construction
public:
//	CCurrentLoop(CWnd* pParent = NULL);   // standard constructor
	CCurrentLoop(CFieldsManager* pCFieldsManager, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCurrentLoop)
	enum { IDD = IDD_CURRENTLOOP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurrentLoop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation

	CFieldsManager* m_pCFieldsManager;

	bool m_bModalNotPersistant;

	int m_WindowWidth;
	int m_WindowHeight;
	double m_radius;

	bool m_bDragging;

	vector<Vector3D>& m_currentLoop;
	vector<Vector3D>& m_currentLoopDeltas;
	vector<Vector3D>& m_currentLoopDeltasMidpoint;

	vector<POINT> m_ptCircuitNodes;
	vector<RECT> m_ptCircuitRects;
	vector<RECT>::iterator i_SelectedCircuitRect;
	vector<BYTE> m_ptTypes;
	vector<POINT> m_ptFlatPath;
	vector<BYTE> m_ptFlatPathTypes;
	long m_NumPoints;

	vector<RECT>::iterator HitTest(const POINT& pt);

	POINT Transform(double x, double y);
//	RECT TransformToRect(double x, double y);
	Vector3D InvTransform(const POINT& pt);


	void RecordPath();
	void Initialize();
/////////////////////////////////////////////////////////////////////////////////
protected:

	static HCURSOR _threeCursors[3];

	// Generated message map functions
	//{{AFX_MSG(CCurrentLoop)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	afx_msg void OnClickedReset();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

} // namespace SolveIt

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURRENTLOOP_H__17E6C225_F662_41DB_8E7F_0CC865639AC4__INCLUDED_)
