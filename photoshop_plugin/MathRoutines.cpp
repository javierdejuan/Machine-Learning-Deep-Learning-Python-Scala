//#include "FilterBigDocument.h"
//#include "PIDefines.h"
//#include "PITypes.h"
//#include "PIFilter.h"
//#include "Logger.h"
//#include "Hexaedro.h"
//#include "SingletonInit.h"
//#include "math.h"
#include "stdafx.h"


void CHexaedro::getXYZ(const uint16& u, const uint16& v, double* vector)
{
	double lado = (double)m_faces[0].getSide();
	double OC[2] = { 0. };
	double& x = vector[0];
	double& y = vector[1];
	double& z = vector[2];

	switch (m_faceToProcess)
	{

	case efaces::right:		// y = -a / 2, horizontal - x, vertical - z
	{

		OC[0] = 2.5*lado;
		OC[1] = 1.5*lado;
		y = -lado / 2.;
		x = OC[0] - u;		// translacion horizontal
		z = OC[1] - v;		// translacion vertical
		break;
	}

	case efaces::left:	// y = a / 2, horizontal + x, vertical - z
	{

		OC[0] = 0.5*lado;
		OC[1] = 1.5*lado;
		y = lado / 2.;
		x = u - OC[0];		// translacion horizontal
		z = OC[1] - v;		 // translacion vertical
		break;
	}

	case efaces::front: // x = a / 2, horizontal + x, vertical - z
	{


		OC[0] = 1.5*lado;
		OC[1] = 1.5*lado;
		x = lado / 2;
		y = OC[0] - u;// translacion horizontal
		z = OC[1] - v;// translacion vertical
		break;
	}


	case efaces::back: // x = -a / 2, horizontal + x, vertical - z
	{

		OC[0] = 3.5*lado;
		OC[1] = 1.5*lado;
		x = -lado / 2.;
		y = u - OC[0];		// translacion horizontal
		z = OC[1] - v;		// translacion vertical
		break;
	}

	case efaces::top: // z=a/2, horizontal -y, vertical +x
	{


		OC[0] = 1.5*lado;
		OC[1] = 0.5*lado;
		z = lado / 2.;
		y = OC[0] - u;		// translacion horizontal
		x = v - OC[1];		// translacion vertical
		break;
	}

	case efaces::bottom: // z = -a / 2, horizontal - y, vertical - x
	{

		OC[0] = 1.5*lado;
		OC[1] = 2.5*lado;
		z = -lado / 2.;
		y = OC[0] - u;		// translacion horizontal
		x = OC[1] - v;		// translacion vertical
		break;
	}

	}


}

void CHexaedro::CubeIntersection(const double* vector, double* i_v, efaces& face)
{
	double& x = i_v[0];
	double& y = i_v[1];
	double& z = i_v[2];

	double a = fabs(vector[0]);
	double b = fabs(vector[1]);
	double c = fabs(vector[2]);

	double medio_lado = (double)m_faces[0].getSide() / 2.;

	double maximo = max(a, max(b, c));
	double k = 0.;


	if (maximo == a)
	{
		x = vector[0] / a * (medio_lado);            // interseccion con el plano x = +-lado / 2
		k = x / vector[0];
		y = k * vector[1];
		z = k * vector[2];
		face = efaces::front;
		if (x < 0)
			face = efaces::back;
	}
	else if (maximo == b)
	{
		y = vector[1] / b * (medio_lado); // interseccion con el plano y = +-lado / 2
		k = y / vector[1];
		x = k * vector[0];
		z = k * vector[2];
		face = efaces::left;
		if (y < 0)
			face = efaces::right;
	}

	else if (c == maximo)
	{

		z = vector[2] / c * (medio_lado); // interseccion con el plano z = +-lado / 2
		k = z / vector[2];
		x = k * vector[0];
		y = k * vector[1];
		face = efaces::bottom;
		if (z > 0)
			face = efaces::top;
	}
	else
	{
		int kk;
		kk = 0;
		//logger("Unable to solve impact");
	}


}

void CHexaedro::getUV(const double* intersect_vector, const efaces& face_impacted, VPoint& PixelTransformed)
{
	int32& u = PixelTransformed.h;
	int32& v = PixelTransformed.v;

	int32 lado = (int32)m_faces[0].getSide();
	int32 OC[2] = { 0 };

	int32 x = (int32)round(intersect_vector[0]);
	int32 y = (int32)round(intersect_vector[1]);
	int32 z = (int32)round(intersect_vector[2]);

	if (face_impacted == efaces::right)
	{									// y = -a / 2, horizontal - x, vertical - z

		OC[0] = (int32)(2.5*lado);
		OC[1] = (int32)(1.5*lado);

		u = OC[0] - x;		// translacion horizontal
		v = OC[1] - z;		// translacion vertical
	}
	else if (face_impacted == efaces::left)
	{    // y = a / 2, horizontal + x, vertical - z

		OC[0] = (int32)(0.5*lado);
		OC[1] = (int32)(1.5*lado);

		u = OC[0] + x;  // translacion horizontal
		v = OC[1] - z;	//translacion vertical
	}

	else if (face_impacted == efaces::front)       // x = a / 2, horizontal - y, vertical - z
	{

		OC[0] = (int32)(1.5*lado);
		OC[1] = (int32)(1.5*lado);

		u = OC[0] - y; // translacion horizontal
		v = OC[1] - z; // translacion vertical
	}
	else if (face_impacted == efaces::back)// x = -a / 2, horizontal + y, vertical - z
	{

		OC[0] = (int32)(3.5*lado);
		OC[1] = (int32)(1.5*lado);

		u = OC[0] + y; // translacion horizontal
		v = OC[1] - z; // translacion vertical
	}
	else if (face_impacted == efaces::top)
	{         // z = a / 2, horizontal - y, vertical + x


		OC[0] = (int32)(1.5*lado);
		OC[1] = (int32)(0.5*lado);

		u = OC[0] - y;// translacion horizontal
		v = OC[1] + x;// translacion vertical
	}
	else if (face_impacted == efaces::bottom)	//z = -a / 2, horizontal - y, vertical - x
	{
		OC[0] = (int32)(1.5 *lado);
		OC[1] = (int32)(2.5 * lado);

		u = OC[0] - y;	// translacion horizontal
		v = OC[1] - x;	// translacion vertical
	}
	else
	{
		int kk;
		kk = 0;
	}

}

bool CalcRectIntersection( VRect* a, VRect* b,VRect* intersect)
{

	bool bIntersect = false;

	intersect->bottom	= 0;
	intersect->left		= 0;
	intersect->right	= 0;
	intersect->bottom	= 0;

	intersect->left		= max(a->left, b->left);
	intersect->top		= max(a->top, b->top);
	intersect->right	= min(a->right, b->right);
	intersect->bottom	= min(a->bottom, b->bottom);

	bIntersect = ((intersect->right > intersect->left) && (intersect->bottom > intersect->top));

	return bIntersect;
	
}