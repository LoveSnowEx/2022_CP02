#include "myvector.h"

sVector *myvector_init() {
	return calloc(1, sizeof(sVector));
}

int myvector_set(sVector *pVector, uint8_t type, double a, double b) {
	if(type != 1 && type != 2) return -1;
	pVector->type = type;
	if(type == 1) {
		pVector->data.c.x = a;
		pVector->data.c.y = b;
	}
	else {
		pVector->data.p.distance = a;
		pVector->data.p.angle = b;
	}
	return 0;
}

int myvector_print(const sVector *pVector, uint8_t type) {
	if(type != 1 && type != 2) return -1;
	sVector tmp = *pVector;
	myvector_convert(&tmp, type);

	if(type == 1) printf("(%g,%g)\n", tmp.data.c.x, tmp.data.c.y);
	else printf("(%g,%g-pi)\n", tmp.data.p.distance, tmp.data.p.angle*M_1_PI);

	return 0;
}

int myvector_add(sVector *pA, const sVector *pB, const sVector *pC) {
	sVector B = *pB, C = *pC;
	if(myvector_convert(&B, 1) == -1) return -1;
	if(myvector_convert(&C, 1) == -1) return -1;
	double a, b;
	a = B.data.c.x + C.data.c.x;
	b = B.data.c.y + C.data.c.y;
	myvector_set(pA, 1, a, b);
	myvector_convert(pA, pB->type);
	return 0;
}

int myvector_inner_product(double *pA, const sVector *pB, const sVector *pC) {
	sVector B = *pB, C = *pC;
	if(myvector_convert(&B, 1) == -1) return -1;
	if(myvector_convert(&C, 1) == -1) return -1;
	*pA = B.data.c.x * C.data.c.x + B.data.c.y * C.data.c.y;
	return 0;
}

int myvector_area(double *pArea, const sVector *pB, const sVector *pC) {
	sVector B = *pB, C = *pC;
	if(myvector_convert(&B, 1) == -1) return -1;
	if(myvector_convert(&C, 1) == -1) return -1;
	*pArea = fabs(B.data.c.x * C.data.c.y - B.data.c.y * C.data.c.x);
	return 0;
}

int myvector_cvp(double *pX, double *pY, const double *pTx, const double *pTy, const sVector *pA, const sVector *pB) {
	sVector A = *pA, B = *pB;
	if(myvector_convert(&A, 1) == -1) return -1;
	if(myvector_convert(&B, 1) == -1) return -1;
	double denominator = A.data.c.x*B.data.c.y - B.data.c.x*A.data.c.y;
	if(denominator == 0) return -1;
	int m = roundf((B.data.c.y*(*pTx) - B.data.c.x*(*pTy)) / denominator);
	int n = roundf((A.data.c.y*(*pTx) - A.data.c.x*(*pTy)) / -denominator);
	*pX = m * A.data.c.x + n * B.data.c.x;
	*pY = n * A.data.c.y + n * B.data.c.y;
	return 0;
}

int myvector_simplified(sVector *pVector) {
	if(pVector->type != 1 && pVector->type != 2) return -1;
	if(pVector->type == 1) return 0;
	if(pVector->data.p.distance == 0) {
		pVector->data.p.angle = 0;
		return 0;
	}
	if(pVector->data.p.distance < 0) {
		pVector->data.p.angle += M_PI;
		pVector->data.p.distance *= -1;
	}
	pVector->data.p.angle = fmod(fmod(pVector->data.p.angle, M_PI) + M_PI, M_PI);
	return 0;
}

int myvector_convert(sVector *pVector, uint8_t type) {
	if(type != 1 && type != 2) return -1;
	if(pVector->type == type) return 0;
	double a, b;
	if(type == 1) {
		a = pVector->data.p.distance * cos(pVector->data.p.angle);
		b = pVector->data.p.distance * sin(pVector->data.p.angle);
	}
	else {
		myvector_simplified(pVector);
		a = sqrt((pVector->data.c.x)*(pVector->data.c.x)+(pVector->data.c.y)*(pVector->data.c.y));
		b = a ? atan2(pVector->data.c.y, pVector->data.c.x) : 0.0;
	}
	myvector_set(pVector, type, a, b);
	return 0;
}
