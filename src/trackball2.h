//	(c) 2000 J. Gans (jdg9@cornell.edu), Shalloway Lab, 
//	Cornell University. You can modify and freely distribute 
//	this code, but please keep this header intact.
//
//	Look-and-feel inspired by Xmol 
//	(http://www.networkcs.com/msc/docs/xmol/).
//
//	OpenGL selection code and quaterion rotation matrix
//	formulation from molview demo program by Mark Kilgard
//	(http://reality.sgi.com/opengl/OpenGLforX.html).
//
//	The printing routines are based on code from
//	Craig Fahrnbach <craignan@home.com> via
//	Uwe Kotyczka <kotyczka@bnm-gmbh.de>.
//
//	Covalent bond determination code based on VMD
//	http://www.ks.uiuc.edu/Research/vmd/
//
//	Stide (http://www.embl-heidelberg.de/stride/stride.html)
//	is used for secondary structure determination and has been 
//	included in Qmol with the kind permission of 
//
//	Dmitrij Frishman, PhD
//	Institute for Bioinformatics
//	GSF - Forschungszentrum f�r Umwelt und Gesundheit, GmbH
//	Ingolst�dter Landstra�e 1,
//	D-85764 Neuherberg, Germany
// 
//	Telephone: +49-89-3187-4201
//	Fax: +49-89-31873585
//	e-mail: d.frishman@gsf.de
//	WWW: http://mips.gsf.de/mips/staff/frishman/
// 
// Controll the rotation of the molecule(s) with a virtual
// trackball. Based on the trackball code by M. Kilgard.

// Include the OpenGL headers

// The default trackball size (i.e. mouse senesitivity).
#define DEFAULT_TRACKBALLSIZE  (0.8)

#define MAX_TRACKBALLSIZE  (1.5)
#define MIN_TRACKBALLSIZE  (0.4)

#define RENORMCOUNT 97

/////////////////////////////////////////////////////////////////////////////////
namespace SolveIt
{
///////////////////////////////////////////////////////////////////////////////

class VirtualTrackBall{
	private:
		// Quaterions for the trackball state
		double curquat[4];
		double lastquat[4];

		void axis_to_quat(Vector3D &a, double phi);
		double tb_project_to_sphere(double r, double x, double y);
		double trackballsize;
	public:
		VirtualTrackBall();
///////////////////////////////////////////////////////////////////////////////
	void Reset()
	{
		curquat[0] = lastquat[0] = 0.0f;
		curquat[1] = lastquat[1] = 0.0f;
		curquat[2] = lastquat[2] = 0.0f;
		curquat[3] = lastquat[3] = 1.0f;
		trackballsize = DEFAULT_TRACKBALLSIZE;
	}

	Vector3D axis() const
	{
		Vector3D a(curquat[0] , curquat[1] , curquat[2]);
		if (a.norm() == 0.0) a = Vector3D(0,0,1);	// axis is arbitrary here
		else a.normalize();
		return a;
	}

	double angle() const {return 2 * acos(curquat[3]);}


	void update();
	void build_rotmatrix(double m[4][4]);

	void trackball(double p1x, double p1y, double p2x, double p2y);

		// Allow variable rotational sensitivity by adjusting
		// the size of the virtual track ball.
		void IncreaseBallSize()
		{
			// Don't allow a track ball greater than MAX_TRACKBALLSIZE.
			if(trackballsize < MAX_TRACKBALLSIZE){
				trackballsize += 0.1f;
			}
		};

		void DecreaseBallSize()
		{
			// Don't allow a track ball less than MIN_TRACKBALLSIZE.
			if(trackballsize > MIN_TRACKBALLSIZE){
				trackballsize -= 0.1f;
			}
		};

		void DefaultBallSize()
		{
			trackballsize = DEFAULT_TRACKBALLSIZE;
		};


///////////////////////////////////////////////////////////////////////////////
void vzero(double *v) {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}
void vset(double *v, double x, double y, double z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}
void vsub(const double *src1, const double *src2, double *dst) {
    dst[0] = src1[0] - src2[0];
    dst[1] = src1[1] - src2[1];
    dst[2] = src1[2] - src2[2];
}
void vcopy(const double *v1, double *v2) {
    int i;
    for (i = 0 ; i < 3 ; i++)
        v2[i] = v1[i];
}
void vcross(const double *v1, const double *v2, double *cross) {
    double temp[3];

    temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    vcopy(temp, cross);
}
double vlength(const double *v) {
    return double(sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
}
void vscale(double *v, double div) {
    v[0] *= div;
    v[1] *= div;
    v[2] *= div;
}
void vnormal(double *v) {
    vscale(v,double(1.0/vlength(v)));
}
double vdot(const double *v1, const double *v2) {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}
void vadd(const double *src1, const double *src2, double *dst) {
    dst[0] = src1[0] + src2[0];
    dst[1] = src1[1] + src2[1];
    dst[2] = src1[2] + src2[2];
}
void negate_quat(double q[4], double nq[4]) {
    nq[0] = -q[0];
    nq[1] = -q[1];
    nq[2] = -q[2];
    nq[3] = q[3];
}
void add_quats(double q1[4], double q2[4], double dest[4]) {
    static int count=0;
    double t1[4], t2[4], t3[4];
    double tf[4];
#if 0
printf("q1 = %f %f %f %f\n", q1[0], q1[1], q1[2], q1[3]);
printf("q2 = %f %f %f %f\n", q2[0], q2[1], q2[2], q2[3]);
#endif
    vcopy(q1,t1);
    vscale(t1,q2[3]);

    vcopy(q2,t2);
    vscale(t2,q1[3]);

    vcross(q2,q1,t3);
    vadd(t1,t2,tf);
    vadd(t3,tf,tf);
    tf[3] = q1[3] * q2[3] - vdot(q1,q2);
#if 0
printf("tf = %f %f %f %f\n", tf[0], tf[1], tf[2], tf[3]);
#endif
    dest[0] = tf[0];
    dest[1] = tf[1];
    dest[2] = tf[2];
    dest[3] = tf[3];

#define RENORMCOUNT 97
    if (++count > RENORMCOUNT) {
        count = 0;
        normalize_quat(dest);
    }
}
void normalize_quat(double q[4]) {
    int i;
    double mag;
    mag = double(sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]));
    for (i = 0; i < 4; i++) q[i] /= mag;
}

};
////////////////////////////////////////////////////////////////////////
}//end namespace SolveIt

