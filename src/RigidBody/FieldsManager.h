// FieldsManager.h: interface for the CFieldsManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDSMANAGER_H__D8BE8163_5EBF_453C_88D1_DAA761AE6916__INCLUDED_)
#define AFX_FIELDSMANAGER_H__D8BE8163_5EBF_453C_88D1_DAA761AE6916__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Types.h"
#include "Cruncher.h"
#include "FieldLineODE.h"

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt {
///////////////////////////////////////////////////////////////////////////////
class CCurrentLoop;
class CRigidBody;
class CFieldLineODEThread;
///////////////////////////////////////////////////////////////////////////////
typedef struct pair<Vector2D,Vector2D> PairVV2D;
typedef struct pair<Point3D,Vector3D> PairPV3D;
///////////////////////////////////////////////////////////////////////////////
struct vector_of_Vector
{
	union
	{
		vector_of_Vector2D* two;
		vector_of_Vector3D* three;
	};
	vector_of_Vector(vector_of_Vector2D* a):two(a){}
	vector_of_Vector(vector_of_Vector3D* a):three(a){}
};

class CFieldsManager   :
//	public vector<vector_of_double*>
	public vector<vector_of_Vector>
{
public:
	CFieldsManager();
	virtual ~CFieldsManager();
	void ClearVectorFieldLines();
	void ClearEquipotentials();
	void ClearFieldLines();
	void ClearVectorFields();
	void FieldLineData(const vector_of_double& data);
	void OnDraw();
///////////////////////////////////////////////////////////////////////////////
	void Clear();
	void Reset();
	void Edit();
	void Initialize();
///////////////////////////////////////////////////////////////////////////////
	Vector3D Static_GravitationalField (const Point3D& p, const CRigidBody* pBody = 0);
	Vector3D ElectrostaticField (const Point3D& p, const CRigidBody* pBody = 0);
//	Vector3D StaticMagneticField (const Point3D& p);
/////////////////////////////////////////////////////////////////////////////////
	CCurrentLoop* m_pCCurrentLoop;
	Vector3D MagneticField (const Point3D& p, const CRigidBody* pBody = 0);
	void RecordPath();
	void InitializePath();
	Point3D m_selectedPointForMagneticField;
	Point3D m_selectedFieldPoint;
	vector<PairPV3D> m_vecPairMagnetic_current_loop;
	double m_scaleMagneticField;
	double m_fScaleElectricField;
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void DrawVector2D(const Vector2D& pt);
	double V(const Vector2D& p);
	Vector2D E(const Vector2D& p);
	void DrawEquipotential2D(const Vector2D& pt, int niter=64, bool bRecord = false);
//	enum RecordPotential{ no, yes} m_RecordPotential;
	long m_RecordPotentialNumSteps;
///////////////////////////////////////////////////////////////////////////////
	bool m_bShowEquipotentials;
	bool m_bShowVectorFields;
	bool m_bShowFieldLines;
	void ShowEquipotentials(bool bNewValue);
	void ShowVectorFields(bool bNewValue);
	void ShowFieldLines(int nFieldLinesType);

	void ComputeFieldLine(int nFieldLinesType);
	void ComputeEquipotentialLine(void);
/////////////////////////////////////////////////////////////////////////////
	Vector3D N_Body_GravitationalField (const Point3D& p);
	Vector3D N_Body_ElectrostaticField (const Point3D& p);

	bool m_bShowN_Body_ElectrostaticField;
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	vector<PairVV2D> m_vecPair_2Delectrostatic;
	vector<PairPV3D> m_vecPair_electric;
/////////////////////////////////////////////////////////////////////////////////
	vector<PairPV3D> m_vecPair_current_loop;
	vector<PairPV3D> m_vecPair_gravitational;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	Vector2D m_CurrentMousePosition;
///////////////////////////////////////////////////////////////////////////////
	Vector3D m_currentEquipotentialPoint;
//	vector_of_double m_currentEquipotential;
	vector_of_Vector3D	m_currentEquipotential;
/////////////////////////////////////////////////////////////////////////////
	Vector2D m_electrostatic_currentFieldLinePoint2D;
//	vector_of_double m_electric_2D_currentFieldLine;
	vector_of_Vector2D	m_electric_2D_currentFieldLine;

	Vector3D m_electrostatic_currentFieldLinePoint;
//	vector_of_double m_electric_3D_currentFieldLine;
	vector_of_Vector3D	m_electric_3D_currentFieldLine;

//	Vector3D m_magnetic_current_loop_currentFieldLinePoint;
//	vector_of_double m_magnetic_current_loop_currentFieldLine;
//	vector_of_Vector3D	m_magnetic_current_loop_currentFieldLine;
/////////////////////////////////////////////////////////////////////////////
	Vector3D m_magnetostatic_currentFieldLinePoint;
	vector_of_Vector3D m_magnetostatic_currentFieldLine;

//	Vector3D m_magnetic_currentFieldLinePoint;
//	vector_of_double m_magnetic_currentFieldLine;

	Vector3D m_gravitational_currentFieldLinePoint;
//	vector_of_double m_gravitational_currentFieldLine;
	vector_of_Vector3D	m_gravitational_currentFieldLine;
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//	Mutex protectSoTransforms;
	Mutex protectFieldLines;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	void KillThreads();
///////////////////////////////////////////////////////////////////////////////
	FieldLineElectric2D*				m_threadFieldLineElectric2D;
	FieldLineElectric3D*				m_threadFieldLineElectric3D;
//	FieldLineMagnetic_current_loop*		m_threadFieldLineMagnetic_current_loop;
	FieldLineMagnetostatic*				m_threadFieldLineMagnetostatic;
//	FieldLineMagnetic*					m_threadFieldLineMagnetic;
	FieldLineGravitational*				m_threadFieldLineGravitational;
	FieldLine_Equipotential*			m_threadFieldLine_Equipotential;
///////////////////////////////////////////////////////////////////////////////
	void KillThread(CWinThread* worker);
///////////////////////////////////////////////////////////////////////////////
	enum ThreadType {
		thread_time_step,
		thread_fieldline_gravitational,
		thread_fieldline_electrostatic,
		thread_fieldline_electric,
		thread_fieldline_magnetic_current_loop,
		thread_fieldline_magnetostatic,
		thread_fieldline_magnetic,
		thread_fieldline__Equipotential
	} m_nThreadType;
	void KillThread(ThreadType threadType);
///////////////////////////////////////////////////////////////////////////////
	void FieldLine(ThreadType t);
	void FieldLines(ThreadType t, Vector3D from, Vector3D to, Vector3D dx);
	Vector3D Compute_FieldLine_Tangent(ThreadType threadType, const Point3D& p, double t);
	Vector3D m_fromFieldLines;
	Vector3D m_toFieldLines;
	Vector3D m_drFieldLines;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

	double m_GravitationalCoupling;

	Vector3D m_ExternalGravitationalField;// usually 0 or ~ -3

	double m_ExternalElectrostaticFieldX1;//linear term in Ex
	double m_ExternalElectrostaticFieldX2;//squared term in Ex
	double m_ExternalElectrostaticFieldX3;//cubic term in Ex

	bool m_bSinusoidalForce_Component_X_IsValid;
	bool m_bSinusoidalForce_Component_Y_IsValid;
	bool m_bSinusoidalForce_Component_Z_IsValid;

	Vector3D m_SinusoidalForce_Component_X;
	Vector3D m_SinusoidalForce_Component_Y;
	Vector3D m_SinusoidalForce_Component_Z;

	bool m_bCentralForce_IsValid;
	double m_fCentralForce;

	bool m_bLennardJones_IsValid;
	Vector3D m_fLennardJones;//[x != 0 => valid; y / r^6 - z / r^12 
	void Set_LENNARD_JONES(const Vector3D& val);//const double& x,const double& y,const double& z);
///////////////////////////////////////////////////////////////////////////////

	bool m_bTimeDependentExternalForce_Component_X_IsValid;
	bool m_bTimeDependentExternalForce_Component_Y_IsValid;
	bool m_bTimeDependentExternalForce_Component_Z_IsValid;

	Vector3D m_ExternalMagneticField;
	Vector3D m_ExternalElectricField;

	bool m_bValidCurrentLoopMagneticField;

	vector<Vector3D> m_currentLoop;
	vector<Vector3D> m_currentLoopDeltas;
	vector<Vector3D> m_currentLoopDeltasMidpoint;
	double m_currentLoopMagneticFieldStrength;
	friend Vector3D CurrentLoopMagneticField(const Point3D& p);

	void OnCurrentLoopMagneticField(double currentLoopMagneticFieldStrength);

};

} // namespace SolveIt

#endif // !defined(AFX_FIELDSMANAGER_H__D8BE8163_5EBF_453C_88D1_DAA761AE6916__INCLUDED_)
