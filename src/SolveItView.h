// SolveItView.h : interface of the CSolveItView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVEITVIEW_H__C91BA0CB_E14A_47A5_880A_5C50DE613DD2__INCLUDED_)
#define AFX_SOLVEITVIEW_H__C91BA0CB_E14A_47A5_880A_5C50DE613DD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

//class CSolveItCntrItem;
class CTextBoxWindow;
class CSolveItDoc;
class CGenExamp;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class vecCEdit : public vector<CTextBoxWindow*>
{
public:
	virtual ~vecCEdit();
	void MoveVertically(CTextBoxWindow* p, int numRows, int cy);
	int GetBottom();
};
///////////////////////////////////////////////////////////////////////////////

class CSolveItView : public CFormView
{
	friend class CSolveItApp;
protected: // create from serialization only
	CSolveItView();
	DECLARE_DYNCREATE(CSolveItView)

// Attributes
public:
	//{{AFX_DATA(CSolveItView)
	//}}AFX_DATA
	enum { IDD = IDD_SolveItVIEW };
	CSolveItDoc* GetDocument();
	vecCEdit vec_pTextBoxWindow;
	long m_entryNumber;
	int m_Height;
	int m_Width;
	long m_lastLineNumber;
	CPoint m_ptCaret;// = GetCaretPos();

	CTextBoxWindow* m_pCTextBoxWindow;
//	CGenExamp*		m_pCGenExamp;

protected:
//	IRichEditOle	*m_pRichEditOle;

// Operations
public:
	bool OnCompile2(const char* buf);//auto_ptr<char>& abuf);
	const char* OnCompile(const char* buf);
	void SizeUpdate(int nAdditionalHeight);
///////////////////////////////////////////////////////////////////////////////
//	void CreateGenExampThread(bool bCREATE_SUSPENDED = false);
//	BOOL StopGenExamp(void);
///////////////////////////////////////////////////////////////////////////////
	void OnWriteResponse(const string& strText);
	void OnWriteCommand(const string& strText);
	void WriteBegin(BSTR strText);
	void WriteAdd(BSTR strText);
	void WriteEnd(BSTR strText);
///////////////////////////////////////////////////////////////////////////////
	void WriteBrowserStatusLine(const string& strText);
	void WritePrompt (bool bInput = true);
	void CreateTextBoxWindow();
	BOOL ClipCopy(const string& text);
	BOOL CreateOutputWindow(const wstring& text);
	bool OnUpdateNeedSel();
	bool OnUpdateNeedClip();
	void WriteCommand(BSTR strText);
///////////////////////////////////////////////////////////////////////////////
	string m_ScriptFileName;
	string m_Script;
///////////////////////////////////////////////////////////////////////////////

// Overrides, int _nCrLf = -1
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolveItView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
	virtual void OnDraw(CDC* pDC);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CSolveItView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL IsEdit( CWnd* pWnd );

// Generated message map functions
protected:
	//{{AFX_MSG(CSolveItView)
	//}}AFX_MSG
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); 
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditDelete();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnUpdateTextBoxWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT DoWriteCommand(WPARAM wParam, LPARAM lParam);

	afx_msg void WriteFileScript();
	afx_msg void ReadFileScript();


	afx_msg void OnSpline();
	afx_msg void OnTestPlot();
	afx_msg void OnTestRecursion();

	afx_msg void OnSum();
	afx_msg void OnDifference();
	afx_msg void OnCrossProduct();

	afx_msg void OnMultiBodyTest();
	afx_msg void OnMultiBodySlider();
	afx_msg void OnMultiMassSpring();
	afx_msg void OnMultiBodyChain();

	afx_msg void OnSimplePendulum();
	afx_msg void OnSimpleHarmonicOscillator();
	afx_msg void OnResonantSimpleHarmonicOscillator();
	afx_msg void OnCubicAnharmonicOscillator();
	afx_msg void OnAnharmonicOscillator();
	afx_msg void OnTest();
	afx_msg void OnLennardJones();
	afx_msg void OnLennardJonesHeatedCooled();
	afx_msg void OnRamp();
	afx_msg void OnPulley();
	afx_msg void OnTwoBodyScattering();
	afx_msg void OnThreeBodyCollision();
	afx_msg void OnFourBodyCollision();
	afx_msg void OnFourBodyCollision2();
	afx_msg void OnBallOnShpere();
	afx_msg void OnSphereAndBlock();
	afx_msg void OnTwoBlocksAndPulley();
	afx_msg void OnBlockAndPulley();
	afx_msg void OnBlockAndTackle();
	afx_msg void OnInelasticCollision();
	afx_msg void OnInelasticPendulumCollision();
	afx_msg void OnNewtonCradle();
	afx_msg void OnNewtonCradle2();
	afx_msg void OnUserDefinedTimeIndependentX();
	afx_msg void OnUserDefinedTimeIndependentY();
	afx_msg void OnUserDefinedTimeIndependentZ();
	afx_msg void OnUserDefinedTimeDependentX();
	afx_msg void OnUserDefinedTimeDependentY();
	afx_msg void OnUserDefinedTimeDependentZ();

	afx_msg void Scattering1Dim();
	afx_msg void Scattering2Dim();
	afx_msg void QM_Ground_State_1Dim();
	afx_msg void QM_Ground_State_2DimBEC();
	afx_msg void QMStationary1Dim();
	afx_msg void QMStationary1DimB();
	afx_msg void QMStationary2Dim();
	afx_msg void Superposition();
	afx_msg void OnN_Body();
	afx_msg void OnOrbitalMechanics();
	afx_msg void OnSunEarthMoon();
	afx_msg void OnEarthMoon();
	afx_msg void OnCannonBall();
	afx_msg void OnTwoBody();
	afx_msg void OnThreeBody();
	afx_msg void OnThreeChargedBodies();
	afx_msg void OnElectroStaticField();
	afx_msg void OnCyclotron();
	afx_msg void OnCurrentLoop();
	afx_msg void OnVerhulstDynamics();
	afx_msg void On6Body();
	afx_msg void OnChain();
	afx_msg void OnRandomChain();
	afx_msg void OnCompoundPendulum();
	afx_msg void OnPhysicalPendulum();
	afx_msg void OnTrapping();
	afx_msg void OnTwoNonShpericalBody();
	afx_msg void OnPendulumAndFixedCharge();
	afx_msg void OnPendulumAndPlanarCharge();
	afx_msg void OnSubmerged();
	afx_msg void Problem2_14();
	afx_msg void Problem2_37();
	afx_msg void Example2_4();
	afx_msg void Problem3_46();
	afx_msg void Problem3_51();
	afx_msg void Problem3_60();
	afx_msg void Example7_8();

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnUpdateTextBoxWindow();
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnUpdateTextBoxWindow(CCmdUI* pCmdUI);
#if 0
	afx_msg void OnPopupCopy();
	afx_msg void OnPopupCut();
	afx_msg void OnPopupDelete();
	afx_msg void OnPopupPaste();
	afx_msg void OnUpdatePopupCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupPaste(CCmdUI* pCmdUI);
#endif
/////////////////////////////////////////////////////////////////////////////
};

#ifndef _DEBUG  // debug version in SolveItView.cpp
inline CSolveItDoc* CSolveItView::GetDocument()
   { return (CSolveItDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_SOLVEITVIEW_H__C91BA0CB_E14A_47A5_880A_5C50DE613DD2__INCLUDED_)
