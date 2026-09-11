#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"

#define POINT_RAD	0.05f

float lambda = 0.0f;
TransformedView tv;

/*
float fn_g(float x){
	return 2.0f * F32_Sigmoid(10.0f * x) - 1.0f;
}
float fn_f(float x){
	return sinf(x);
}
float Function_1D(float x){
	//return x * sinf(x);
	//return F32_SMax(0.001f * x * x,sinf(x),lambda);
	//return expf(lambda * logf(x));

	float y = x;
	for(int i = 0;i<=(int)floorf(lambda);i++){
		y = fn_g(fn_f(y));
	}
	return y;
}
*/

Vec2* selected = NULL;
Vector points;
float Lagrange_Yi(int i,float x){
	const Vec2 points_i = *(Vec2*)Vector_Get(&points,i);

	float p = 1.0f;
	for(int j = 0;j<points.size;j++){
		const Vec2 points_j = *(Vec2*)Vector_Get(&points,j);

		if(i == j) continue;
		p *= x - points_j.x;
		p /= points_i.x - points_j.x;
	}
	return p * points_i.y;
}

float Function0(float x){
	return x * expf(x);
}
float Function1(float x){
	return x;
}
float Function2(float x){
	return Math_Nw_LambertW_0(x);
}
float Function3(float x){
	return Math_Hy_LambertW_0(x);
}
float Function4(float x){
	float p = 0.0f;
	for(int i = 0;i<points.size;i++){
		p += Lagrange_Yi(i,x);
	}
	return p;
}

void Plot_Function(float(*fn)(float),Pixel color){
	Vec2 PosBefore = {};
	for(int i = 0;i<GetWidth();i++){
		const float x = TransformedView_ScreenWorldX(&tv,i);
		const float y = fn(x);
		const Vec2 Pos = { i,TransformedView_WorldScreenY(&tv,-y) };
		RenderLine(PosBefore,Pos,color,1.0f);
		PosBefore = Pos;
	}
}

void Setup(AlxWindow* w){
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	points = Vector_New(sizeof(Vec2));
}
void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
	const Vec2 wm = Vec2_Mul(TransformedView_ScreenWorldPos(&tv,GetMouse()),(Vec2){ 1.0f,-1.0f });

	if(Stroke(ALX_KEY_UP).DOWN){
		lambda += 1.0f * w->ElapsedTime;
	}else if(Stroke(ALX_KEY_DOWN).DOWN){
		lambda -= 1.0f * w->ElapsedTime;
	}

	if(Stroke(ALX_KEY_1).PRESSED){
		selected = NULL;
		Vector_Push(&points,(Vec2[]){ wm });
	}else if(Stroke(ALX_KEY_2).PRESSED){
		selected = NULL;
		for(int i = 0;i<points.size;i++){
			Vec2* const points_i = (Vec2*)Vector_Get(&points,i);
			const Vec2 d = Vec2_Sub(wm,*points_i);
			if(Vec2_Mag2(d) < POINT_RAD * POINT_RAD){
				Vector_Remove(&points,i);
				break;
			}
		}
	}else if(Stroke(ALX_MOUSE_L).PRESSED){
		for(int i = 0;i<points.size;i++){
			Vec2* const points_i = (Vec2*)Vector_Get(&points,i);
			const Vec2 d = Vec2_Sub(wm,*points_i);
			if(Vec2_Mag2(d) < POINT_RAD * POINT_RAD)
				selected = points_i;
		}
	}else if(Stroke(ALX_MOUSE_L).DOWN){
		if(selected) *selected = wm;
	}else if(Stroke(ALX_MOUSE_L).RELEASED){
		selected = NULL;
	}

	Clear(BLACK);

	const Vec2 wvls = { 0.0f,TransformedView_ScreenWorldY(&tv,0.0f) };
	const Vec2 wvle = { 0.0f,TransformedView_ScreenWorldY(&tv,GetHeight()) };
	const Vec2 whls = { TransformedView_ScreenWorldX(&tv,0.0f),0.0f };
	const Vec2 whle = { TransformedView_ScreenWorldX(&tv,GetWidth()),0.0f };
	const Vec2 svls = TransformedView_WorldScreenPos(&tv,wvls);
	const Vec2 svle = TransformedView_WorldScreenPos(&tv,wvle);
	const Vec2 shls = TransformedView_WorldScreenPos(&tv,whls);
	const Vec2 shle = TransformedView_WorldScreenPos(&tv,whle);
	RenderLine(svls,svle,WHITE,1.0f);
	RenderLine(shls,shle,WHITE,1.0f);

	Plot_Function(Function0,GREEN);
	Plot_Function(Function1,RED);
	Plot_Function(Function2,BLUE);
	Plot_Function(Function3,YELLOW);
	Plot_Function(Function4,CYAN);

	const Vec2 len = TransformedView_WorldScreenLength(&tv,(Vec2){ POINT_RAD,POINT_RAD });
	for(int i = 0;i<points.size;i++){
		const Vec2 points_i = *(Vec2*)Vector_Get(&points,i);
		const Vec2 pos = TransformedView_WorldScreenPos(&tv,(Vec2){ points_i.x,-points_i.y });
		Circle_R_RenderWire(WINDOW_STD_ARGS,pos,len,WHITE,1.0f);
	}

	Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,0.0f,WHITE,"P: X: %f, Y: %f",p.x,p.y);
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,GetAlxFont()->CharSizeY,WHITE,"Lambda: %f",lambda);
}
void Delete(AlxWindow* w){
	Vector_Free(&points);
}

int main(){
    if(Create("Geogebra 2.1",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}