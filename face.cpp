#include "face.h"

face::face(int v1, int v2, int v3, int vt1, int vt2, int vt3, int vn1, int vn2, int vn3)
{
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;

	vt[0] = vt1;
	vt[1] = vt2;
	vt[2] = vt3;

	vn[0] = vn1;
	vn[1] = vn2;
	vn[2] = vn3;
}