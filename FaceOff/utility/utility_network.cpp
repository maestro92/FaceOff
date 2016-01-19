#include "utility.h"

void utl::readBitStream(RakNet::BitStream& bsIn, glm::vec3& v)
{	
	/*
	bsIn.Read(v.x);
	bsIn.Read(v.y);
	bsIn.Read(v.z);
	*/
	bsIn.ReadVector(v.x, v.y, v.z);
}

void utl::setBitStream(RakNet::BitStream& bsOut, glm::vec3& v)
{
	/*
	bsOut.Write(v.x);
	bsOut.Write(v.y);
	bsOut.Write(v.z);
	*/
	bsOut.WriteVector(v.x, v.y, v.z);
}


