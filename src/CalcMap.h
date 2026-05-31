#if !defined(AFX_CALCMAP_H__13C56B9B_137B_4493_B7A3_66C176F7AB1D__INCLUDED_)
#define AFX_CALCMAP_H__13C56B9B_137B_4493_B7A3_66C176F7AB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{


#define ROW_AGENT_NONE 0

// RowInfo struct
struct RowInfo {
  bool calculated;
  int  agent;
};


class PixelBuffer
{
protected:
  // Width in pixels
  const int width_;
   // Height in pixels
  const int height_;
  // RowInfo structure
  struct RowInfo* rowInfos_;
  // Buffer pointer
  unsigned int* mem_;


public:
	PixelBuffer(int w, int h);
	virtual ~PixelBuffer();
  unsigned int getWidth();
  unsigned int getHeight();
  unsigned int* getRowPointer(int row);
  unsigned int getNumPixels();
  RowInfo* getRowInfo(int row);
  bool isRowCalculated(int row);
  void setRowCalculated(int row, bool calculated);
  int getRowAgent(int row);
  unsigned int getTotalIters(unsigned int maxi, unsigned int iterBlack);
  void setRowAgent(int row, int agent);
  void clearBuffer();
protected:
  void resetRow(int row);
};
/*
static double four = 4.0;
static int w_=0; // Window width
static double ax_ = -2.0;
static double sx_, sy_;
*/
/////////////////////////////////////////////////////////////////////////////
// CCalcMap thread
#define ITER_BLACK 0xFFFFFFFF
#define ITER_WHITE 0xFFFFFFFE

class CCalcMap : public CWinThread
{
	DECLARE_DYNCREATE(CCalcMap)
protected:
	CCalcMap();           // protected constructor used by dynamic creation

// Attributes
public:
int h_, w_; // Window height
double ax_, ay0_, ex0_, ey0_, sx_;

PixelBuffer* pixBuf_;// = NULL;


//const int ITER_BLACK;// 0xFFFFFFFF
unsigned int maxi_;// = 40;
double ay_;// = -1.5;
double ex_;// =  1.0;
double ey_;// =  1.5;
double sy_;

GLubyte* glArrColor_;// = NULL;
GLfloat* glArrVertex_;// = NULL;
GLubyte* colorTable_;// = NULL;
bool doublebuf_;// = false;
bool useGLArrays_;// = true;
bool reshaped_;// = false;

// Operations
public:
bool renderImage(unsigned int maxi, int mode);
bool calcPixelRow(int row, unsigned int maxi, int mode);
bool calcPixelRow_C(int row, unsigned int maxi);
bool calcPixelRow_FPU_ASM(int row, unsigned int maxi);

bool renderPixelRow(int row, int mode);
void prepareColorTable(int numColors, GLubyte startR, GLubyte startG, GLubyte startB);

	void OnDraw();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcMap)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCalcMap();

	// Generated message map functions
	//{{AFX_MSG(CCalcMap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnStep(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStepDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHalt(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetParam(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt


#endif // !defined(AFX_CALCMAP_H__13C56B9B_137B_4493_B7A3_66C176F7AB1D__INCLUDED_)
