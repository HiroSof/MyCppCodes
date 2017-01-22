#pragma once
#include "HSColorConv.h"


/*************************************************************************
�֐���	�FHSCC_Max3
�@�\	�F3�̎����l���ő�̒l��Ԃ�
��`	�Fdouble HSCC_Max3(double num1,double num2,double num3);
*************************************************************************/
double HSCC_Max3(double num1,double num2,double num3){
	double n;
	n = (num1 > num2) ? num1 : num2 ; 
	return (n > num3) ? n : num3;
}

/*************************************************************************
�֐���	�FHSCC_Min3
�@�\	�F3�̎����l���ŏ��̒l��Ԃ�
��`	�Fdouble HSCC_Min3(double num1,double num2,double num3);
*************************************************************************/
double HSCC_Min3(double num1,double num2,double num3){
	double n;
	n = (num1 < num2) ? num1 : num2 ;
	return (n < num3) ? n : num3;
}

//�͈͓��̐������m�F
template<typename T> bool HSCC_IsRangeValue(T value, T min_v, T max_v) {
	return ((min_v <= value) && (max_v >= value))  ? true : false;
}

//HSV�AHLS��H(Hue�A�F���j�̒l��0�`359.9�E�E�E�͈̔͂Ɏ��߂�
double HSCC_FitRangeHue(double hue) {
/*
	double rethue = inhue;

	while (rethue >= 360) {
		rethue -= 360;
	}
	while (rethue < 0) {
		rethue += 360;
	}

	return rethue;
	*/

	int ihue = static_cast< int >( hue );
	int ihue_s = ihue % 360;
	if ( ihue_s >= 0 ) {
		return ihue_s + ( hue - ihue );
	} else {
		return ihue_s + ( hue - ihue ) + 360;
	}

}


/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HSV
�@�\	�FRGB�l��HSV�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HSV(double R,double G,double B,double *pH,double *pS,double *pV);
*****************************************************************************************/
bool HSCC_RGB_to_HSV(double R,double G,double B,double *pH,double *pS,double *pV){

	double rgb[]={R,G,B},hsv[3];
	double minval,maxval;
	if((!pH) || (!pS) || (!pV))return false;
	if (!HSCC_IsRangeValue<double>(R,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(G,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(B,0,1)) return false;
	/*
		�ŏ��l�ƍő�l���擾
	*/
	minval = HSCC_Min3(rgb[0],rgb[1],rgb[2]);
	maxval = HSCC_Max3(rgb[0],rgb[1],rgb[2]);

	/*
		�ő�l = V�̒l
	*/
	hsv[2] = maxval;

	/*
		H�̒l���v�Z
	*/
	if(minval == maxval){
		//�ŏ��l�ƍő�l���������ꍇH�͒�`����Ȃ��B
		hsv[0]=0.0;
	}else{
		//R���ő�̎�60�~(G-B)��(�ő�l-�ŏ��l)+0
		if(maxval==rgb[0])hsv[0]=60.0*(rgb[1]-rgb[2])/(maxval-minval);

		//G���ő�̎�60�~(B-R)��(�ő�l-�ŏ��l)+120
		else if(maxval==rgb[1])hsv[0]=60.0*(rgb[2]-rgb[0])/(maxval-minval)+120;
	
		//B���ő�̎�60�~(R-G)��(�ő�l-�ŏ��l)+240
		else hsv[0]=60.0*(rgb[0]-rgb[1])/(maxval-minval)+240;
		
		//��̌v�Z�Ń}�C�i�X�l�ɂȂ����ꍇ360.0�����Z
		if(hsv[0] <0)hsv[0]+=360.0;
	}

	/*
		S�̒l���v�Z
	*/	
	if(maxval==0)hsv[1]=0;//�ő�l��0�̏ꍇS�͖���`
	else hsv[1]=(maxval-minval)/maxval;

	
	/*
		���ʂ���
	*/
	*pH=hsv[0]; *pS=hsv[1]; *pV=hsv[2];
	return true;
}

/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HSV
�@�\	�FRGB�l��HSV�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HSV(int R,int G,int B,double *pH,double *pS,double *pV);
*****************************************************************************************/
bool HSCC_RGB_to_HSV(int R, int G, int B, double *pH, double *pS, double *pV) {
	double rgb[]={R,G,B};
	return HSCC_RGB_to_HSV(rgb[0] / 255, rgb[1] / 255, rgb[2] / 255, pH , pS , pV);
}

/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HLS
�@�\	�FRGB�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HLS(double R,double G,double B,double *pH,double *pL,double *pS);
*****************************************************************************************/
bool HSCC_RGB_to_HLS(double R, double G, double B, double *pH, double *pL, double*pS) {
	double  valRGB[] = { R , G , B } , valHLS[3]={ 0 , 0 , 0 };
	double maxNum = 0 , minNum=0;
	if((!pH) || (!pL) || (!pS))return false;
	if (!HSCC_IsRangeValue<double>(R,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(G,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(B,0,1)) return false;

	//�ŏ��l�ƍő�l���擾
	maxNum = HSCC_Max3(valRGB[0],valRGB[1],valRGB[2]);
	minNum = HSCC_Min3(valRGB[0],valRGB[1],valRGB[2]);

	//L=(�ő�l + �ŏ��l)/2
	valHLS[1] =(maxNum + minNum)/2;

	//S��H�̒l���擾
	if(maxNum == minNum){
		//�ő�l = �ŏ��l�̎� H��S�͂Ƃ���0�ł���;
		valHLS[0] = valHLS[2] = 0;
	}else{
		/*
			S���v�Z
		*/
		//L��0.5�̎� S = (�ő�l - �ŏ��l)/(�ő�l + �ŏ��l)
		if(valHLS[1]<=0.5)valHLS[2] = (maxNum - minNum)/(maxNum + minNum);
		
		//L>0.5�̎��@S = (�ő�l - �ŏ��l)/(2 - �ő�l - �ŏ��l)
		else valHLS[2] = (maxNum - minNum)/(2 - maxNum - minNum);
	
		/*
			H���v�Z�i�v�Z����HSV��H�Ɠ���)
		*/
		//R���ő�̎�60�~(G-B)��(�ő�l-�ŏ��l)+0
		if(maxNum==valRGB[0])valHLS[0]=60.0*(valRGB[1]-valRGB[2])/(maxNum-minNum);

		//G���ő�̎�60�~(B-R)��(�ő�l-�ŏ��l)+120
		else if(maxNum==valRGB[1])valHLS[0]=60.0*(valRGB[2]-valRGB[0])/(maxNum-minNum)+120;

		//B���ő�̎�60�~(R-G)��(�ő�l-�ŏ��l)+240
		else valHLS[0]=60.0*(valRGB[0]-valRGB[1])/(maxNum-minNum)+240;
		
		//��̌v�Z�Ń}�C�i�X�l�ɂȂ����ꍇ360.0�����Z
		if(valHLS[0] <0)valHLS[0]+=360.0;
	}
	
	/*
		*pH , *pL , *pS�ɑ��
	*/
	*pH = valHLS[0] ; *pL = valHLS[1] ; *pS = valHLS[2];

	return true;

}

/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HLS
�@�\	�FRGB�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HLS(int R,int G,int B,double *pH,double *pL,double *pS);
*****************************************************************************************/
bool HSCC_RGB_to_HLS(int R, int G, int B, double *pH, double *pL, double *pS) {
	double rgb[]={R,G,B};
	return HSCC_RGB_to_HLS(rgb[0] / 255, rgb[1] / 255, rgb[2] / 255, pH , pL , pS);
}


/*************************************************************************
�֐���	�FHSCC_HSV_to_RGB
�@�\	�FHSV�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_RGB(double H,double S,double V,double *pR,double *pG,double *pB);
*************************************************************************/
bool HSCC_HSV_to_RGB(double H, double S, double V, double *pR, double *pG, double *pB) {

	if((!pR) || (!pG) || (!pB))return false;
	if (!HSCC_IsRangeValue<double>(S,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(V,0,1)) return false;

	H = HSCC_FitRangeHue(H);

	if (S == 0) {
		*pR = *pG = *pB = V;
	} else {
		int hi;
		double f,p,q,t,fr,fg,fb;
		hi=((int)(H/60.0))%6;

		f=H/60.0-hi;

		p = V * (1 - S);
	
		q = V * (1 - f*S);

		t = V * (1 - (1 - f)*S);
	
		switch(hi){
			case 0:
				fr = V ; fg = t ; fb = p;
				break;
			case 1:
				fr = q ; fg = V ; fb = p;
				break;
			case 2:
				fr = p ; fg = V ; fb = t;
				break;
			case 3:
				fr = p ; fg = q ; fb = V;
				break;
			case 4:
				fr = t ; fg = p ; fb = V;
				break;
			case 5:
				fr = V ; fg = p ; fb = q;
				break;
		}

		*pR = fr;
		*pG = fg; 
		*pB = fb;
	}
	return true;
}


/*************************************************************************
�֐���	�FHSCC_HSV_to_RGB
�@�\	�FHSV�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_RGB(double H,double S,double V,float *pR,float *pG,float *pB);
*************************************************************************/
bool HSCC_HSV_to_RGB ( double H , double S , double V , float *pR , float *pG , float *pB ) {
	double dR , dG , dB;
	if((!pR) || (!pG) || (!pB))return false;
	if (!HSCC_HSV_to_RGB(H , S , V , &dR , &dG , &dB))return false;
	*pR = (float) dR;
	*pG = (float) dG;
	*pB = (float) dB;
	return true;
}


/*************************************************************************
�֐���	�FHSCC_HSV_to_RGB
�@�\	�FHSV�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_RGB(double H,double S,double V,int *pR,int *pG,int *pB);
*************************************************************************/
bool HSCC_HSV_to_RGB(double H, double S, double V, int *pR, int *pG, int *pB) {
	double dR , dG , dB;
	if((!pR) || (!pG) || (!pB))return false;
	if (!HSCC_HSV_to_RGB(H , S , V , &dR , &dG , &dB))return false;
	*pR = (int)(dR * 255);
	*pG = (int)(dG * 255);
	*pB = (int)(dB * 255);
	return true;
}


/*************************************************************************
�֐���	�FHSCC_HLS_to_RGB
�@�\	�FHLS�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_RGB(double H,double L,double S,double *pR,double *pG,double *pB);
*************************************************************************/
bool HSCC_HLS_to_RGB(double H, double L, double S, double *pR, double *pG, double *pB) {

	if((!pR) || (!pG) || (!pB))return false;
	if (!HSCC_IsRangeValue<double>(L,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(S,0,1)) return false;

	if (S == 0) {
		*pR = *pG = *pB = L;
	} else {
		double valRGB[3]={0,0,0},valH,nMax,nMin;

		nMax = (L <=0.5) ? L*(1 + S) : L*(1 - S) + S ;
		nMin = 2*L - nMax;

		//R�̒l���v�Z
		valH = HSCC_FitRangeHue(H + 120);
		if((valH >= 0) && (valH < 60)){
			valRGB[0] = nMin + (nMax - nMin) * valH / 60;
		}else if((valH >= 60) && (valH<180)){
			valRGB[0] = nMax;
		}else if((valH >= 180) && (valH<240)){
			valRGB[0] = nMin + (nMax - nMin)*(240 - valH) / 60;
		}else{
			valRGB[0] = nMin;
		}

		//G�̒l���v�Z
		valH = HSCC_FitRangeHue(H);
		if((valH >= 0) && (valH < 60)){
			valRGB[1] = nMin + (nMax - nMin) * valH / 60;
		}else if((valH >= 60) && (valH<180)){
			valRGB[1] = nMax;
		}else if((valH >= 180) && (valH<240)){
			valRGB[1] = nMin + (nMax - nMin)*(240 - valH) / 60;
		}else{
			valRGB[1] = nMin;
		}

		//B�̒l���v�Z
		valH = HSCC_FitRangeHue(H - 120);
		if((valH >= 0) && (valH < 60)){
			valRGB[2] = nMin + (nMax - nMin) * valH / 60;
		}else if((valH >= 60) && (valH<180)){
			valRGB[2] = nMax;
		}else if((valH >= 180) && (valH<240)){
			valRGB[2] = nMin + (nMax - nMin)*(240 - valH) / 60;
		}else{
			valRGB[2] = nMin;
		}

		*pR = valRGB[0];
		*pG = valRGB[1];
		*pB = valRGB[2];
	}

	return true;
}

/*************************************************************************
�֐���	�FHSCC_HLS_to_RGB
�@�\	�FHLS�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_RGB(double H,double L,double S,float *pR,float *pG,float *pB);
*************************************************************************/
bool HSCC_HLS_to_RGB ( double H , double L , double S , float *pR , float *pG , float *pB ) {
	double dR , dG , dB;
	if((!pR) || (!pG) || (!pB))return false;
	if (!HSCC_HLS_to_RGB(H , L , S , &dR , &dG , &dB))return false;
	*pR = (float) dR;
	*pG = (float) dG;
	*pB = (float) dB;
	return true;
}
/*************************************************************************
�֐���	�FHSCC_HLS_to_RGB
�@�\	�FHLS�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_RGB(double H,double L,double S,int *pR,int *pG,int *pB);
*************************************************************************/
bool HSCC_HLS_to_RGB(double H, double L, double S, int *pR, int *pG, int *pB) {
	double dR , dG , dB;
	if((!pR) || (!pG) || (!pB))return false;
	if (!HSCC_HLS_to_RGB(H , L , S , &dR , &dG , &dB))return false;
	*pR = (int)(dR * 255);
	*pG = (int)(dG * 255);
	*pB = (int)(dB * 255);
	return true;
}


/*************************************************************************
�֐���	�FHSCC_HSV_to_HLS
�@�\	�FHSV�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_HLS(double HSV_h , double HSV_s ,double HSV_v  , double *pHLS_h ,  double *pHLS_l , double *pHLS_s);
*************************************************************************/
bool HSCC_HSV_to_HLS(double HSV_h, double HSV_s, double HSV_v, double *pHLS_h, double *pHLS_l, double *pHLS_s) {
	double dR , dG , dB;
	if (!HSCC_HSV_to_RGB(HSV_h , HSV_s , HSV_v , &dR , &dG , &dB)) return false;
	return HSCC_RGB_to_HLS(dR, dG , dB , pHLS_h , pHLS_l , pHLS_s);
}

/*************************************************************************
�֐���	�FHSCC_HLS_to_HSV
�@�\	�FHSV�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_HSV(double HLS_h , double HLS_l ,double HLS_s  , double *pHSV_h ,  double *pHSV_s , double *pHSV_v);
*************************************************************************/
bool HSCC_HLS_to_HSV(double HLS_h, double HLS_l, double HLS_s, double *pHSV_h, double *pHSV_s, double *pHSV_v) {
	double dR , dG , dB;
	if (!HSCC_HLS_to_RGB(HLS_h , HLS_l , HLS_s , &dR , &dG , &dB)) return false;
	return HSCC_RGB_to_HSV(dR, dG , dB , pHSV_h , pHSV_s , pHSV_v);
}