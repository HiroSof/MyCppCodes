#pragma once

/*************************************************************************
�֐���	�FHSCC_Max3
�@�\	�F3�̎����l���ő�̒l��Ԃ�
��`	�Fdouble HSCC_Max3(double num1,double num2,double num3);
����	�Fmaxnum = HSCC_Max3(num1,num2,num3);
�߂�l	�F�n���ꂽ�l�̂����ő�̒l

�p�����[�^����
maxnum�E�E�E�߂�l�̑����(�����^)
num1�Anum2�Anum3 �E�E�E ��r�����������R��
*************************************************************************/
double HSCC_Max3(double,double,double);

/*************************************************************************
�֐���	�FHSCC_Min3
�@�\	�F3�̎����l���ŏ��̒l��Ԃ�
��`	�Fdouble HSCC_Min3(double num1,double num2,double num3);
����	�Fminnum = HSCC_Min3(num1,num2,num3);
�߂�l	�F�n���ꂽ�l�̂����ŏ��̒l

�p�����[�^����
minnum�E�E�E�߂�l�̑����(�����^)
num1�Anum2�Anum3 �E�E�E ��r�����������R��
*************************************************************************/
double HSCC_Min3(double,double,double);

//�͈͓��̐������m�F
template<typename T> bool HSCC_IsRangeValue(T value , T min_v , T max_v);


//HSV�AHLS��H(Hue�A�F���j�̒l��0�`359.9�E�E�E�͈̔͂Ɏ��߂�
double HSCC_FitRangeHue(double);

/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HSV
�@�\	�FRGB�l��HSV�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HSV(double R,double G,double B,double *pH,double *pS,double *pV);
����	�FHSCC_RGB_to_HSV(R,G,B,&H,&S,&V);
�߂�l	�Ffalse�E�E�E�p�����[�^�Ɉُ킠�� �Atrue�E�E�E�ُ�Ȃ�

���̓p�����[�^����
R�E�E�ERGB�l��R(��)�̒l(�l�͈̔́F0.0�`1.0)
G�E�E�ERGB�l��G(��)�̒l(�l�͈̔́F0.0�`1.0)
B�E�E�ERGB�l��B(��)�̒l(�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
H�E�E�E�ϊ�����HSV�l��H�l(�F��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`359.9�E�E�E)
S�E�E�E�ϊ�����HSV�l��S�l(�ʓx)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
V�E�E�E�ϊ�����HSV�l��V�l(���x)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*****************************************************************************************/
bool HSCC_RGB_to_HSV(double, double, double, double*, double*, double*);


/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HSV
�@�\	�FRGB�l��HSV�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HSV(int R,int G,int B,double *pH,double *pS,double *pV);
����	�FHSCC_RGB_to_HSV(R,G,B,&H,&S,&V);
�߂�l	�Ffalse�E�E�E�p�����[�^�Ɉُ킠�� �Atrue�E�E�E�ُ�Ȃ�

���̓p�����[�^����
R�E�E�ERGB�l��R(��)�̒l(�l�͈̔́F0�`255)
G�E�E�ERGB�l��G(��)�̒l(�l�͈̔́F0�`255)
B�E�E�ERGB�l��B(��)�̒l(�l�͈̔́F0�`255)

�o�̓p�����[�^����
H�E�E�E�ϊ�����HSV�l��H�l(�F��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`359.9�E�E�E)
S�E�E�E�ϊ�����HSV�l��S�l(�ʓx)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
V�E�E�E�ϊ�����HSV�l��V�l(���x)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*****************************************************************************************/
bool HSCC_RGB_to_HSV(int, int, int, double*, double*, double*);


/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HLS
�@�\	�FRGB�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HLS(double R,double G,double B,double *pH,double *pL,double *pS);
����	�FHSCC_RGB_to_HLS(R,G,B,&H,&L,&S);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
R�E�E�ERGB�l��R(��)�̒l(�l�͈̔́F0.0�`1.0)
G�E�E�ERGB�l��G(��)�̒l(�l�͈̔́F0.0�`1.0)
B�E�E�ERGB�l��B(��)�̒l(�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
H�E�E�E�ϊ�����HLS�l��H�l(�F��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`359.9�E�E�E)
L�E�E�E�ϊ�����HLS�l��L�l(�P�x)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
S�E�E�E�ϊ�����HLS�l��S�l(�ʓx)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*****************************************************************************************/
bool HSCC_RGB_to_HLS(double, double, double, double*, double*, double*);

/*****************************************************************************************
�֐���	�FHSCC_RGB_to_HLS
�@�\	�FRGB�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_RGB_to_HLS(int R,int G,int B,double *pH,double *pL,double *pS);
����	�FHSCC_RGB_to_HLS(R,G,B,&H,&L,&S);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
R�E�E�ERGB�l��R(��)�̒l(�l�͈̔́F0�`255)
G�E�E�ERGB�l��G(��)�̒l(�l�͈̔́F0�`255)
B�E�E�ERGB�l��B(��)�̒l(�l�͈̔́F0�`255)

�o�̓p�����[�^����
H�E�E�E�ϊ�����HLS�l��H�l(�F��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`359.9�E�E�E)
L�E�E�E�ϊ�����HLS�l��L�l(�P�x)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
S�E�E�E�ϊ�����HLS�l��S�l(�ʓx)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*****************************************************************************************/
bool HSCC_RGB_to_HLS(int, int, int, double*, double*, double*);

/*************************************************************************
�֐���	�FHSCC_HSV_to_RGB
�@�\	�FHSV�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_RGB(double H,double S,double V,double *pR,double *pG,double *pB);
����	�FHSCC_HSV_to_RGB(H,S,V,&R,&G,&B);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
H�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
S�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)
V�E�E�EHSV�l��V�l(���x�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
R�E�E�ERGB�l��R�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
G�E�E�ERGB�l��G�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
B�E�E�ERGB�l��B�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*************************************************************************/
bool HSCC_HSV_to_RGB ( double , double , double , double* , double* , double* );


/*************************************************************************
�֐���	�FHSCC_HSV_to_RGB
�@�\	�FHSV�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_RGB(double H,double S,double V,float *pR,float *pG,float *pB);
����	�FHSCC_HSV_to_RGB(H,S,V,&R,&G,&B);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
H�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
S�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)
V�E�E�EHSV�l��V�l(���x�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
R�E�E�ERGB�l��R�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
G�E�E�ERGB�l��G�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
B�E�E�ERGB�l��B�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*************************************************************************/
bool HSCC_HSV_to_RGB ( double , double , double , float* , float* , float* );


/*************************************************************************
�֐���	�FHSCC_HSV_to_RGB
�@�\	�FHSV�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_RGB(double H,double S,double V,int *pR,int *pG,int *pB);
����	�FHSCC_HSV_to_RGB(H,S,V,&R,&G,&B);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
H�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
S�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)
V�E�E�EHSV�l��V�l(���x�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
R�E�E�ERGB�l��R�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0�`255)
G�E�E�ERGB�l��G�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0�`255)
B�E�E�ERGB�l��B�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0�`255)
*************************************************************************/
bool HSCC_HSV_to_RGB(double, double, double, int*, int*, int*);

/*************************************************************************
�֐���	�FHSCC_HLS_to_RGB
�@�\	�FHLS�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_RGB(double H,double L,double S,double *pR,double *pG,double *pB);
����	�FHSCC_HLS_to_RGB(H,L,S,&R,&G,&B);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
H�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
L�E�E�EHSV�l��L�l(�P�x�A�l�͈̔́F0.0�`1.0)
S�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
R�E�E�ERGB�l��R�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
G�E�E�ERGB�l��G�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
B�E�E�ERGB�l��B�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*************************************************************************/
bool HSCC_HLS_to_RGB(double, double, double, double*, double*, double*);

/*************************************************************************
�֐���	�FHSCC_HLS_to_RGB
�@�\	�FHLS�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_RGB(double H,double L,double S,float *pR,float *pG,float *pB);
����	�FHSCC_HLS_to_RGB(H,L,S,&R,&G,&B);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
H�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
L�E�E�EHSV�l��L�l(�P�x�A�l�͈̔́F0.0�`1.0)
S�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
R�E�E�ERGB�l��R�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
G�E�E�ERGB�l��G�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
B�E�E�ERGB�l��B�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0.0�`1.0)
*************************************************************************/
bool HSCC_HLS_to_RGB ( double , double , double , float* , float* , float* );

/*************************************************************************
�֐���	�FHSCC_HLS_to_RGB
�@�\	�FHLS�l��RGB�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_RGB(double H,double L,double S,int *pR,int *pG,int *pB);
����	�FHSCC_HLS_to_RGB(H,L,S,&R,&G,&B);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
H�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
L�E�E�EHSV�l��L�l(�P�x�A�l�͈̔́F0.0�`1.0)
S�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
R�E�E�ERGB�l��R�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0�`255)
G�E�E�ERGB�l��G�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0�`255)
B�E�E�ERGB�l��B�l(��)�̑����ւ̃|�C���^(�l�͈̔́F0�`255)
*************************************************************************/
bool HSCC_HLS_to_RGB(double, double, double, int*, int*, int*);

/*************************************************************************
�֐���	�FHSCC_HSV_to_HLS
�@�\	�FHSV�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_HSV_to_HLS(double HSV_h , double HSV_s ,double HSV_v  , double *pHLS_h ,  double *pHLS_l , double *pHLS_s);
����	�FHSCC_HSV_to_HLS(HSV_h , HSV_s ,HSV_v  , &HLS_h ,  &HLS_l , &HLS_s);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
HSV_h�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
HSV_s�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)
HSV_v�E�E�EHSV�l��V�l(���x�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
HLS_h�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
HLS_l�E�E�EHSV�l��L�l(�P�x�A�l�͈̔́F0.0�`1.0)
HLS_s�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)
*************************************************************************/
bool HSCC_HSV_to_HLS(double, double, double, double*, double*, double*);

/*************************************************************************
�֐���	�FHSCC_HLS_to_HSV
�@�\	�FHSV�l��HLS�l�ɕϊ�����
��`	�Fbool HSCC_HLS_to_HSV(double HLS_h , double HLS_l ,double HLS_s  , double *pHSV_h ,  double *pHSV_s , double *pHSV_v);
����	�FHSCC_HLS_to_HSV(HLS_h , HLS_l ,HLS_s  , &HSV_h ,  &HSV_s , &HSV_v);
�߂�l	�F0�E�E�E�o�̓p�����[�^�Ɉُ킠�� �A1�E�E�E�ُ�Ȃ�

���̓p�����[�^����
HLS_h�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
HLS_l�E�E�EHSV�l��L�l(�P�x�A�l�͈̔́F0.0�`1.0)
HLS_s�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)

�o�̓p�����[�^����
HSV_h�E�E�EHSV�l��H�l(�F���A�l�͈̔́F0.0�`359.9�E�E�E)
HSV_s�E�E�EHSV�l��S�l(�ʓx�A�l�͈̔́F0.0�`1.0)
HSV_v�E�E�EHSV�l��V�l(���x�A�l�͈̔́F0.0�`1.0)
*************************************************************************/
bool HSCC_HLS_to_HSV(double, double, double, double*, double*, double*);
