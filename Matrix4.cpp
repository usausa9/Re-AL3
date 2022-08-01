#include "Matrix4.h"

void Matrix4::IdentityMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j = i)
			{
				m[j][i] = 1;
			}
			else
			{
				m[j][i] = 0;
			}
		}
	}
}