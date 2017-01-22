#pragma once
#include "HSColorConv.h"


/*************************************************************************
関数名	：HSCC_Max3
機能	：3つの実数値より最大の値を返す
定義	：double HSCC_Max3(double num1,double num2,double num3);
*************************************************************************/
double HSCC_Max3(double num1,double num2,double num3){
	double n;
	n = (num1 > num2) ? num1 : num2 ; 
	return (n > num3) ? n : num3;
}

/*************************************************************************
関数名	：HSCC_Min3
機能	：3つの実数値より最小の値を返す
定義	：double HSCC_Min3(double num1,double num2,double num3);
*************************************************************************/
double HSCC_Min3(double num1,double num2,double num3){
	double n;
	n = (num1 < num2) ? num1 : num2 ;
	return (n < num3) ? n : num3;
}

//範囲内の数かを確認
template<typename T> bool HSCC_IsRangeValue(T value, T min_v, T max_v) {
	return ((min_v <= value) && (max_v >= value))  ? true : false;
}

//HSV、HLSのH(Hue、色相）の値を0～359.9・・・の範囲に収める
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
関数名	：HSCC_RGB_to_HSV
機能	：RGB値をHSV値に変換する
定義	：bool HSCC_RGB_to_HSV(double R,double G,double B,double *pH,double *pS,double *pV);
*****************************************************************************************/
bool HSCC_RGB_to_HSV(double R,double G,double B,double *pH,double *pS,double *pV){

	double rgb[]={R,G,B},hsv[3];
	double minval,maxval;
	if((!pH) || (!pS) || (!pV))return false;
	if (!HSCC_IsRangeValue<double>(R,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(G,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(B,0,1)) return false;
	/*
		最小値と最大値を取得
	*/
	minval = HSCC_Min3(rgb[0],rgb[1],rgb[2]);
	maxval = HSCC_Max3(rgb[0],rgb[1],rgb[2]);

	/*
		最大値 = Vの値
	*/
	hsv[2] = maxval;

	/*
		Hの値を計算
	*/
	if(minval == maxval){
		//最小値と最大値が等しい場合Hは定義されない。
		hsv[0]=0.0;
	}else{
		//Rが最大の時60×(G-B)÷(最大値-最小値)+0
		if(maxval==rgb[0])hsv[0]=60.0*(rgb[1]-rgb[2])/(maxval-minval);

		//Gが最大の時60×(B-R)÷(最大値-最小値)+120
		else if(maxval==rgb[1])hsv[0]=60.0*(rgb[2]-rgb[0])/(maxval-minval)+120;
	
		//Bが最大の時60×(R-G)÷(最大値-最小値)+240
		else hsv[0]=60.0*(rgb[0]-rgb[1])/(maxval-minval)+240;
		
		//上の計算でマイナス値になった場合360.0を加算
		if(hsv[0] <0)hsv[0]+=360.0;
	}

	/*
		Sの値を計算
	*/	
	if(maxval==0)hsv[1]=0;//最大値が0の場合Sは未定義
	else hsv[1]=(maxval-minval)/maxval;

	
	/*
		結果を代入
	*/
	*pH=hsv[0]; *pS=hsv[1]; *pV=hsv[2];
	return true;
}

/*****************************************************************************************
関数名	：HSCC_RGB_to_HSV
機能	：RGB値をHSV値に変換する
定義	：bool HSCC_RGB_to_HSV(int R,int G,int B,double *pH,double *pS,double *pV);
*****************************************************************************************/
bool HSCC_RGB_to_HSV(int R, int G, int B, double *pH, double *pS, double *pV) {
	double rgb[]={R,G,B};
	return HSCC_RGB_to_HSV(rgb[0] / 255, rgb[1] / 255, rgb[2] / 255, pH , pS , pV);
}

/*****************************************************************************************
関数名	：HSCC_RGB_to_HLS
機能	：RGB値をHLS値に変換する
定義	：bool HSCC_RGB_to_HLS(double R,double G,double B,double *pH,double *pL,double *pS);
*****************************************************************************************/
bool HSCC_RGB_to_HLS(double R, double G, double B, double *pH, double *pL, double*pS) {
	double  valRGB[] = { R , G , B } , valHLS[3]={ 0 , 0 , 0 };
	double maxNum = 0 , minNum=0;
	if((!pH) || (!pL) || (!pS))return false;
	if (!HSCC_IsRangeValue<double>(R,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(G,0,1)) return false;
	if (!HSCC_IsRangeValue<double>(B,0,1)) return false;

	//最小値と最大値を取得
	maxNum = HSCC_Max3(valRGB[0],valRGB[1],valRGB[2]);
	minNum = HSCC_Min3(valRGB[0],valRGB[1],valRGB[2]);

	//L=(最大値 + 最小値)/2
	valHLS[1] =(maxNum + minNum)/2;

	//SとHの値を取得
	if(maxNum == minNum){
		//最大値 = 最小値の時 HとSはともに0である;
		valHLS[0] = valHLS[2] = 0;
	}else{
		/*
			Sを計算
		*/
		//L≦0.5の時 S = (最大値 - 最小値)/(最大値 + 最小値)
		if(valHLS[1]<=0.5)valHLS[2] = (maxNum - minNum)/(maxNum + minNum);
		
		//L>0.5の時　S = (最大値 - 最小値)/(2 - 最大値 - 最小値)
		else valHLS[2] = (maxNum - minNum)/(2 - maxNum - minNum);
	
		/*
			Hを計算（計算式はHSVのHと同じ)
		*/
		//Rが最大の時60×(G-B)÷(最大値-最小値)+0
		if(maxNum==valRGB[0])valHLS[0]=60.0*(valRGB[1]-valRGB[2])/(maxNum-minNum);

		//Gが最大の時60×(B-R)÷(最大値-最小値)+120
		else if(maxNum==valRGB[1])valHLS[0]=60.0*(valRGB[2]-valRGB[0])/(maxNum-minNum)+120;

		//Bが最大の時60×(R-G)÷(最大値-最小値)+240
		else valHLS[0]=60.0*(valRGB[0]-valRGB[1])/(maxNum-minNum)+240;
		
		//上の計算でマイナス値になった場合360.0を加算
		if(valHLS[0] <0)valHLS[0]+=360.0;
	}
	
	/*
		*pH , *pL , *pSに代入
	*/
	*pH = valHLS[0] ; *pL = valHLS[1] ; *pS = valHLS[2];

	return true;

}

/*****************************************************************************************
関数名	：HSCC_RGB_to_HLS
機能	：RGB値をHLS値に変換する
定義	：bool HSCC_RGB_to_HLS(int R,int G,int B,double *pH,double *pL,double *pS);
*****************************************************************************************/
bool HSCC_RGB_to_HLS(int R, int G, int B, double *pH, double *pL, double *pS) {
	double rgb[]={R,G,B};
	return HSCC_RGB_to_HLS(rgb[0] / 255, rgb[1] / 255, rgb[2] / 255, pH , pL , pS);
}


/*************************************************************************
関数名	：HSCC_HSV_to_RGB
機能	：HSV値をRGB値に変換する
定義	：bool HSCC_HSV_to_RGB(double H,double S,double V,double *pR,double *pG,double *pB);
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
関数名	：HSCC_HSV_to_RGB
機能	：HSV値をRGB値に変換する
定義	：bool HSCC_HSV_to_RGB(double H,double S,double V,float *pR,float *pG,float *pB);
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
関数名	：HSCC_HSV_to_RGB
機能	：HSV値をRGB値に変換する
定義	：bool HSCC_HSV_to_RGB(double H,double S,double V,int *pR,int *pG,int *pB);
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
関数名	：HSCC_HLS_to_RGB
機能	：HLS値をRGB値に変換する
定義	：bool HSCC_HLS_to_RGB(double H,double L,double S,double *pR,double *pG,double *pB);
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

		//Rの値を計算
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

		//Gの値を計算
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

		//Bの値を計算
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
関数名	：HSCC_HLS_to_RGB
機能	：HLS値をRGB値に変換する
定義	：bool HSCC_HLS_to_RGB(double H,double L,double S,float *pR,float *pG,float *pB);
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
関数名	：HSCC_HLS_to_RGB
機能	：HLS値をRGB値に変換する
定義	：bool HSCC_HLS_to_RGB(double H,double L,double S,int *pR,int *pG,int *pB);
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
関数名	：HSCC_HSV_to_HLS
機能	：HSV値をHLS値に変換する
定義	：bool HSCC_HSV_to_HLS(double HSV_h , double HSV_s ,double HSV_v  , double *pHLS_h ,  double *pHLS_l , double *pHLS_s);
*************************************************************************/
bool HSCC_HSV_to_HLS(double HSV_h, double HSV_s, double HSV_v, double *pHLS_h, double *pHLS_l, double *pHLS_s) {
	double dR , dG , dB;
	if (!HSCC_HSV_to_RGB(HSV_h , HSV_s , HSV_v , &dR , &dG , &dB)) return false;
	return HSCC_RGB_to_HLS(dR, dG , dB , pHLS_h , pHLS_l , pHLS_s);
}

/*************************************************************************
関数名	：HSCC_HLS_to_HSV
機能	：HSV値をHLS値に変換する
定義	：bool HSCC_HLS_to_HSV(double HLS_h , double HLS_l ,double HLS_s  , double *pHSV_h ,  double *pHSV_s , double *pHSV_v);
*************************************************************************/
bool HSCC_HLS_to_HSV(double HLS_h, double HLS_l, double HLS_s, double *pHSV_h, double *pHSV_s, double *pHSV_v) {
	double dR , dG , dB;
	if (!HSCC_HLS_to_RGB(HLS_h , HLS_l , HLS_s , &dR , &dG , &dB)) return false;
	return HSCC_RGB_to_HSV(dR, dG , dB , pHSV_h , pHSV_s , pHSV_v);
}