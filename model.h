// MODEL.H only defines some auxiliary matrix functions

// G. E. Smid
// Transportation Equipment Control (T.E.C.)
// Delft, September 1999

Matrix MakeRot(Matrix Angle)
{
 Matrix Rx(3,3), Ry(3,3), Rz(3,3);

 Rz(0,0)=cos(Angle(2,0)); Ry(0,0)=cos(Angle(1,0)); Rx(0,0)=1.0;
 Rz(0,1)=-sin(Angle(2,0));Ry(0,1)=0.0;             Rx(0,1)=0.0;
 Rz(0,2)=0.0;             Ry(0,2)=sin(Angle(1,0)); Rx(0,2)=0.0;
 Rz(1,0)=sin(Angle(2,0)); Ry(1,0)=0.0;             Rx(1,0)=0.0;
 Rz(1,1)=cos(Angle(2,0)); Ry(1,1)=1.0;             Rx(1,1)=cos(Angle(0,0));
 Rz(1,2)=0.0;             Ry(1,2)=0.0;             Rx(1,2)=-sin(Angle(0,0));
 Rz(2,0)=0.0;             Ry(2,0)=-sin(Angle(1,0));Rx(2,0)=0.0;
 Rz(2,1)=0.0;             Ry(2,1)=0.0;             Rx(2,1)=sin(Angle(0,0));
 Rz(2,2)=1.0;             Ry(2,2)=cos(Angle(1,0)); Rx(2,2)=cos(Angle(0,0));

 return Rz * Ry * Rx;
}

Matrix Cross(Matrix A, Matrix B)
{
	int k,j;
	Matrix Out(3,1);

	Out(0,0)=0.0;Out(1,0)=0.0;Out(2,0)=0.0;
	for (k=0;k<(int)A.ColNo();k++) {
		if (B.ColNo()>1) j=k; else j=0;

		Out(0,0) += A(1,k)*B(2,j) - A(2,k)*B(1,j);
		Out(1,0) += A(2,k)*B(0,j) - A(0,k)*B(2,j);
		Out(2,0) += A(0,k)*B(1,j) - A(1,k)*B(0,j);
	}
	return Out;
}
	
Matrix Transpose(Matrix R)
{
	int i,j;
	Matrix Out(3,3);
	for (i=0;i<3;i++) for (j=0;j<3;j++)
		Out(i,j) = R(j,i);
	return Out;
}

Matrix MakeRf(Matrix Angle)
{
	Matrix Out(3,3);
	double phi = Angle(0,0), theta = Angle(1,0), psi = Angle(2,0);

	Out(0,0) = cos(psi);  Out(0,1) = -sin(psi);
	Out(0,2) = sin(psi)*sin(phi)+cos(psi)*cos(phi)*sin(theta);
    Out(1,0) = sin(psi);  Out(1,1) = cos(psi);
	Out(1,2) = -cos(psi)*sin(phi)+sin(psi)*cos(phi)*sin(theta);
    Out(2,0) = 0.0;       Out(2,1) = 0.0;
	Out(2,2) = cos(phi)*cos(theta);
	return Out;
}

void Quaternion(float Q[], Matrix Angle)
{
	Matrix R(3,3);
	R = MakeRot(Angle);

   // Convert to quarternion (See Craig 2nd Ed. p.52)
   Q[3] = (float)(acos((R(0,0)+R(1,1)+R(2,2)-1)/2));
   Q[0] = (float)((R(2,1)-R(1,2))/(2*sin(Q[3])+1E-10));
	Q[1] = (float)((R(0,2)-R(2,0))/(2*sin(Q[3])+1E-10));
	Q[2] = (float)((R(1,0)-R(0,1))/(2*sin(Q[3])+1E-10));
}

