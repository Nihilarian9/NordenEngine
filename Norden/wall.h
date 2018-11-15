
class Wall
{
public:
	struct wallVerts {
		glm::vec3 A, B, C, D;
		glm::vec3 a, b, c, d;
		glm::vec2 At, Bt, Ct, Dt;
		glm::vec2 at, bt, ct, dt;
	};
	
	float wallAngle;
	float aAngle, bAngle, cAngle, dAngle;
	float offset = 10.0f;
	float wallHeight = 0.5f;
	glm::vec3 wallPos;
	
	wallVerts wallVs;
	const static int numVerts = 12 * 3;

	float wallArray[numVerts];// move to HEAP once VBO established.

	void buildFort(float rotation, glm::vec3 wallPosition, int wallNum)
	{
		//	set variables
		wallAngle = rotation; // wall rotation is same as tangent angle.
		wallPos = wallPosition;
		
		//	set angles for determining vertex xz
		aAngle = glm::radians(wallAngle - 90.0f + offset);
		bAngle = glm::radians(wallAngle + 90.0f - offset);
		cAngle = glm::radians(wallAngle - 90.0f - offset);
		dAngle = glm::radians(wallAngle + 90.0f + offset);
		
		wallVs.a.x = cos(aAngle) / 2 + wallPos.x;
		wallVs.A.x = cos(aAngle) / 2 + wallPos.x;
		wallVs.a.y = wallPos.y;
		wallVs.A.y = wallPos.y + wallHeight;
		wallVs.a.z = sin(aAngle) / 2 + wallPos.z;
		wallVs.A.z = sin(aAngle) / 2 + wallPos.z;
		wallVs.at.x = 0.0f;
		wallVs.at.y = 0.0f;
		wallVs.At.x = 0.0f;
		wallVs.At.y = 1.0f;

		wallVs.b.x = cos(bAngle) / 2 + wallPos.x;
		wallVs.B.x = cos(bAngle) / 2 + wallPos.x;
		wallVs.b.y = wallPos.y;
		wallVs.B.y = wallPos.y + wallHeight;
		wallVs.b.z = sin(bAngle) / 2 + wallPos.z;
		wallVs.B.z = sin(bAngle) / 2 + wallPos.z;
		wallVs.bt.x = 1.0f;
		wallVs.bt.y = 0.0f;
		wallVs.Bt.x = 1.0f;
		wallVs.Bt.y = 1.0f;

		wallVs.c.x = cos(cAngle) / 2 + wallPos.x;
		wallVs.C.x = cos(cAngle) / 2 + wallPos.x;
		wallVs.c.y = wallPos.y;
		wallVs.C.y = wallPos.y + wallHeight;
		wallVs.c.z = sin(cAngle) / 2 + wallPos.z;
		wallVs.C.z = sin(cAngle) / 2 + wallPos.z;
		wallVs.ct.x = 0.0f;
		wallVs.ct.y = 0.0f;
		wallVs.Ct.x = 0.0f;
		wallVs.Ct.y = 1.0f;

		wallVs.d.x = cos(dAngle) / 2 + wallPos.x;
		wallVs.D.x = cos(dAngle) / 2 + wallPos.x;
		wallVs.d.y = wallPos.y;
		wallVs.D.y = wallPos.y + wallHeight;
		wallVs.d.z = sin(dAngle) / 2 + wallPos.z;
		wallVs.D.z = sin(dAngle) / 2 + wallPos.z;
		wallVs.dt.x = 1.0f;
		wallVs.dt.y = 0.0f;
		wallVs.Dt.x = 1.0f;
		wallVs.Dt.y = 1.0f;

		//	transfer calculations directly once sure of accuracy.
		// first square
		int arrayMod = 0;
		int iteratorMod = 3;
		
		wallArray[arrayMod + 0] = wallVs.A.x;		wallArray[arrayMod + 1] = wallVs.A.y;		wallArray[arrayMod + 2] = wallVs.A.z;
		//wallArray[triMod + 3] = wallVs.At.x;		wallArray[triMod + 4] = wallVs.At.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.B.x;		wallArray[arrayMod + 1] = wallVs.B.y;		wallArray[arrayMod + 2] = wallVs.B.z;
		//wallArray[triMod + 3] = wallVs.Bt.x;		wallArray[triMod + 4] = wallVs.Bt.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.a.x;		wallArray[arrayMod + 1] = wallVs.a.y;		wallArray[arrayMod + 2] = wallVs.a.z;
		//wallArray[triMod + 3] = wallVs.at.x;		wallArray[triMod + 4] = wallVs.at.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.B.x;		wallArray[arrayMod + 1] = wallVs.B.y;		wallArray[arrayMod + 2] = wallVs.B.z;
		//wallArray[triMod + 3] = wallVs.Bt.x;		wallArray[triMod + 4] = wallVs.Bt.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.a.x;		wallArray[arrayMod + 1] = wallVs.a.y;		wallArray[arrayMod + 2] = wallVs.a.z;
		//wallArray[triMod + 3] = wallVs.at.x;		wallArray[triMod + 4] = wallVs.at.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.b.x;		wallArray[arrayMod + 1] = wallVs.b.y;		wallArray[arrayMod + 2] = wallVs.b.z;
		//wallArray[triMod + 3] = wallVs.bt.x;		wallArray[triMod + 4] = wallVs.bt.y;
		arrayMod += iteratorMod;

		// second square

		wallArray[arrayMod + 0] = wallVs.C.x;		wallArray[arrayMod + 1] = wallVs.C.y;		wallArray[arrayMod + 2] = wallVs.C.z;
		//wallArray[triMod + 3] = wallVs.Ct.x;		wallArray[triMod + 4] = wallVs.Ct.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.D.x;		wallArray[arrayMod + 1] = wallVs.D.y;		wallArray[arrayMod + 2] = wallVs.D.z;
		//wallArray[triMod + 3] = wallVs.Dt.x;		wallArray[triMod + 4] = wallVs.Dt.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.c.x;		wallArray[arrayMod + 1] = wallVs.c.y;		wallArray[arrayMod + 2] = wallVs.c.z;
		//wallArray[triMod + 3] = wallVs.ct.x;		wallArray[triMod + 4] = wallVs.ct.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.D.x;		wallArray[arrayMod + 1] = wallVs.D.y;		wallArray[arrayMod + 2] = wallVs.D.z;
		//wallArray[triMod + 3] = wallVs.Dt.x;		wallArray[triMod + 4] = wallVs.Dt.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.c.x;		wallArray[arrayMod + 1] = wallVs.c.y;		wallArray[arrayMod + 2] = wallVs.c.z;
		//wallArray[triMod + 3] = wallVs.ct.x;		wallArray[triMod + 4] = wallVs.ct.y;
		arrayMod += iteratorMod;
		wallArray[arrayMod + 0] = wallVs.d.x;		wallArray[arrayMod + 1] = wallVs.d.y;		wallArray[arrayMod + 2] = wallVs.d.z;
		//wallArray[triMod + 3] = wallVs.dt.x;		wallArray[triMod + 4] = wallVs.dt.y;
		
	}

};
